/*
 * Copyright 2011 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Artur 'aljen' Wyszynski <harakash@gmail.com>
 *     Łukasz 'sil2100' Zemczak <sil2100@vexillium.org>
 */

#include <stdio.h>
#include <stdlib.h>
#include <View.h>
#include <String.h>
#include <StringView.h>
#include <TextControl.h>
#include <Button.h>
#include <CheckBox.h>
#include <Box.h>
#include <SpaceLayoutItem.h>
#include <GroupLayoutBuilder.h>

#include "Msg.h"
#include "Preferences.h"
#include "Main.h"
#include "GfxStuff.h"
#include "Network.h"
#include "Person.h"

#define PREFERENCES_NAME "Ustawienia"

Preferences::Preferences(Profile* aProfile, MainWindow* aWindow, BRect aRect, 
		BResources* aRes)
	: 
	BWindow(aRect, PREFERENCES_NAME, B_FLOATING_WINDOW, 
		B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS),
	iProfile(aProfile),
	iWindow(aWindow)
{
	BBox *boxPrefs = new BBox(B_PLAIN_BORDER, NULL);
	BBox *boxButtons = new BBox(B_PLAIN_BORDER, NULL);
	BGroupLayout *layPrefs = new BGroupLayout(B_VERTICAL, 5);
	BGroupLayout *layButtons = new BGroupLayout(B_HORIZONTAL, 5);
	boxPrefs->SetLayout(layPrefs);
	boxButtons->SetLayout(layButtons);

	BAlignment align(B_ALIGN_LEFT, B_ALIGN_VERTICAL_CENTER);
	BStringView *labelAccount = new BStringView("stringViewAccount", "Konto");
	labelAccount->SetExplicitAlignment(align);
	labelAccount->SetFont(be_bold_font);
	BStringView *labelMisc = new BStringView("stringViewMisc", "Pozostale");
	labelMisc->SetExplicitAlignment(align);
	labelMisc->SetFont(be_bold_font);

	iNumberControl = new BTextControl("iNumberControl", "Numer GG:", "", NULL);
	iPasswordControl = new BTextControl("iPasswordControl", "Haslo:", "", NULL);
	iPasswordControl->TextView()->HideTyping(true);

	BButton *buttonOk = new BButton("ok", "OK", new BMessage(PREFERENCES_OK));
	BButton *buttonCancel = new BButton("cancel", "Anuluj", 
		new BMessage(PREFERENCES_CANCEL));
	buttonOk->MakeDefault(true);

	iCheckRemember = new BCheckBox("iCheckRemember", 
		"Pamiętaj ostatnio użyty status", new BMessage(PREFERENCES_REMEMBER));
	BCheckBox *checkSound = new BCheckBox("checkSound",
		"Włącz dźwięki wiadomości", new BMessage(PREFERENCES_SOUND));
	checkSound->SetEnabled(false);

	SetLayout(new BGroupLayout(B_HORIZONTAL));

	AddChild(BGroupLayoutBuilder(B_VERTICAL, 0)
		.Add(BGroupLayoutBuilder(layPrefs)
			.Add(labelAccount)
			.Add(BGroupLayoutBuilder(B_HORIZONTAL, 0)
				.Add(BSpaceLayoutItem::CreateHorizontalStrut(10))
				.Add(BGroupLayoutBuilder(B_VERTICAL, 0)
					.Add(iNumberControl)
					.Add(iPasswordControl)
				)
			)
			.Add(BSpaceLayoutItem::CreateVerticalStrut(5))

			.Add(labelMisc)
			.Add(BGroupLayoutBuilder(B_HORIZONTAL, 0)
				.Add(BSpaceLayoutItem::CreateHorizontalStrut(10))
				.Add(BGroupLayoutBuilder(B_VERTICAL, 0)
					.Add(iCheckRemember)
					.Add(checkSound)
				)
			)
			.Add(BSpaceLayoutItem::CreateVerticalStrut(5))

			.SetInsets(5, 5, 15, 5)
		)
		.Add(BGroupLayoutBuilder(layButtons)
			.AddGlue()
			.AddGlue()
			.Add(buttonCancel)
			.Add(buttonOk)

			.SetInsets(5, 5, 5, 5)
		)
	);

	LoadPreferences();
}

void Preferences::LoadPreferences(void)
{
	if(iNumberControl->LockLooper())
	{
		BString a;
		a << (int32)iProfile->iNumber;
		iNumberControl->SetText(a.String());
		iPasswordControl->SetText(iProfile->iPassword->String());
		iCheckRemember->SetValue((iProfile->iRememberStatus) ? B_CONTROL_ON : B_CONTROL_OFF);
		fprintf(stderr, "numer: %s\nhaslo: %s\n", a.String(), iProfile->iPassword->String());
		iNumberControl->UnlockLooper();
	} 
}

void Preferences::MessageReceived(BMessage* aMessage)
{
	switch(aMessage->what)
	{
		case PREFERENCES_OK:
		{
			if (iNumberControl->LockLooper()) {
				iProfile->iNumber = atoi(iNumberControl->Text());
				iNumberControl->UnlockLooper();
			}
			if (iPasswordControl->LockLooper()) {
				iProfile->iPassword->SetTo(iPasswordControl->Text());
				iPasswordControl->UnlockLooper();
			}
			// Now, drop down in switch to close the window
		}

		case PREFERENCES_CANCEL:
		{
			BMessenger(this).SendMessage(B_QUIT_REQUESTED);
			break;
		}

		case PREFERENCES_REMEMBER:
		{
			if (iCheckRemember->Value() == B_CONTROL_OFF)
				iProfile->iRememberStatus = false;
			else 
				iProfile->iRememberStatus = true;
			break;
		}

		default:
			BWindow::MessageReceived(aMessage);
	}
}

void Preferences::Show()
{
	BWindow::Show();
}

