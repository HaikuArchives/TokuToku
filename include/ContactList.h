/*
 * Copyright 2011 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Łukasz 'sil2100' Zemczak, sil2100@vexillium.org
 */

#ifndef __BEGADU_CONTACTLIST_H__
#define __BEGADU_CONTACTLIST_H__

#include <ListView.h>

class ContactList : public BListView {
public:
	ContactList(BRect frame, const char* name, 
				list_view_type type = B_SINGLE_SELECTION_LIST,
				uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP,
				uint32 flags = B_WILL_DRAW | B_NAVIGABLE | B_FRAME_EVENTS);
	virtual void MouseDown(BPoint point);
};

#endif
