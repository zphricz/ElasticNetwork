import sys
from PySide import QtGui, QtCore
from PySide.QtGui import *
import tdvector
import network

class Example(QtGui.QWidget):
    def __init__(self, net, scaleChoice = True):
        super(Example, self).__init__()
        self.net = net
        self.height = 800
        self.width = 800
        self.pause = False
        maxval = 0.0
        for i in self.net.nodes:
            if abs(i.position.xComp) > maxval:
                maxval = abs(i.position.xComp)
            if abs(i.position.yComp) > maxval:
                maxval = abs(i.position.yComp)
        self.scale = maxval / 300
        self.scaleChoice = scaleChoice
        self.interval = 1
        self.timer = QtCore.QBasicTimer()
        self.setGeometry(0, 0, self.width, self.height)
        self.setWindowTitle('Network')
        self.selection = 0
        self.show()
        self.timer.start(17, self)

    def paintEvent(self, event):
            painter = QtGui.QPainter()
            painter.begin(self)
            if self.scaleChoice:
                self.scaler()
            self.drawNetwork(painter)
            painter.end()

    def timerEvent(self, event):
        if not self.pause:
            for i in range(self.interval):
                self.net.calculateForce()
                self.net.move()
        self.update()

    def keyPressEvent(self, event):
        if event.key() == QtCore.Qt.Key_F2:
            self.net.timeStep *= 2.0
        elif event.key() == QtCore.Qt.Key_F1:
            self.net.timeStep /= 2.0
        elif event.key() == QtCore.Qt.Key_F4:
            if self.net.dampFactor < 0.5:
                self.net.dampFactor *= 2.0
            else:
                self.net.dampFactor += (1.0 - self.net.dampFactor) / 2.0
        elif event.key() == QtCore.Qt.Key_F3:
            self.net.dampFactor /= 2.0
        elif event.key() == QtCore.Qt.Key_F6:
            self.net.attractFactor *= 2.0
        elif event.key() == QtCore.Qt.Key_F5:
            self.net.attractFactor /= 2.0
        elif event.key() == QtCore.Qt.Key_F8:
            self.net.repelFactor *= 2.0
        elif event.key() == QtCore.Qt.Key_F7:
            self.net.repelFactor /= 2.0
        elif event.key() == QtCore.Qt.Key_Return:
            self.pause = not self.pause
        elif event.key() == QtCore.Qt.Key_Left:
            self.net.nodes[self.selection].position.xComp -= 20.0 * self.scale
        elif event.key() == QtCore.Qt.Key_Right:
            self.net.nodes[self.selection].position.xComp += 20.0 * self.scale
        elif event.key() == QtCore.Qt.Key_Up:
            self.net.nodes[self.selection].position.yComp += 20.0 * self.scale
        elif event.key() == QtCore.Qt.Key_Down:
            self.net.nodes[self.selection].position.yComp -= 20.0 * self.scale
        elif event.key() == QtCore.Qt.Key_Shift:
            self.selection = (self.selection + 1) % len(self.net.nodes)
        elif event.key() == QtCore.Qt.Key_Escape:
            self.close()
        elif event.key() == QtCore.Qt.Key_Delete:
            if len(self.net.nodes) == 1:
                self.close()
            node = self.net.nodes.pop(self.selection)
            for connection in reversed(self.net.connections):
                if node in connection:
                    self.net.connections.remove(connection)
            del(node)

    def scaler(self):
        maxval = 0
        for i in self.net.nodes:
            if abs(i.position.xComp) > maxval:
                maxval = abs(i.position.xComp)
            if abs(i.position.yComp) > maxval:
                maxval = abs(i.position.yComp)

        if maxval / self.scale > self.height/2 - 25 or maxval / self.scale < 125:
            self.scale = maxval / 300

    def drawNetwork(self, painter):
        painter.fillRect(0, 0, self.width, self.height, QtGui.QColor(0,0,0))
        painter.setPen(QColor(75, 75, 75))
        self.drawLines(painter)
        for i in self.net.nodes:
            x = i.position.xComp/self.scale
            y = i.position.yComp/self.scale
            if i != self.net.nodes[self.selection]:
                color = QtGui.QColor(255, 0, 0)
            else:
                color = QtGui.QColor(0, 255, 0)
            painter.fillRect(self.height / 2 + x - 4, self.height/2 - y - 4, 9, 9, color)

    def drawLines(self, painter):
        for connection in self.net.connections:
            painter.drawLine(self.height/2 + connection[0].position.xComp/self.scale,
                             self.height/2 - connection[0].position.yComp/self.scale,
                             self.height/2 + connection[1].position.xComp/self.scale,
                             self.height/2 - connection[1].position.yComp/self.scale)

