/*
 * ============================================================================
 *  Nazwa    : TokuToku z TokuToku.h
 *  Projekt  : TokuToku
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Glowny modul
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_H__
#define __BEGADU_H__

#include <Application.h>
#include "LoginPrompt.h"

/* Forward classes */
class DebugWindow;
class MainWindow;
class Profile;

class TokuToku : public BApplication {
public:
	TokuToku();

	virtual bool QuitRequested();
	virtual void MessageReceived(BMessage *message);
	virtual void ReadyToRun();
	void AddDeskbarIcon();
	void DelDeskbarIcon();
	MainWindow* GetMainWindow() const;
	DebugWindow* GetDebugWindow() const;
	bool FirstRun();
	bool HideAtStart();
	void SetHideAtStart(bool hide);
	BString* LastProfile();

private:
	bool _TryConnecting(LoginPrompt::ID *id);
	void _ShowMainWindow(Profile *profile);

	MainWindow  	*	iWindow;
	DebugWindow 	*	iDebugWindow;
	bool				iFirstRun;
	bool				iHideAtStart;
	BString			*	iLastProfileName;
	Profile *iProfile;
	LoginPrompt *iPrompt;
};

#endif /* __BEGADU_H__ */
