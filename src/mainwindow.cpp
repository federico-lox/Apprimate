#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
	arguments = QCoreApplication::arguments();
	webView = new QWebView(this);
	int argLength = arguments.size();

	//make the background color of the view match the Desktop environment's default one
	webView->setStyleSheet("background-color: " + this->palette().window().color().name());
	//disable scrollbars
	//TODO: make this optional via config and arguments
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);

	if(argLength > 0)
	{
		QByteArray data = QtJson::Json::serialize(arguments);

		//DEBUG
		qDebug() << "Running with arguments: " << data;

		webView
			->page()
			->mainFrame()
			->evaluateJavaScript(
				"window.arguments = "\
				//escape quotes for letting JSON.parse do its' job
				"JSON.parse(\"" + data.replace("\"", "\\\"") + "\");"
			 );

		if(arguments.size() > 1)
		{
			QString param = arguments.at(1);
			QUrl url;

			//QUrl doesn't handle relative file paths gracefully
			//let's give it a hand with that
			QFileInfo* fInfo = new QFileInfo(param);

			if(fInfo->exists() && fInfo->isRelative())
			{
				param = fInfo->absoluteFilePath();
			}

			delete fInfo;

			url = QUrl::fromUserInput(param);

			if(url.isValid())
			{
				//DEBUG
				qDebug() << "Loading from: " << url.toString();

				webView->load(url);
			}
			else
			{
				//DEBUG
				qDebug() << "Loading with content: " << param;

				webView->setHtml(param);
			}
		}
		else
		{
			//DEBUG
			webView->setHtml("<script>document.write('Parameters: ' + window.arguments.join(','));</script>");
		}
	}

	this->setCentralWidget(webView);
}

MainWindow::~MainWindow(){
	delete webView;
}
