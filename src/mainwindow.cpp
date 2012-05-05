#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
	CONF_OPTION_NAME = "conf";
	CONF_WINDOW_TITLE = "windowTitle";
	CONF_WINDOW_MIN_HEIGHT = "windowMinHeight";
	CONF_WINDOW_MIN_WIDTH = "windowMinWidth";
	CONF_WINDOW_HEIGHT = "windowHeight";
	CONF_WINDOW_WIDTH = "windowWidth";

	this->cli = new CommandLine();
	this->webView = new QWebView(this);

	//default settings
	this->conf[CONF_WINDOW_TITLE] = "Apprimate";
	this->conf[CONF_WINDOW_MIN_HEIGHT] = 300;
	this->conf[CONF_WINDOW_MIN_WIDTH] = 300;
	this->conf[CONF_WINDOW_WIDTH] = 600;
	this->conf[CONF_WINDOW_HEIGHT] = 400;

	if(this->cli->hasOption(CONF_OPTION_NAME))
	{
		QString confFilePath = this->cli->getOption(CONF_OPTION_NAME).toString();

		if(!confFilePath.isEmpty() && !confFilePath.isNull())
		{
			QFile f(confFilePath);

			if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				qCritical() << "Could not open configuration file from " << confFilePath;
				exit(1);
			}

			QByteArray data = f.readAll();
			bool success;
			f.close();

			QVariant res = QtJson::Json::parse(data.constData(), success);
			data.clear();

			if(!success)
			{
				qCritical() << "Could not parse configuration file %s" << confFilePath;
				QCoreApplication::exit(1);
				return;
			}

			Configuration tmp = res.toMap();

			//let configuration file override default settings
			foreach(QString key, tmp.keys())
			{
				this->conf[key] = tmp.value(key);
			}
		}

		confFilePath.clear();
	}

	//let commandline options override configuration file
	foreach(QString key, this->cli->getOptions().keys())
	{
		this->conf[key] = this->cli->getOption(key);
	}

	//TODO: make the following settings be accessible via config and arguments
	this->setMinimumSize(
		this->conf.value(CONF_WINDOW_MIN_WIDTH).toInt(),
		this->conf.value(CONF_WINDOW_MIN_HEIGHT).toInt()
	);
	this->resize(this->conf.value(CONF_WINDOW_WIDTH).toInt(), this->conf.value(CONF_WINDOW_HEIGHT).toInt());
	this->setWindowTitle(this->conf.value(CONF_WINDOW_TITLE).toString());

	//make the background color of the view match the Desktop environment's default one
	this->webView->setStyleSheet("background-color: " + this->palette().window().color().name());
	//disable scrollbars
	//TODO: make this optional via config and arguments
	QWebPage* p = this->webView->page();
	p->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	p->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	p = NULL;

	this->webView
		->page()
		->mainFrame()
		->evaluateJavaScript(
			"window.commandline = {"\
				//escape quotes for letting JSON.parse do its' job
				"arguments: JSON.parse(\"" + QtJson::Json::serialize(this->cli->getArguments()).replace("\"", "\\\"") + "\"),"\
				"options: JSON.parse(\"" + QtJson::Json::serialize(this->conf).replace("\"", "\\\"") + "\")"\
			"};"
		 );

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
		this->webView->setHtml("<script>document.write('Arguments: ' + window.commandline.arguments.join(', '));</script>");
	}

	this->setCentralWidget(this->webView);
}

MainWindow::~MainWindow(){
	delete this->webView;
	delete this->cli;
}
