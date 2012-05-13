#-------------------------------------------------
#
# Project created by QtCreator 2012-04-28T20:38:06
#
#-------------------------------------------------

#Disable debug messages
#DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT

QT       += core gui webkit

TARGET    = apprimate
TEMPLATE  = app

SOURCES +=	main.cpp\
			mainwindow.cpp \
			commandline.cpp \
			lib/qt-json/json.cpp

HEADERS  += mainwindow.h \
			commandline.h \
			lib/qt-json/json.h

macx {
	TARGET = Apprimate

	#tell linker to load Cocoa Foundation for Obj-C code
	LIBS += -framework Foundation
	HEADERS += mac.h
	OBJECTIVE_SOURCES += mac.mm

	#avoid creating an app bundle on Mac OSX
	#CONFIG -= app_bundle
}

#eable console window and output (Windows only?)
#win32: CONFIG += console
