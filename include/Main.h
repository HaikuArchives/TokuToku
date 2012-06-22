/*
 * Copyright 2011 Łukasz 'sil2100' Zemczak. All rights reserved.
 *
 * Authors:
 *     Artur 'aljen' Wyszynski <harakash@gmail.com>
 *     Łukasz 'sil2100' Zemczak <sil2100@vexillium.org>
 */

#ifndef __BEGADU_MAINWINDOW_H__
#define __BEGADU_MAINWINDOW_H__

#include <Window.h>
#include <Resources.h>
#include <Messenger.h>

/* Forward references */
class Network;
class Preferences;
class List;
class BStringView;
class BMenuItem;
class BMenu;
class BBitmap;
class BListItem;
class BListView;
class Profile;
class BPopUpMenu;
class BMenuField;
class GaduListItem;
class GaduMenuItem;
class BScrollView;
class ContactList;
class Person;

class MainWindow : public BWindow {
public:
	MainWindow(Profile *aProfile);
	virtual bool QuitRequested();
	virtual void MessageReceived(BMessage *aMessage);

	static int SortUsers(const void *, const void *);

	void SetStatus(char *aStatus);
	void SetProfile(BString *aProfile);
	void SetMessenger(BMessenger &aMessenger);
	void LoadIcons();
	BBitmap *GetBitmap(const char *aName);
	Person *GetContactAt(int index);
	Profile *GetProfile() const;
	Network *GetNetwork() const;
	BListView *ListView() const;
	GaduListItem *ListItem() const;
	List *ListItems() const;
	void AboutRequested();

private:
	void ShowContactMenu(BPoint where);

	/* to gg */
	Profile				*	iProfile;
	Network				*	iNetwork;

	/* to interface */
	BResources				iResources;
	BView				*	iGaduView;
	BScrollView			*	iScrollView;
	ContactList			*	iListView;
	GaduListItem		*	iListItem;
	List				*	iListItems;
	BMenu				*	iSubMenu;
	BMenuItem			*	iProfileItem;

	BMenuItem			*	iListMenu;
	BMenuItem			*	iListImport;
	BMenuItem			*	iListExport;

	BMenuItem			*	iAddPerson;
	BMenuItem			*	iDelPerson;
	BMenuItem			*	iCatalog;
	BMenuItem			*	iAbout;
	BMenuItem			*	iPreferences;
	BView				*	iIconsView;

	BMenuField			*	iStatus;
	BPopUpMenu			*	iStatusMenu;
	GaduMenuItem		*	iAvail;
	GaduMenuItem		*	iBRB;
	GaduMenuItem		*	iInvisible;
	GaduMenuItem		*	iNotAvail;
	GaduMenuItem		*	iDescr;
	BBitmap				*	iIconAvail;
	BBitmap				*	iIconBRB;
	BBitmap				*	iIconInvisible;
	BBitmap				*	iIconNotAvail;
	BBitmap				*	iIconAvailDescr;
	BBitmap				*	iIconBRBDescr;
	BBitmap				*	iIconInvisibleDescr;
	BBitmap				*	iIconNotAvailDescr;
	BMessenger				iDeskbarMessenger;
};

#endif
