#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class MainWindow;

#include "lib/qt-json/json.h"
#include "commandline.h"

#ifdef Q_WS_MACX
#include "mac.h"
#endif

#include <QDebug>
#include <QByteArray>
#include <QCoreApplication>
#include <QFileInfo>
#include <QMap>
#include <QResource>
#include <QString>
#include <QStringList>
#include <QtGui/QMainWindow>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>

typedef QMap<QString, QVariant> Configuration;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();

	private:
		static const int DEFAULT_WIDTH;
		static const int DEFAULT_HEIGHT;
		static const bool DEFAULT_ALLOW_FULLSCREEN;
		static const char* CONF_OPTION_NAME;
		static const char* CONF_WINDOW_TITLE;
		static const char* CONF_WINDOW_MIN_HEIGHT;
		static const char* CONF_WINDOW_MIN_WIDTH;
		static const char* CONF_WINDOW_HEIGHT;
		static const char* CONF_WINDOW_WIDTH;
		static const char* CONF_ALLOW_FULLSCREEN;
		CommandLine* cli;
		//TODO: should probably be static, so as to share it among different windows
		Configuration conf;
		QWebView* webView;
		void allowFullscreen();
};

#endif // MAINWINDOW_H
