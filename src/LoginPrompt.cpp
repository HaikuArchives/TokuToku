/*
 * Copyright 2012 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Łukasz 'sil2100' Zemczak, sil2100@vexillium.org
 */

#include "LoginPrompt.h"

LoginPrompt::LoginPrompt(Profile *profile, MainWindow *window)
	:
	BWindow(BRect(0, 0, 400, 170), PREFERENCES_NAME, B_FLOATING_WINDOW,
		B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS),
	fProfile(profile),
	fWindow(window)
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

ID *
LoginPrompt::Go(void)
{
	fSem = create_sem(0, "LoginPromptSem");
	if (fSem < B_OK)
		return NULL;

	Show();

	while (acquire_sem(fSem) == B_INTERRUPTED) { }

	ID *ret = new ID;
	ret.UIN = atoi(fControlID->Text());
	ret.password.SetTo(fControlPass->Text());

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
			if (fSem >= B_OK) 
				delete_sem(fSem);
			fSem = -1;
			break;
		}

		case PASSWORDPROMPT_CANCEL:
		{
			if (fSem >= B_OK) 
				delete_sem(fSem);
			fSem = -1;
			break;
		}

		default:
			BWindow::MessageReceived(msg);
	}
}
