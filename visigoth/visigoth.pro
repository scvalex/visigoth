QT += core gui opengl

TARGET = visigoth
TEMPLATE = app

SOURCES += main.cpp \
          mainwindow.cpp \
          node.cpp \
          graphwidget.cpp \
          edge.cpp \
          preferential.cpp \
          treecode.cpp \
          treenode.cpp \
          graphscene.cpp \
          glgraphwidget.cpp \
          glancillary.c

HEADERS += mainwindow.h \
           node.h \
           graphwidget.h \
           edge.h \
           preferential.h \
           treenode.h \
           treecode.h \
           graphscene.h \
           glgraphwidget.h \
           glancillary.h

FORMS += mainwindow.ui
