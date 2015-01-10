import tdvector


class Node():
    def __init__(self, x, y):
        self.position = tdvector.TDVector(x, y)
        self.velocity = tdvector.TDVector(0.0, 0.0)
        self.forceSum = tdvector.TDVector(0.0, 0.0)
        self.connected = []
        self.mainConnection = False

    def __del__(self):
        for node in self.connected:
            node.connected.remove(self)

    def connect(self, node):
        if node.mainConnection:
            self.mainConnection = True
            self.setMainConnection([])
        elif self.mainConnection:
            node.mainConnection = True
            node.setMainConnection([])
        self.connected.append(node)
        node.connected.append(self)

    def dampVelocity(self, dampFactor):
        self.velocity.xComp *= dampFactor
        self.velocity.yComp *= dampFactor

    def zero(self):
        self.forceSum.xComp = 0.0
        self.forceSum.yComp = 0.0

    def setMainConnection(self, alreadySet):
        temp = []
        for node in self.connected:
            if node not in alreadySet and not node.mainConnection:
                node.mainConnection = True
                alreadySet.append(node)
                temp.append(node)
        for node in temp:
            node.setMainConnection(alreadySet)

