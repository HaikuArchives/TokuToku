/*
 * Copyright 2012 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Łukasz 'sil2100' Zemczak <sil2100@vexillium.org>
 */

#ifndef __LOGINPROMPT_H__
#define __LOGINPROMPT_H__

#include <Window.h>
#include <String.h>

#include <libgadu.h>

class Profile;
class BTextControl;
class BMessenger;

class LoginPrompt : public BWindow {
public:

	struct ID {
		uin_t UIN;
		BString password;
	};

	LoginPrompt(BMessenger *msgr);
	~LoginPrompt();

	ID *Go(void); // XXX: Probably deprecated

	virtual void MessageReceived(BMessage *msg);

private:
	sem_id fSem; // XXX: Probably deprecated
	BTextControl *fControlID;
	BTextControl *fControlPass;
	BMessenger *fMessenger;
};

#endif
