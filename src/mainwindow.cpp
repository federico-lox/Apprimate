#include "mainwindow.h"

//default values
const int MainWindow::DEFAULT_WIDTH = 640;
const int MainWindow::DEFAULT_HEIGHT = 480;
const bool MainWindow::DEFAULT_ALLOW_FULLSCREEN = false;

//available options
const char* MainWindow::CONF_OPTION_NAME = "conf";
const char* MainWindow::CONF_WINDOW_TITLE = "windowTitle";
const char* MainWindow::CONF_WINDOW_MIN_HEIGHT = "windowMinHeight";
const char* MainWindow::CONF_WINDOW_MIN_WIDTH = "windowMinWidth";
const char* MainWindow::CONF_WINDOW_HEIGHT = "windowHeight";
const char* MainWindow::CONF_WINDOW_WIDTH = "windowWidth";
const char* MainWindow::CONF_ALLOW_FULLSCREEN = "allowFullscreen";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){
	cli = new CommandLine();
	webView = new QWebView(this);
#ifdef QTMAC_H
	macNativeWindow = new Mac::NativeWindow(this);
#endif

	//default settings
	conf[CONF_WINDOW_TITLE] = "Apprimate";
	conf[CONF_WINDOW_MIN_WIDTH] = conf[CONF_WINDOW_WIDTH] = DEFAULT_WIDTH;
	conf[CONF_WINDOW_MIN_HEIGHT] = conf[CONF_WINDOW_HEIGHT] = DEFAULT_HEIGHT;
	conf[CONF_ALLOW_FULLSCREEN] = DEFAULT_ALLOW_FULLSCREEN;

	if(cli->hasOption(CONF_OPTION_NAME))
	{
		QString confFilePath = cli->getOption(CONF_OPTION_NAME).toString();

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
				conf[key] = tmp.value(key);
			}
		}

		confFilePath.clear();
	}

	//let commandline options override configuration file
	foreach(QString key, cli->getOptions().keys())
	{
		conf[key] = cli->getOption(key);
	}

	//process menues and actions after processing all the config settings
	createMenus();
	setMinimumSize(
		conf.value(CONF_WINDOW_MIN_WIDTH).toInt(),
		conf.value(CONF_WINDOW_MIN_HEIGHT).toInt()
	);
	resize(conf.value(CONF_WINDOW_WIDTH).toInt(), conf.value(CONF_WINDOW_HEIGHT).toInt());
	setWindowTitle(conf.value(CONF_WINDOW_TITLE).toString());

	if(conf.value(CONF_ALLOW_FULLSCREEN).toBool())
	{
		allowFullscreen();
	}

	//make the background color of the view match the Desktop environment's default one
	this->webView->setStyleSheet("background-color: " + this->palette().window().color().name());
	//disable scrollbars
	//TODO: make this optional via config and arguments
	QWebPage* p = webView->page();
	p->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
	p->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
	p = NULL;

	//allow the JS context to access the configuration
	webView	->page()
			->mainFrame()
			->evaluateJavaScript(
				"window.commandline = {"\
					//escape quotes for letting JSON.parse do its' job
					"arguments: JSON.parse(\"" +
						QtJson::Json::serialize(cli->getArguments()).replace("\"", "\\\"") + "\"),"\
					"options: JSON.parse(\"" +
						QtJson::Json::serialize(conf).replace("\"", "\\\"") + "\")"\
				"};"
			 );

	if(cli->getArguments().size() > 1)
	{
		QString param = cli->getArgument(1);
		QUrl url;

		//QUrl doesn't handle relative file paths gracefully
		//let's give it a hand with that
		QFileInfo* fInfo = new QFileInfo(param);

		if(fInfo->exists() && fInfo->isRelative())
			param = fInfo->absoluteFilePath();

		delete fInfo;

		url = QUrl::fromUserInput(param);

		if(url.isValid())
			webView->load(url);
		else
			webView->setHtml(param);
	}
	else
	{
		//DEBUG
		webView->setHtml(
			"<script>"\
				"document.write("\
					"'Arguments: ' + window.commandline.arguments.join(', ')"\
				");"\
			"</script>"
		);
	}

	setCentralWidget(webView);
}

void MainWindow::setFullScreen(bool checked)
{
#ifdef QTMAC_H
	//OSX >= 10.7 has a different native way to toggle fullscreen mode
	if(checked)
		macNativeWindow->showFullScreen();
	else
	{
		macNativeWindow->showNormal();
	}
#else
	if(checked)
		showFullScreen();
	else
	{
		showNormal();
	}
#endif
}

#ifdef QTMAC_H
void MainWindow::viewMenuAboutToShow()
{
	//OSX >= 10.7 has a different way to toggle fullscreen mode
	//and the view menu needs to be kept in sync
	fullScreenAction->blockSignals(true);//avoid signal loops
	fullScreenAction->setChecked(macNativeWindow->isFullScreen());
	fullScreenAction->blockSignals(false);//re-enable signals
}
#endif

void MainWindow::createMenus()
{
	if(conf.value(CONF_ALLOW_FULLSCREEN).toBool())
	{
		QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
		fullScreenAction = new QAction(tr("Fullscreen"), this);

		fullScreenAction->setCheckable(true);
		fullScreenAction->setVisible(conf.value(CONF_ALLOW_FULLSCREEN).toBool());

#ifdef QTMAC_H
		connect(viewMenu, SIGNAL(aboutToShow()), this, SLOT(viewMenuAboutToShow()));
#endif
		connect(fullScreenAction, SIGNAL(toggled(bool)), this, SLOT(setFullScreen(bool)));
		viewMenu->addAction(fullScreenAction);
		viewMenu = NULL;
	}
	else
	{
		fullScreenAction = NULL;
	}
}

void MainWindow::allowFullscreen()
{
#ifdef QTMAC_H
	macNativeWindow->enableFullScreen();
#endif

	//this function can be called more than once
	//avoid setting the property value if not needed
	if(fullScreenAction != NULL && !(fullScreenAction->isEnabled()))
		fullScreenAction->setVisible(true);
}

MainWindow::~MainWindow(){
	if(fullScreenAction != NULL)
		delete fullScreenAction;

#ifdef QTMAC_H
	delete macNativeWindow;
#endif
	delete webView;
	delete cli;
}
