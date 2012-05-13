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
				m_options[QString(tokens[0])] = QVariant(tokens[1]);
				tokens.clear();
			}
			else
			{
				//--option
				m_options[strippedArg.toString()] = QVariant(true);
			}
		}
		else if(arg.startsWith('-'))
		{
			//-o
			QStringRef strippedArg = arg.rightRef(arg.size() - 1);
			m_options[strippedArg.toString()] = QVariant(true);
		}
		else
		{
			m_arguments.append(arg);
		}
	}
}

CommandLine::~CommandLine()
{
	m_arguments.clear();
	m_options.clear();
}

bool CommandLine::hasOption(const QString& name) const
{
	return m_options.contains(name);
}

QVariant CommandLine::getOption(const QString& name, const bool& required) const
{
	bool exists = hasOption(name);

	if(required == true && !exists)
		throw new MissingRequiredOptionException("Missing required option " + name.toStdString());

	QVariant ret = (exists) ? m_options[name] : QVariant();
	return ret;
}

QVariant CommandLine::getOption(const QString& name, const QVariant& defaultValue) const
{
	return (hasOption(name)) ? getOption(name) : defaultValue;
}

const Options& CommandLine::getOptions() const
{
	return m_options;
}

QString CommandLine::getArgument(const int& index) const
{
	return m_arguments.at(index);
}

const QStringList& CommandLine::getArguments() const
{
	return m_arguments;
}

MissingRequiredOptionException::MissingRequiredOptionException(std::string what)
	: std::runtime_error("Missing required option: " + what)
{
	//kill literal warning
	qCritical("%s", what.c_str());
}
