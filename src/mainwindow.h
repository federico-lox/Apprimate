#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lib/qt-json/json.h"
#include "commandline.h"
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
		const char* CONF_OPTION_NAME;
		const char* CONF_WINDOW_TITLE;
		const char* CONF_WINDOW_MIN_HEIGHT;
		const char* CONF_WINDOW_MIN_WIDTH;
		const char* CONF_WINDOW_HEIGHT;
		const char* CONF_WINDOW_WIDTH;
		CommandLine* cli;
		Configuration conf;
		QWebView* webView;
};

#endif // MAINWINDOW_H
