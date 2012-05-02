/**
 * A really simple commandline handler based on some silly assumptions
 * but it does the job ;)
 */
#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <stdexcept>
#include <QCoreApplication>
#include <QDebug>
#include <QHash>
#include <QString>
#include <QStringRef>
#include <QStringList>
#include <QVariant>

typedef QHash<QString, QVariant> Options;

class CommandLine
{
	public:
		CommandLine();
		~CommandLine();
		QVariant getOption(const QString &name, const bool &required = false) const;
		QVariant getOption(const QString &name, const QVariant& defaultValue) const;
		const Options& getOptions() const;
		QString getArgument(const int &index) const;
		const QStringList& getArguments() const;

	private:
		QStringList m_arguments;
		Options m_options;
};

class MissingRequiredOptionException : public std::runtime_error
{
	public:
		explicit MissingRequiredOptionException(std::string what);
		virtual ~MissingRequiredOptionException() throw() {}
};

#endif // COMMANDLINE_H
