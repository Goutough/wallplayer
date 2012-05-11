TEMPLATE = lib
DESTDIR = .
TARGET = qmpwidget

QT += network 
CONFIG += staticlib

# Optional features
QT += opengl
CONFIG += pipemode

HEADERS += \
	qmpwidget.h

SOURCES += \
	qmpwidget.cpp

!win32:pipemode: {
DEFINES += QMP_USE_YUVPIPE
HEADERS += qmpyuvreader.h
}
