#ifndef MACSUPPORT_H
#define MACSUPPORT_H

#include "mainwindow.h"

class MacSupport
{
	public:
		static bool isLion();
		static void addFullscreen(MainWindow *window);
};

#endif // MACSUPPORT_H
