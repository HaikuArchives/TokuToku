/*
 * Copyright 2011-2012 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Artur 'aljen' Wyszynski <harakash@gmail.com>
 *     Łukasz 'sil2100' Zemczak <sil2100@vexillium.org>
 */

#ifndef __BEGADU_CHATWINDOW_H__
#define __BEGADU_CHATWINDOW_H__

#include <Window.h>
#include <libgadu.h>

/* Forward references */
class Network;
class MainWindow;
class Person;
class BTextControl;
class BScrollView;
class BTextView;

class ChatWindow : public BWindow
{
public:
	ChatWindow(Network *aNetwork, MainWindow *aWindow, uin_t aWho);
	virtual void MessageReceived(BMessage *aMessage);
	virtual bool QuitRequested();
	//virtual void FrameResized( float, float );
	virtual void Show();
	virtual void WindowActivated(bool aActivated);

	Network			*iNetwork;
	MainWindow		*iWindow;
	uin_t				iWho;
	Person			*iPerson;

	BTextControl	*iSayControl;
	BScrollView		*iScrollView;
	BTextView		*iChat;
};

#endif // __BEGADU_CHATWINDOW_H__
