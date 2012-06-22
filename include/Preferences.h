/*
 * Copyright 2011-2012 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Artur 'aljen' Wyszynski <harakash@gmail.com>
 *     Łukasz 'sil2100' Zemczak <sil2100@vexillium.org>
 */

#ifndef __BEGADU_PREFERENCES_H__
#define __BEGADU_PREFERENCES_H__

#include <Window.h>
#include <Resources.h>

class Profile;
class BTextControl;
class BCheckBox;
class MainWindow;
class Network;
class Person;
class BBitmap;
class BitmapView;

class Preferences : public BWindow
{
	public:
		Preferences(Profile *aProfile, MainWindow *aWindow, BRect aRect, BResources *aRes);
		virtual void MessageReceived(BMessage *aMessage);
		virtual void Show();

	private:
		void LoadPreferences(void);

		Profile				*	iProfile;
		MainWindow			*	iWindow;
		BTextControl		*	iNumberControl;
		BTextControl		*	iPasswordControl;
		BCheckBox			*	iCheckRemember;
		BCheckBox			*	iCheckNotify;
		BCheckBox			*	iCheckRememberPassword;
};

#endif /* __BEGADU_PREFERENCES_H__ */
