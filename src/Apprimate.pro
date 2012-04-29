#-------------------------------------------------
#
# Project created by QtCreator 2012-04-28T20:38:06
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = apprimate
TEMPLATE = app


SOURCES += main.cpp\
			json.cpp\
			mainwindow.cpp

HEADERS  += mainwindow.h

#avoid creating an app bundle on Mac OSX
#CONFIG -= app_bundle
