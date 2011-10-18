QT += core gui opengl

TARGET = visigoth
TEMPLATE = app

SOURCES += main.cpp \
          mainwindow.cpp \
          node.cpp \
    graphwidget.cpp \
    edge.cpp \
    randomgenerator.cpp \
    francescogenerator.cpp \
    treecode.cpp \
    treenode.cpp

HEADERS += mainwindow.h \
           node.h \
    graphwidget.h \
    edge.h \
    graphgenerator.h \
    randomgenerator.h \
    francescogenerator.h \
    treenode.h

FORMS += mainwindow.ui
