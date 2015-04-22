#include <QMainWindow>
#include <AppKit/NSView.h>
#include <AppKit/NSWindow.h>
#include <AppKit/NSApplication.h>

void* setFullScreen(QMainWindow *mainWindow)
{
    NSView *nsview = (NSView *) mainWindow->winId();
    NSWindow *nswindow = [nsview window];
    [nswindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenPrimary];
}
