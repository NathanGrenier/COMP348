from shapes import Shape, Circle, Ellipse, Rhombus
import commands
from enum import Enum
import sys
import inspect

class Commands(Enum):
    LOAD = [1, "<filename>"]
    TOSET = [2]
    SAVE = [3, "<filename>"]
    PRINT = [4]
    SUMMARY = [5]
    DETAILS = [6]
    QUIT = [7]

def error(msg):
    sys.stderr.write(f"Error: {msg}")
    sys.exit(1)

def linenum():
    return inspect.currentframe().f_back.f_lineno

def displayMenu():
    dashCount = 25
    print()
    print("-" * dashCount)
    print("| Commands:" + " " * (dashCount - 12) + "|")
    for cmd in (Commands):
        print(f"| {cmd.name + ' ' + ''.join(cmd.value[1:]):{dashCount - 3}}|")
    print("-" * dashCount)

def getCmd():
    cmd = input("Please enter a command: ")
    args = cmd.split(" ")
    
    for possibleCmd in (Commands):
        if args[0].upper() == possibleCmd.name:
            if (checkArgs(possibleCmd, args) == False):
                return getCmd()
            return (possibleCmd, args)
    sys.stderr.write(f"{args[0]} is not a valid command.\n")
    return getCmd()

def checkArgs(cmd, args):
    if len(args) != len(cmd.value):
        sys.stderr.write(f"Wrong number of arguments passed. Should be {len(cmd.value)}\n")
        return False
    else:
        return True

def main():
    displayMenu()
    cmd, args = getCmd()
    commands.init()
    while cmd != Commands.QUIT:
        if (cmd == Commands.LOAD):
            commands.load(args[1])
        elif (cmd == Commands.TOSET):
            commands.toSet()
        elif (cmd == Commands.SAVE):
            commands.save(args[1])
        elif (cmd == Commands.PRINT):
            commands.printShapes()
        elif (cmd == Commands.SUMMARY):
            commands.summary() 
        elif (cmd == Commands.DETAILS):
            commands.displayDetails()   
        displayMenu()
        cmd, args = getCmd()
    print("Exited.")

if __name__ == "__main__":
    main()