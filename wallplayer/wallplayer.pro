TEMPLATE = app
TARGET = wallplayer
DESTDIR = .

QT += network opengl

INCLUDEPATH += ../qmpwidget
QMAKE_LIBDIR += ../qmpwidget
LIBS += -lqmpwidget

SOURCES += main.cpp
