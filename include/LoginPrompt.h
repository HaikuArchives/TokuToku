/*
 * Copyright 2012 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Łukasz 'sil2100' Zemczak <sil2100@vexillium.org>
 */


#ifndef __LOGINPROMPT_H__
#define __LOGINPROMPT_H__

#include <Window.h>

class LoginPrompt : public BWindow {
public:

	struct ID {
		uin_t UIN;
		BString *password;
	};

	LoginPrompt(Profile *profile, MainWindow *window);
	ID *Go(void);

	virtual void MessageReceived(BMessage *msg);

private:
	sem_id fSem;
};

#endif
