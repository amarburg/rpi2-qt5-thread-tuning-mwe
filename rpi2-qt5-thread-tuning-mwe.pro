QT += core
QT -= gui

CONFIG += c++11

TARGET = rpi2-qt5-thread-tuning-mwe
CONFIG += console release thread

TEMPLATE = app

LIBS += -lrt

SOURCES += main.cpp
HEADERS += config.h critical_task.h non_critical_task.h main_thread.h tuning.h
