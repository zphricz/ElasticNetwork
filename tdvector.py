import math

class TDVector():
    def __init__(self, xComp, yComp):
        self.xComp = xComp
        self.yComp = yComp

    def magnitude(self):
        return math.sqrt(self.xComp * self.xComp + self.yComp * self.yComp)

    def magnitudeSquare(self):
        return self.xComp * self.xComp + self.yComp * self.yComp

    def angle(self):
        if self.xComp > 0:
            if self.yComp >= 0:
                return math.atan(self.yComp/self.xComp)
            return math.atan(self.yComp/self.xComp) + 2 * math.pi
        if self.xComp < 0:
            return math.atan(self.yComp/self.xComp) + math.pi
        if self.xComp == 0:
            if self.yComp > 0:
                return math.pi / 2
            if self.yComp < 0:
                return 3 * math.pi / 2
            return float('NaN')

    def scale(self, factor):
        return TDVector(self.xComp * factor, self.yComp * factor)

    def add(self, Vec):
        self.xComp += Vec.xComp
        self.yComp += Vec.yComp

    def subtract(self, Vec):
        self.xComp -= Vec.xComp
        self.yComp -= Vec.yComp

def difference(Vec1, Vec2):
    return TDVector(Vec1.xComp - Vec2.xComp, Vec1.yComp - Vec2.yComp)

