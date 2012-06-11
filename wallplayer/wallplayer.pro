TEMPLATE = app
TARGET = wallplayer
DESTDIR = .

QT += network opengl

INCLUDEPATH += ../qmpwidget
QMAKE_LIBDIR += ../qmpwidget
LIBS += -lqmpwidget

HEADERS += playerpanel.h controlpanel.h player.h
SOURCES += main.cpp playerpanel.cpp controlpanel.cpp
