#include "mac.h"

inline NSWindow* getNativeWindow(MainWindow* window)
{
	NSView *nsview = (NSView *) window->winId();
	return [nsview window];
}

NSString *string = [NSString string];
// this selector is available only on OSX >= 10.7
bool canHaveFullscreenTrigger = [string respondsToSelector:@selector(linguisticTagsInRange:scheme:options:orthography:tokenRanges:)];

void Mac::addFullscreenSwitch(MainWindow* window)
{
	//runtime check is needed since the app could run on OSX < 10.7
	if (canHaveFullscreenTrigger)
		[getNativeWindow(window) setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
}

void Mac::toggleFullscreen(MainWindow *window)
{
	//runtime check is needed since the app could run on OSX < 10.7
	if (canHaveFullscreenTrigger)
		[getNativeWindow(window) toggleFullScreen:nil];
	else
	{
		if(window->isFullScreen())
			window->showNormal();
		else
			window->showFullScreen();
	}
}

bool Mac::isFullScreen(MainWindow *window)
{
	//runtime check is needed since the app could run on OSX < 10.7
	if (canHaveFullscreenTrigger)
		return (([getNativeWindow(window) styleMask] & NSFullScreenWindowMask) != 0);
	else
		return window->isFullScreen();
}
