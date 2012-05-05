#include "commandline.h"

CommandLine::CommandLine()
{
	QStringList arguments = QCoreApplication::arguments();

	foreach (QString arg, arguments)
	{
		if(arg.startsWith("--"))
		{
			QStringRef strippedArg = arg.rightRef(arg.size() - 2);
			int equalsPos = strippedArg.indexOf('=');

			if(equalsPos != -1)
			{
				//--option=value
				QStringList tokens = strippedArg.toString().split('=');
				this->m_options[QString(tokens[0])] = QVariant(tokens[1]);
				tokens.clear();
			}
			else
			{
				//--option
				this->m_options[strippedArg.toString()] = QVariant(true);
			}
		}
		else if(arg.startsWith('-'))
		{
			//-o
			QStringRef strippedArg = arg.rightRef(arg.size() - 1);
			this->m_options[strippedArg.toString()] = QVariant(true);
		}
		else
		{
			this->m_arguments.append(arg);
		}
	}
}

CommandLine::~CommandLine()
{
	this->m_arguments.clear();
	this->m_options.clear();
}

bool CommandLine::hasOption(const QString& name) const
{
	return this->m_options.contains(name);
}

QVariant CommandLine::getOption(const QString& name, const bool& required) const
{
	bool exists = this->hasOption(name);

	if(required == true && !exists)
		throw new MissingRequiredOptionException("Missing required option " + name.toStdString());

	QVariant ret = (exists) ? this->m_options[name] : QVariant();
	return ret;
}

QVariant CommandLine::getOption(const QString& name, const QVariant& defaultValue) const
{
	return (this->hasOption(name)) ? this->getOption(name) : defaultValue;
}

const Options& CommandLine::getOptions() const
{
	return this->m_options;
}

QString CommandLine::getArgument(const int& index) const
{
	return this->m_arguments.at(index);
}

const QStringList& CommandLine::getArguments() const
{
	return this->m_arguments;
}

MissingRequiredOptionException::MissingRequiredOptionException(std::string what)
	: std::runtime_error("Missing required option: " + what)
{
	//kill literal warning
	qCritical("%s", what.c_str());
}
