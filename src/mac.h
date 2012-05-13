#ifndef MAC_H
#define MAC_H

#include "mainwindow.h"

namespace Mac
{
	void addFullscreenSwitch(MainWindow* window);
	void toggleFullscreen(MainWindow* window);
	bool isFullScreen(MainWindow *window);
}
#endif // MAC_H