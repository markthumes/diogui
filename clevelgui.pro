TEMPLATE = app
TARGET = clevel

QT = core gui widgets network

INCLUDEPATH= hnet

SOURCES += main.cpp \
           layout.cpp \
           hnet/udp.cpp \
           hnet/host.cpp

HEADERS += layout.h \
           hnet/udp.h \
           hnet/host.h
