#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
	this->cli = new CommandLine();
	this->webView = new QWebView(this);
	this->config = NULL;
	QVariant confFilePath = this->cli->getOption("conf");

	if(!confFilePath.isNull())
	{
		QString path = confFilePath.toString();
		QFile f(path);

		if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			qCritical() << "Could not open configuration file from " << path;
			exit(1);
		}

		QByteArray data = f.readAll();
		bool success;
		f.close();

		QVariant res = QtJson::Json::parse(QString(data), success);

		if(!success)
		{
			qCritical() << "Could not parse configuration file %s" << path;
			QCoreApplication::exit(1);
			return;
		}

		*(this->config) = res.toMap();
	}

	//TODO: make the following settings be accessible via config and arguments
	this->setMinimumSize(300, 300);
	this->resize(600, 400);
	this->setWindowTitle("MyApp");

	//make the background color of the view match the Desktop environment's default one
	this->webView->setStyleSheet("background-color: " + this->palette().window().color().name());
	//disable scrollbars
	//TODO: make this optional via config and arguments
	QWebPage* p = this->webView->page();
	p->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	p->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	p = NULL;

	QByteArray argsData = QtJson::Json::serialize(this->cli->getArguments());
	QByteArray optsData = QtJson::Json::serialize(this->cli->getOptions());

	this->webView
		->page()
		->mainFrame()
		->evaluateJavaScript(
			"window.arguments = "\
			//escape quotes for letting JSON.parse do its' job
			"JSON.parse(\"" + argsData.replace("\"", "\\\"") + "\");"\
			"window.options = "\
			"JSON.parse(\"" + optsData.replace("\"", "\\\"") + "\");"\
		 );

	argsData.clear();
	optsData.clear();

	if(this->cli->getArguments().size() > 1)
	{
		QString param = this->cli->getArgument(1);
		QUrl url;

		//QUrl doesn't handle relative file paths gracefully
		//let's give it a hand with that
		QFileInfo* fInfo = new QFileInfo(param);

		if(fInfo->exists() && fInfo->isRelative())
			param = fInfo->absoluteFilePath();

		delete fInfo;

		url = QUrl::fromUserInput(param);

		if(url.isValid())
			this->webView->load(url);
		else
			this->webView->setHtml(param);
	}
	else
	{
		//DEBUG
		this->webView->setHtml("<script>document.write('Parameters: ' + window.arguments.join(','));</script>");
	}

	this->setCentralWidget(this->webView);
}

MainWindow::~MainWindow(){
	if(this->config != NULL)
		delete this->config;

	delete this->webView;
	delete this->cli;
}
