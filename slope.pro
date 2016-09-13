TEMPLATE = app
TARGET = slope_test
CONFIG -= qt
INCLUDEPATH += src
QMAKE_CC = gcc
QMAKE_CXX = gcc
CONFIG += link_pkgconfig
PKGCONFIG = gtk+-3.0

HEADERS += \
    src/slope/drawing.h \
    src/slope/global.h \
    src/slope/slope.h \
    src/slope/view.h \
    src/slope/mouse.h \
    src/slope/item.h \
    src/slope/item_p.h \
    src/slope/scale.h \
    src/slope/figure.h \
    src/slope/figure_p.h

SOURCES += \
    test.c \
    src/slope/drawing.c \
    src/slope/view.c \
    src/slope/item.c \
    src/slope/scale.c \
    src/slope/figure.c
