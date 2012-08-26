/**
 * @author Federico "Lox" Lucignano <http://plus.ly/federico.lox>
 * 
 * @see https://github.com/federico-lox/Apprimate
 */

#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
