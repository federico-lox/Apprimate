#include "commandline.h"

CommandLine::CommandLine()
{
	QStringList arguments = QCoreApplication::arguments();

	//DEBUG
	qDebug() << "Command input: " << arguments;

	foreach (QString arg, arguments)
	{
		qDebug() << "processing input" << arg;

		if(arg.startsWith("--"))
		{
			QStringRef strippedArg = arg.rightRef(arg.size() - 2);
			int equalsPos = strippedArg.indexOf('=');

			if(equalsPos != -1)
			{
				//--option=value
				QStringList tokens = strippedArg.toString().split('=');

				//DEBUG
				qDebug() << "--option=value" << tokens[0] << tokens[1];

				this->m_options[QString(tokens[0])] = QVariant(tokens[1]);
				tokens.clear();
			}
			else
			{
				//--option

				//DEBUG
				qDebug() << "--option" << strippedArg.toString();

				this->m_options[strippedArg.toString()] = QVariant(true);
			}
		}
		else if(arg.startsWith('-'))
		{
			//-o
			QStringRef strippedArg = arg.rightRef(arg.size() - 1);

			//DEBUG
			qDebug() << "-o" << strippedArg;

			this->m_options[strippedArg.toString()] = QVariant(true);
		}
		else
		{
			//DEBUG
			qDebug() << "argument:" << arg;

			this->m_arguments.append(arg);
		}
	}
}

CommandLine::~CommandLine()
{
	this->m_arguments.clear();
	this->m_options.clear();
}

const QVariant& CommandLine::getOption(const QString &name) const
{
	//let the temporary reference survive as long as needed
	const QVariant& ret = (this->m_options.contains(name)) ?
		this->m_options[name] :
		QVariant();

	return ret;
}

const QVariant& CommandLine::getOption(const QString &name, const QVariant& defaultValue) const
{
	//let the temporary reference survive as long as needed
	const QVariant& ret = (this->getOption(name).isNull()) ?
		defaultValue :
		this->getOption(name);

	return ret;
}

const Options& CommandLine::getOptions() const
{
	return this->m_options;
}

const QString& CommandLine::getArgument(const int &index) const
{
	//let the temporary reference survive as long as needed
	const QString& ret = this->m_arguments.at(index);
	return ret;
}

const QStringList& CommandLine::getArguments() const
{
	return this->m_arguments;
}
