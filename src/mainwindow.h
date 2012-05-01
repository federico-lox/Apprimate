#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lib/qt-json/json.h"
#include "commandline.h"
#include <QDebug>
#include <QByteArray>
#include <QCoreApplication>
#include <QFileInfo>
#include <QResource>
#include <QString>
#include <QStringList>
#include <QtGui/QMainWindow>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();

	private:
		CommandLine* cli;
		QWebView* webView;
};

#endif // MAINWINDOW_H
