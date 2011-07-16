/*
 * Copyright 2011 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Łukasz 'sil2100' Zemczak <sil2100@vexillium.org>
 */

#include "StatusWindow.h"
#include "Msg.h"

#include <Box.h>
#include <Button.h>
#include <StringView.h>
#include <TextView.h>
#include <GridLayoutBuilder.h>
#include <GroupLayoutBuilder.h>

StatusWindow::StatusWindow()
	:
	BWindow(BRect(100, 100, 400, 300), "Informacje o kontakcie",
			B_FLOATING_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, 
			B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{
	BBox *boxStatus = new BBox(B_PLAIN_BORDER, NULL);
	BBox *boxButtons = new BBox(B_PLAIN_BORDER, NULL);
	BGridLayout *layStatus = new BGridLayout(5, 5);
	BGroupLayout *layButtons = new BGroupLayout(B_HORIZONTAL, 5);
	boxStatus->SetLayout(layStatus);
	boxButtons->SetLayout(layButtons);

	iRefreshButton = new BButton("refresh", "Odśwież", 
			new BMessage(STATUSWINDOW_REFRESH));
	iRefreshButton->SetEnabled(false);
	BButton *buttonOk = new BButton("ok", "OK", new BMessage(STATUSWINDOW_OK));

	BStringView *labelName = new BStringView("labelName", "Imię:");
	BStringView *labelSurname = new BStringView("labelSurname", "Nazwisko:");
	BStringView *labelAlias = new BStringView("labelAlias", "Alias:");
	BStringView *labelUIN = new BStringView("labelUIN", "Numer:");
	BStringView *labelDescr = new BStringView("labelDescr", "Opis:");

	iDescrView = new BTextView("textDescr");
	iNameString = new BStringView("stringName", "Alicja");
	iSurnameString = new BStringView("stringSurname", "Bałdys");
	iAliasString = new BStringView("stringAlias", "Test");
	iUINString = new BStringView("stringUIN", "41");

	SetLayout(new BGroupLayout(B_HORIZONTAL));

	AddChild(BGroupLayoutBuilder(B_VERTICAL, 0)
		.Add(BGridLayoutBuilder(layStatus)
			.Add(labelName, 0, 0)
			.Add(iNameString, 1, 0)
			.Add(labelSurname, 0, 1)
			.Add(iSurnameString, 1, 1) // XXX
		)
		.Add(BGroupLayoutBuilder(layButtons)
			.AddGlue()
			.Add(iRefreshButton)
			.Add(buttonOk)
			.SetInsets(5, 5, 5, 5)
		)
	);

	// TODO
}
