import inspect
import shapes
from shapedb import error, linenum
import os
import sys
import re

objects = []
classes = []
details = {}
def init():
    getShapeClasses()
    initShapeDetails()

def getShapeClasses():
    for name, obj in inspect.getmembers(shapes):
        if (inspect.isclass(obj)):
            classes.append(obj)

def initShapeDetails():
    global details
    details = {c.__name__:0 for c in classes}


def isNumeric(value):
    if isinstance(value, int) or isinstance(value, float):
        return True
    elif isinstance(value, str) and bool(re.search(r'-?\d+', value)):
        return True
    return False

def checkErroneous(arr):
    for value in arr:
        if not isNumeric(value) or float(value) <= 0:
            return True
    return False 

def updateShapeSummary():
    for cls in details.keys():
        if cls == "Error": continue
        details[cls] = 0
    for shape in objects:
        details[shape.__class__.__name__] += 1
        if shape.__class__.__name__ != "Shape":
            details["Shape"] += 1
            

def load(filename):
    if (not os.path.exists(filename)):
        sys.stderr.write(f"Error on line {linenum()}: The file <<{filename}>> could not be found.\n")
        return
    with open(filename, 'r') as f:
        print(f"Processing <<{filename}>>")
        rows = 0
        errCount = 0
        shapeCount = 0
        for line in f:
            line = line.rstrip()
            vars = line.split(" ")
            rows += 1
            if (checkErroneous(vars[1:])):
                print(f"Error: Invalid {vars[0]} in file {filename} on line {rows}: {' '.join(str(x) for x in vars)}")
                errCount += 1
                continue
            for i, value in enumerate(vars[1:], 1):
                vars[i] = int(value)
            for c in classes:
                if (vars[0].lower() == c.__name__.lower()):
                    try:
                        objects.append(c(*vars[1:]))
                        shapeCount += 1
                    except TypeError:
                        print(f"Error: Invalid args {vars[1:]} for class {c.__name__}. Expected: {len(inspect.signature(c.__init__).parameters)-1} arguments.")
                        errCount += 1
    updateShapeSummary()
    print(f"Processed {rows} row(s), {shapeCount} shape(s) added, {errCount} error(s)")
        
def save(output):
    writes = 0
    try:
        with open(output, 'w') as f:
            for obj in objects:
                f.write(obj.details() + "\n")
                writes += 1
        print(f"Wrote {writes} shapes to {output}.")
    except IOError as e:
        print(e)
    

def printShapes():
    if (len(objects) == 0):
        print("No shapes to print.")
    for obj in objects:
        print(obj)

def summary():
    names = [c.__name__ for c in classes]
    names.sort()
    for c in names:
        print(f"{c}(s):" if c[-1] != 's' else f"{c}(es):", details[c])

def displayDetails():
    if (len(objects) == 0):
        print("No details to display.")
    for obj in objects:
        print(obj.details())

def getRemoved(list1, list2):
    removed = []
    for x in list1:
        contains = False
        for y in list2:
            if x.id == y.id:
                contains = True
                break
        if (contains == False):
            removed.append(x)
    return removed

def toSet():
    global objects
    shapeSet = set(objects)
    removed = getRemoved(objects, list(shapeSet))
    if (len(removed) == 0):
        print("No duplicate shapes were removed.")
    else:
        for obj in removed:
            print(f"Removed: {obj}")
    objects = list(shapeSet)
    objects.sort(key=lambda obj: obj.id)
    updateShapeSummary()

            
