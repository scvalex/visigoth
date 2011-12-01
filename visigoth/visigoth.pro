QT += core gui opengl xml network

RESOURCES = resources.qrc \

CONFIG += oauth
oauth {
    CONFIG += link_pkgconfig
    PKGCONFIG += qoauth
    DEFINES += HAS_OAUTH
}

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
           barabasialbert.cpp \
           statistics.cpp \
           quadtree.cpp \
           erdosrenyi.cpp \
           vtools.cpp

HEADERS += mainwindow.h \
           node.h \
           edge.h \
           preferential.h \
           graphscene.h \
           glgraphwidget.h \
           glancillary.h \
           bipartite.h \
           algorithm.h \
           statistics.h \
           quadtree.h \
           barabasialbert.h \
           erdosrenyi.h \
           vtools.h

FORMS += mainwindow.ui \
         bipartitecontrol.ui \
         preferentialcontrol.ui \
         erdoscontrol.ui \
         barabasialbert.ui \
         helpWidget.ui \
         statistics.ui

test {
    CONFIG -= release
    TARGET = test
    QT += testlib
    SOURCES -= main.cpp
    SOURCES += testsimple.cpp
}

oauth {
    SOURCES += twitter.cpp
    HEADERS += twitter.h
    FORMS += twitauthdialog.ui \
             twittercontrol.ui
}

