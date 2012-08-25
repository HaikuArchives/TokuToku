/*
 * Copyright 2012 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Łukasz 'sil2100' Zemczak, sil2100@vexillium.org
 */

#include "LoginPrompt.h"
#include "Person.h"
#include "Msg.h"

#include <stdio.h>
#include <stdlib.h>
#include <Window.h>
#include <View.h>
#include <TextControl.h>
#include <Button.h>
#include <Box.h>
#include <CheckBox.h>
#include <SpaceLayoutItem.h>
#include <GroupLayoutBuilder.h>

#define PASSWORDPROMPT_LOG			'pplo'
#define PASSWORDPROMPT_CANCEL		'ppca'
#define PASSWORDPROMPT_NAME			"Zaloguj się"

LoginPrompt::LoginPrompt(BMessenger *msgr)
	:
	BWindow(BRect(100, 100, 300, 250), PASSWORDPROMPT_NAME, 
		B_MODAL_WINDOW_LOOK, B_FLOATING_ALL_WINDOW_FEEL,
		B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS),
	fMessenger(msgr)
{
	BBox *boxControl = new BBox(B_PLAIN_BORDER, NULL);
	BBox *boxButtons = new BBox(B_PLAIN_BORDER, NULL);
	BGroupLayout *layControl = new BGroupLayout(B_VERTICAL, 5);
	BGroupLayout *layButtons = new BGroupLayout(B_HORIZONTAL, 5);
	boxControl->SetLayout(layControl);
	boxButtons->SetLayout(layButtons);

	fControlID = new BTextControl("fControlID", "Numer GG:", "", NULL);
	fControlPass = new BTextControl("fControlPass", "Hasło:", "", NULL);
	fControlPass->TextView()->HideTyping(true);

	BButton *buttonLogin = new BButton("login", "Zaloguj", new BMessage(PASSWORDPROMPT_LOG));
	BButton *buttonCancel = new BButton("cancel", "Anuluj", new BMessage(PASSWORDPROMPT_CANCEL));

	SetLayout(new BGroupLayout(B_HORIZONTAL));

	AddChild(BGroupLayoutBuilder(B_VERTICAL, 0)
		.SetInsets(5, 5, 5, 5)
		.Add(BGroupLayoutBuilder(layControl)
			.Add(fControlID)
			.Add(fControlPass)
		)

		.Add(BGroupLayoutBuilder(layButtons)
			.AddGlue()
			.AddGlue()
			.Add(buttonCancel)
			.Add(buttonLogin)

			.SetInsets(5, 5, 5, 5)
		)
	);
}

LoginPrompt::~LoginPrompt()
{
	delete fMessenger;
}

// XXX: Probably deprecated
LoginPrompt::ID *
LoginPrompt::Go(void)
{
	fSem = create_sem(0, "LoginPromptSem");
	if (fSem < B_OK)
		return NULL;

	Show();

	while (acquire_sem(fSem) == B_INTERRUPTED) { }

	ID *ret = new ID;
	ret->UIN = atoi(fControlID->Text());
	ret->password.SetTo(fControlPass->Text());

	if (Lock())
		Quit();

	return ret;
}

void
LoginPrompt::MessageReceived(BMessage *msg)
{
	switch (msg->what)
	{
		case PASSWORDPROMPT_LOG:
		{
			BMessage msg(TRY_LOGIN);
			msg.AddInt32("uin", atoi(fControlID->Text()));
			msg.AddString("password", fControlPass->Text());
			fMessenger->SendMessage(&msg);
			break;
		}

		case B_QUIT_REQUESTED:
		case PASSWORDPROMPT_CANCEL:
		{
			// Cancel, shut down the application
			fMessenger->SendMessage(B_QUIT_REQUESTED);
			break;
		}

		default:
			BWindow::MessageReceived(msg);
	}
}
