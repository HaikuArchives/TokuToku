/*
 * Copyright 2011 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Łukasz 'sil2100' Zemczak, sil2100@vexillium.org
 */

#include "ContactList.h"
#include "Msg.h"

#include <Looper.h>

ContactList::ContactList(BRect frame, const char* name, 
				list_view_type type,
				uint32 resizingMode,
				uint32 flags)
	:
	BListView(frame, name, type, resizingMode, flags)
{
}

void
ContactList::MouseDown(BPoint point)
{
	BMessage *message = Looper()->CurrentMessage();
	int32 index = IndexOf(point);
	BLooper *loop;

	if (index < 0 || (message->FindInt32("buttons") != B_SECONDARY_MOUSE_BUTTON)) {
		BListView::MouseDown(point);
		return;
	}

	loop = Looper();
	if (loop == NULL) {
		BListView::MouseDown(point);
		return;
	}

	Select(index);

	BMessage *click = new BMessage(CONTACT_RIGHTCLICK);
	click->AddPoint("where", point);
	loop->PostMessage(click);
	delete click;
}
