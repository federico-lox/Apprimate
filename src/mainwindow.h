#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "json.h"
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
		QWebView* webView;
		QStringList arguments;
};

#endif // MAINWINDOW_H
