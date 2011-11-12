QT += core gui opengl xml

CONFIG += oauth

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
           twitter.cpp \
           barabasialbert.cpp \
           statistics.cpp \
           quadtree.cpp \
           erdosrenyi.cpp

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
           quadtree.h \
           twitter.h \
           barabasialbert.h \
           erdosrenyi.h

FORMS += mainwindow.ui \
         bipartitecontrol.ui \
         preferentialcontrol.ui \
         erdoscontrol.ui
