#ifndef QTMAC_H
#define QTMAC_H

#include <QMainWindow>
#include <QObject>

namespace Mac
{
	class NativeWindow : public QObject
	{
		Q_OBJECT
		Q_CLASSINFO("Author", "Federico \"Lox\" Lucignano")
		Q_CLASSINFO("URL", "http://github.com/federico-lox/QtMac")
		Q_CLASSINFO("Version", "0.1")

		public:
			NativeWindow(QMainWindow* target);
			~NativeWindow();
			void enableFullScreen();
			bool isFullScreen();
			void showFullScreen();
			void showNormal();

		private:
			QMainWindow* mQtWindow;
			//no portable way to include NSWindow.h
			//so let's use a pointer to void and cast in place to id/NSWindow*
			void* mNativeWindow;
			bool mSupportsFullScreenPrimary;
	};
}
#endif // QTMAC_H
