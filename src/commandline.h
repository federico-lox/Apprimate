/**
 * A really simple commandline handler based on some silly assumptions
 * but it does the job ;)
 */
#ifndef COMMANDLINE_H
#define COMMANDLINE_H

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
		const QVariant& getOption(const QString &name) const;
		const QVariant& getOption(const QString &name, const QVariant& defaultValue) const;
		const Options& getOptions() const;
		const QString& getArgument(const int &index) const;
		const QStringList& getArguments() const;

	private:
		QStringList m_arguments;
		Options m_options;
};

#endif // COMMANDLINE_H
