#include "mac.h"

NSString *string = [NSString string];
// this selector is available only on OSX >= 10.7
bool canHaveFullscreenTrigger = [string respondsToSelector:@selector(linguisticTagsInRange:scheme:options:orthography:tokenRanges:)];

void Mac::addFullscreenSwitch(MainWindow* window)
{
	//runtime check is needed since the app could run on OSX < 10.7
	if (canHaveFullscreenTrigger)
	{
		NSView *nsview = (NSView *) window->winId();
		NSWindow *nswindow = [nsview window];
		[nswindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
	}
}

void Mac::toggleFullscreen(MainWindow *window)
{
	//runtime check is needed since the app could run on OSX < 10.7
	if (canHaveFullscreenTrigger)
	{
		NSView *nsview = (NSView *) window->winId();
		NSWindow *nswindow = [nsview window];
		[nswindow toggleFullScreen:nil];
	}
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
	{
		NSView *nsview = (NSView *) window->winId();
		NSWindow *nswindow = [nsview window];
		NSUInteger sm = [nswindow styleMask];

		return ((sm & NSFullScreenWindowMask) != 0);
	}
	else
		return window->isFullScreen();
}
