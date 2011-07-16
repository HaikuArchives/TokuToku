/*
 * Copyright 2011 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Artur 'aljen' Wyszynski <harakash@gmail.com>
 *     Łukasz 'sil2100' Zemczak <sil2100@vexillium.org>
 */

#ifndef __BEGADU_GADULISTITEM_H__
#define __BEGADU_GADULISTITEM_H__

#include <ListItem.h>
#include <Resources.h>
#include <Person.h>

/* Forward references */
class BBitmap;
class BMessage;
class BRect;
class BFont;
class BString;

class GaduListItem : public BListItem {
public:
	GaduListItem(BString* aPerson, int aStatus, BString* aDescription, 
			BResources* aRes, Person *aPtr);
	~GaduListItem();
	virtual void DrawItem(BView* aOwner, BRect aFrame, bool aComplete = false);
	virtual void Update(BView* aOwner, const BFont* aFont);

	void SetIcon(int status);
	BBitmap *GetBitmap(const char* aName);
	Person *GetPerson(void);

	BString		*	iName;
	BString		*	iDescription;
	BFont		*	iNameFont;
	BFont		*	iDescriptionFont;
	int 			iStatus;
	BBitmap		*	iIcon;
	BResources	*	iResources;

private:
	Person		*	iPerson;
};

#endif
