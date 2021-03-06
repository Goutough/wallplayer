TEMPLATE = app
TARGET = wallplayer
DESTDIR = .

QT += network
QT += opengl

CONFIG += qxt
QXT += core

CONFIG += pipemode

HEADERS += playerpanel.h controlpanel.h player.h qmpwidget.h minislider.h
SOURCES += qmpwidget.cpp main.cpp playerpanel.cpp controlpanel.cpp minislider.cpp

!win32:pipemode: {
DEFINES += QMP_USE_YUVPIPE
HEADERS += qmpyuvreader.h
}
