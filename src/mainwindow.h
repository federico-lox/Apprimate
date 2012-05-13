#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "lib/qt-json/json.h"
#include "commandline.h"

#ifdef Q_WS_MACX
#include "lib/qt-mac/mac.h"
#endif

#include <QAction>
#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QMap>
#include <QMenu>
#include <QMenuBar>
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
		QAction* fullScreenAction;
		CommandLine* cli;
		//TODO: should probably be static, so as to share it among different windows
		Configuration conf;
#ifdef Q_WS_MACX
		Mac::NativeWindow* macNativeWindow;
#endif
		QWebView* webView;
		void createMenus();
		void allowFullscreen();

	private slots:
		void setFullScreen(bool checked);
#ifdef Q_WS_MACX
		void viewMenuAboutToShow();
#endif
};

#endif // MAINWINDOW_H
