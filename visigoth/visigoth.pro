QT += core gui opengl

TARGET = visigoth
TEMPLATE = app

SOURCES += main.cpp \
          mainwindow.cpp \
          node.cpp \
    graphwidget.cpp \
    edge.cpp

HEADERS += mainwindow.h \
           node.h \
    graphwidget.h \
    edge.h

FORMS += mainwindow.ui




