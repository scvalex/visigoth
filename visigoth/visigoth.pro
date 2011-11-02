QT += core gui opengl

TARGET = visigoth
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           node.cpp \
           edge.cpp \
           preferential.cpp \
           bipartite.cpp \
           graphscene.cpp \
           glgraphwidget.cpp \
           glancillary.c \
           quadtree.cpp \
           algorithm.cpp

HEADERS += mainwindow.h \
           node.h \
           edge.h \
           preferential.h \
           graphscene.h \
           glgraphwidget.h \
           glancillary.h \
           bipartite.h \
           algorithm.h \
           abstractgraphwidget.h \
           quadtree.h

FORMS += mainwindow.ui \
         bipartitecontrol.ui \
    preferentialcontrol.ui

