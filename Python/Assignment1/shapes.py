import math

class Shape(object):
    _id = 1
    instances = []
    def __init__(self):
        self._id = Shape._id
        Shape._id += 1
        Shape.instances.append(self)

    @property
    def id(self):
        return self._id
    
    def perimiter(self):
        return None
    
    def area(self):
        return None
    
    @staticmethod
    def clear():
        Shape._id = 1
        Shape.instances = []

    def __str__(self):
        str = f"{self._id}: {type(self).__name__}"
        str += f", perimeter: {self.perimiter():.5f}" if self.perimiter() is not None else ", perimeter: undefined"
        str += f", area: {self.area():.5f}" if self.area() is not None else ", area: undefined"
        return str
    
    def __eq__(self, other):
        if type(self) != type(other):
            return NotImplemented
        return True
    
    def __hash__(self):
        return hash(type(self).__name__)

    def details(self):
        return f"{type(self).__name__.lower()}"

class Circle(Shape):
    def __init__(self, radius):
        super().__init__()
        self.radius = radius

    def perimiter(self):
        return (2 * math.pi) * self.radius
    
    def area(self):
        return math.pi * math.pow(self.radius,2)
    
    def __eq__(self, other):
        if type(self) != type(other):
            return NotImplemented
        return self.radius == other.radius
    
    def __hash__(self):
        return hash(type(self).__name__ + str(self.radius))
    
    def details(self):
        return super().details() + f" {self.radius}"
    
class Ellipse(Shape):
    def __init__(self, axis1, axis2):
        super().__init__()
        self.semi_minor = min(axis1, axis2)
        self.semi_major = max(axis1, axis2)
    
    def area(self):
        return math.pi * self.semi_major * self.semi_minor
    
    def eccentricity(self):
        dif = math.pow(self.semi_major,2) - math.pow(self.semi_minor,2)
        if dif < 0: 
            return None
        return math.sqrt(dif)
    
    def __str__(self):
        return f"{super().__str__()}" + f", linear eccentricity: {self.eccentricity():.5f}" if self.eccentricity() is not None else ", linear eccentricity: undefined"
    
    # Even if 2 ellipses with ax1 and ax2 swaped in Ellipse(ax1, ax2) are compared, the result will be true because of the __init__ where we use max() and min() 
    def __eq__(self, other):
        if type(self) != type(other):
            return NotImplemented
        return (self.semi_minor, self.semi_major) == (other.semi_minor, other.semi_major)
    
    def __hash__(self):
        return hash(type(self).__name__ + str(self.semi_minor) + str(self.semi_major))
    
    # This function may swap the order of the arguments (axes). When recreating the object, this won't matter
    def details(self):
        return super().details() + f" {self.semi_minor} {self.semi_major}"
    
class Rhombus(Shape):
    def __init__(self, p, q):
        super().__init__()
        self.p = p
        self.q = q
    
    def perimiter(self):
        return 2 * math.sqrt(math.pow(self.p, 2) + math.pow(self.q, 2))
    
    def area(self):
        return (self.p * self.q) / 2
    
    def side(self):
        return math.sqrt(math.pow(self.p, 2) + math.pow(self.q, 2)) / 2

    def inradius(self):
        if (self.p == 0 and self.q == 0): return None
        return (self.p * self.q) / (2 * math.sqrt(math.pow(self.p, 2) + math.pow(self.q, 2)))
    
    def __str__(self):
        return f"{super().__str__()}, side: {self.side():.5f}, in-radius: {self.inradius():.5f}"
    
    def __eq__(self, other):
        if type(self) != type(other):
            return NotImplemented
        return (self.p, self.q) == (other.p, other.q)
    
    def __hash__(self):
        return hash(type(self).__name__ + str(self.p) + str(self.q))
    
    def details(self):
        return super().details() + f" {self.p} {self.q}"

