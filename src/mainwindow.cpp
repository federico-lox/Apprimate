#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
	cli = new CommandLine();
	webView = new QWebView(this);

	//TODO: make the following settings be accessible via config and arguments
	this->setMinimumSize(300, 300);
	this->resize(600, 400);
	this->setWindowTitle("MyApp");

	//make the background color of the view match the Desktop environment's default one
	webView->setStyleSheet("background-color: " + this->palette().window().color().name());
	//disable scrollbars
	//TODO: make this optional via config and arguments
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	webView->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);

	QByteArray argsData = QtJson::Json::serialize(cli->getArguments());
	QByteArray optsData = QtJson::Json::serialize(cli->getOptions());

	//DEBUG
	qDebug() << "Running with arguments: " << argsData;
	qDebug() << "options: " << optsData;
	qDebug() << cli->getOptions();

	webView
		->page()
		->mainFrame()
		->evaluateJavaScript(
			"window.arguments = "\
			//escape quotes for letting JSON.parse do its' job
			"JSON.parse(\"" + argsData.replace("\"", "\\\"") + "\");"\
			"window.options = "\
			"JSON.parse(\"" + optsData.replace("\"", "\\\"") + "\");"\
		 );

	if(cli->getArguments().size() > 1)
	{
		QString param = cli->getArgument(1);
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

	this->setCentralWidget(webView);
}

MainWindow::~MainWindow(){
	delete webView;
	delete cli;
}
