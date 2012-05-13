#include "mac.h"

void Mac::addFullscreenSwitch(MainWindow* window)
{
	NSString *string = [NSString string];
	// this selector is available only on OSX >= 10.7
	bool canHaveFullscreenTrigger = [string respondsToSelector:@selector(linguisticTagsInRange:scheme:options:orthography:tokenRanges:)];

	//runtime check is needed since the
	if (canHaveFullscreenTrigger)
	{
		NSView *nsview = (NSView *) window->winId();
		NSWindow *nswindow = [nsview window];
		[nswindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
	}
}
