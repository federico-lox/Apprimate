#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QByteArray>
#include <QCoreApplication>
#include <QStringList>
#include <QString>
#include <QResource>
#include <QtGui/QMainWindow>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>
#include "json.h"

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
