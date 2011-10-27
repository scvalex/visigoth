QT += core gui opengl

TARGET = visigoth
TEMPLATE = app

SOURCES += main.cpp \
          mainwindow.cpp \
          node.cpp \
          graphwidget.cpp \
          edge.cpp \
          preferential.cpp \
          bipartite.cpp \
          graphscene.cpp \
          glgraphwidget.cpp \
          glancillary.c \
          quadtree.cpp

HEADERS += mainwindow.h \
           node.h \
           graphwidget.h \
           edge.h \
           preferential.h \
           graphscene.h \
           glgraphwidget.h \
           glancillary.h \
           bipartite.h \
           algorithm.h \
           abstractgraphwidget.h \
           quadtree.h

FORMS += mainwindow.ui
