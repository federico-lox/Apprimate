#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
	arguments = QCoreApplication::arguments();
	webView = new QWebView(this);
	//webView->load(QUrl("http://www.google.com"));
	QByteArray data = QtJson::Json::serialize(arguments);
	webView
		->page()
		->mainFrame()
		->evaluateJavaScript(
			"window.arguments = "\
			"JSON.parse(\"" + data.replace("\"", "\\\"") + "\");"
		 );
	webView->setHtml("<script>document.write(window.arguments[0]);</script>");
	this->setCentralWidget(webView);
}

MainWindow::~MainWindow(){
	delete webView;
}
