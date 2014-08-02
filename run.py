#!/usr/bin/python2.7
import sys
import random
from PySide import QtGui
import network
import drawNetwork

def makeInitialNetwork():
    net = network.Network()

    for i in range(30):
        net.addNode(random.randint(-400, 400), random.randint(-400, 400))
    net.nodes[0].mainConnection = True

    #for i in range(len(net.nodes)):
    #    net.connect(net.nodes[i], net.nodes[(i + 1) % len(net.nodes)])

    ####################################################################
    #for node in net.nodes:
    #    connections = 1
    #    while len(node.connected) < connections:
    #        randNode = net.nodes[random.randint(0,len(net.nodes)-1)]
    #        if randNode not in node.connected and randNode != node:
    #            net.connect(randNode, node)

    #for node in net.nodes:
    #    if not node.mainConnection:
    #        newNode = net.nodes[random.randint(0,len(net.nodes)-1)]
    #        while not newNode.mainConnection:
    #            newNode = net.nodes[random.randint(0,len(net.nodes)-1)]
    #        net.connect(node, newNode)
    ####################################################################
    for index in range(len(net.nodes) - 1):
        for node in net.nodes[index + 1:]:
            net.connect(net.nodes[index], node)

    return net


# Starts the application and passes system information
app = QtGui.QApplication(sys.argv)
ex = drawNetwork.Example(makeInitialNetwork())

sys.exit(app.exec_())
