#include "mac.h"

namespace Mac
{
	NativeWindow::NativeWindow(QMainWindow* target)
	{
		mQtWindow = target;

		//getting a pointer to the native NSWindow
		NSView *nsview = (NSView *) target->winId();
		mNativeWindow = [nsview window];
		nsview = NULL;

		//check for OSX >= 10.7 support to native fullscreen mode
		//by querying a selector introduced in OSX 10.7
		NSString *str = [NSString string];
		mSupportsFullScreenPrimary = [str respondsToSelector:@selector(linguisticTagsInRange:scheme:options:orthography:tokenRanges:)];
		str = NULL;
	}

	void NativeWindow::enableFullScreen()
	{
		if(mSupportsFullScreenPrimary)
			[(id)mNativeWindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
	}

	bool NativeWindow::isFullScreen()
	{
		if (mSupportsFullScreenPrimary)
			return (([(id)mNativeWindow styleMask] & NSFullScreenWindowMask) != 0);
		else
			return mQtWindow->isFullScreen();
	}

	void NativeWindow::showFullScreen()
	{
		if(!isFullScreen())
		{
			if (mSupportsFullScreenPrimary)
				[(id)mNativeWindow toggleFullScreen:nil];
			else
				mQtWindow->showFullScreen();
		}
	}

	void NativeWindow::showNormal()
	{
		if(isFullScreen())
		{
			if (mSupportsFullScreenPrimary)
				[(id)mNativeWindow toggleFullScreen:nil];
			else
				mQtWindow->showNormal();
		}
	}

	NativeWindow::~NativeWindow()
	{
		mNativeWindow = NULL;
		mQtWindow = NULL;
	}
}
