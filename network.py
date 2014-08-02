import math
from PySide import QtCore

import node
import tdvector

class Network():

    def __init__(self, timeStep = 4.0):

            self.nodes = []
            self.connections = []
            self.timeStep = timeStep
            self.repelFactor = 10.0 ** 2.0
            self.attractFactor = 10.0 ** -4.0
            self.dampFactor = 0.5

    def addNode(self, xPos, yPos):
        self.nodes.append(node.Node(xPos, yPos))

    def connect(self, node1, node2):
        self.connections.append([node1, node2])
        node1.connect(node2)

    def calculateForce(self):

        i = 0
        for inode in self.nodes[:-1]:
            for jnode in self.nodes[i+1:]:

                deltaPos = tdvector.difference(jnode.position, inode.position)
                theta = deltaPos.angle()
                forceMag = (self.repelFactor / deltaPos.magnitudeSquare())
                forceVec = tdvector.TDVector(forceMag * math.cos(theta),
                                             forceMag * math.sin(theta))
                inode.forceSum.subtract(forceVec)
                jnode.forceSum.add(forceVec)
            i += 1

        for connection in self.connections:
            deltaPos = tdvector.difference(connection[1].position, connection[0].position)
            theta = deltaPos.angle()
            forceMag = deltaPos.magnitude() * self.attractFactor
            forceVec = tdvector.TDVector(forceMag * math.cos(theta),
                                         forceMag * math.sin(theta))

            connection[0].forceSum.add(forceVec)
            connection[1].forceSum.subtract(forceVec)


    def move(self):
        for node in self.nodes:
            node.velocity.add(node.forceSum.scale(self.timeStep))
            node.dampVelocity(self.dampFactor)
            node.position.add(node.velocity.scale(self.timeStep))
            node.zero()

