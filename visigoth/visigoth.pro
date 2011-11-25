QT += core gui opengl xml network

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
           barabasialbert.h \
           erdosrenyi.h

FORMS += mainwindow.ui \
         bipartitecontrol.ui \
         preferentialcontrol.ui \
         erdoscontrol.ui \
         statistics.ui

test {
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
