QT += core
QT -= gui

CONFIG += c++11

TARGET = rpi2-qt5-thread-tuning-mwe
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
HEADERS += config.h critical_task.h non_critical_task.h main_thread.h tuning.h
