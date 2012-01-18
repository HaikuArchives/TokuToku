/*
 * ============================================================================
 *  Nazwa    : ProfilWizard z ProfilWizard.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Klasa kreatora profili
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_PROFILWIZARD_H__
#define __BEGADU_PROFILWIZARD_H__

#include <Window.h>

#include <libgadu.h>

/* zewnetrzne klasy, includowane w zrodle */
class BTextControl;
class BView;
class BButton;
class BRadioButton;
class BBox;
class BCardLayout;

/* Wiadomości od okna */
#define GO_NEXT 	'gnex'
#define GO_PREV 	'gpre'
/*
#define GO_NEXT2 	'gne2'
#define GO_BACK2 	'gba2'
#define GO_NEXT3 	'gne3'
#define GO_BACK3 	'gba3'
#define GO_NEXT4 	'gne4'
#define GO_BACK4 	'gba4'
#define GO_CANCEL	'goca'
*/

class ProfileWizard : public BWindow
{
	public:
		ProfileWizard();
		~ProfileWizard();
		virtual void MessageReceived( BMessage* aMessage );
		virtual void Show();
		virtual bool QuitRequested();
		bool iBreak;
		Profile* iProfile;
	
	private:
		BView			*iPage1;
		BView			*iPage2;
		BView			*iPage3;
		BView			*iPage4;
		BBox			*iPBox1;
		BBox			*iPBox2;
		BBox			*iPBox3;
		BBox			*iPBox4;
		int				iPhase;

		BRadioButton	*iHave;
		BRadioButton	*iNew;

		BTextControl	*iName;
		BTextControl	*iNumber;
		BTextControl	*iPassword;
		BTextControl	*iEmail;

		BButton			*iNext;
		BButton			*iPrev;
		BButton			*iCancel;

		BCardLayout	*iLayout;
};

#endif /* __BEGADU_PROFILWIZARD_H__ */
