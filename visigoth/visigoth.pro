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
           algorithm.cpp \
           statistics.cpp \
           barabasialbert.cpp \
           quadtree.cpp \
    point.cpp

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
           statistics.h \
           barabasialbert.h \
           quadtree.h \
    point.h

FORMS += mainwindow.ui \
         bipartitecontrol.ui \
         preferentialcontrol.ui





