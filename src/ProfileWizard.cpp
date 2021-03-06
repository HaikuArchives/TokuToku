#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <InterfaceKit.h>
#include <Screen.h>
#include <Application.h>
#include <Alert.h>
#include <String.h>
#include <SpaceLayoutItem.h>
#include <GroupLayoutBuilder.h>
#include <CardLayout.h>
#include <libgadu.h>

#include "Main.h"
#include "Msg.h"
#include "Person.h"
#include "ProfileWizard.h"
#include "GfxStuff.h"

#define PROFILWIZARD_RECT BRect(0,0,350,300)
#define PROFILWIZARD_NAME "Utwórz profil..."

ProfileWizard::ProfileWizard() 
	:
	BWindow(PROFILWIZARD_RECT, PROFILWIZARD_NAME, B_TITLED_WINDOW,
          B_NOT_ZOOMABLE | B_NOT_RESIZABLE),
	iPhase(0)
{
	iNext = new BButton("next", "Dalej", new BMessage(GO_NEXT));
	iNext->MakeDefault(true);
	iPrev = new BButton("prev", "Wróć", new BMessage(GO_PREV));
	iPrev->SetEnabled(false);
	iCancel = new BButton("cancel_pw", "Anuluj", new BMessage(B_QUIT_REQUESTED));

	BFont *font = new BFont(be_bold_font);

	BBox *boxOne = new BBox(B_PLAIN_BORDER, NULL);
	BTextView *tv = new BTextView("tv");
	tv->MakeEditable(false);
	tv->MakeSelectable(false);
	tv->SetStylable(true);
	tv->SetFontAndColor(font);
	tv->Insert("Witaj w kreatorze profili\n");
	font->SetFace(B_REGULAR_FACE);
	tv->SetFontAndColor(font);
	tv->Insert( "\n\nTen kreator pomoże Ci skonfigurować swoje istniejące konto "
				"na serwerze Gadu-Gadu, lub założyć nowe.\n\n"
				"Aby kontynuować, wciśnij " );
	font->SetFace(B_BOLD_FACE);
	tv->SetFontAndColor(font);
	tv->Insert("Dalej.");

	BBox *boxTwo = new BBox(B_PLAIN_BORDER, NULL);
	BStringView *stringWhat = new BStringView("stringWhatDo", "Co chcesz zrobić?");
	BRadioButton *radioOne = new BRadioButton("radioOne", "Skonfigurować istniejące konto", new BMessage());
	BRadioButton *radioTwo = new BRadioButton("radioTwo", "Założyć nowe konto", new BMessage());
	radioTwo->SetEnabled(false);
	radioOne->SetValue(1);

	BBox *boxThree = new BBox(B_PLAIN_BORDER, NULL);
	BStringView *stringWho = new BStringView("stringWho", "Podaj dane konta");
	iName = new BTextControl("iName", "Nazwa profilu", "", NULL);
	iNumber = new BTextControl("iNumber", "Numer GG", "", NULL);
	iPassword = new BTextControl("iPassword", "Hasło", "", NULL);
	iPassword->TextView()->HideTyping(true);
	iName->TextView()->MakeFocus(true);


	// Sub-layouts
	boxOne->SetLayout(new BGroupLayout(B_HORIZONTAL));
	boxOne->AddChild(tv);

	boxTwo->SetLayout(new BGroupLayout(B_HORIZONTAL));
	boxTwo->AddChild(BGroupLayoutBuilder(B_VERTICAL, 0)
		.AddGlue()
		.Add(stringWhat)
		.Add(BSpaceLayoutItem::CreateVerticalStrut(5))
		.Add(radioOne)
		.Add(radioTwo)
		.AddGlue()
		.SetInsets(15, 5, 15, 5)
	);

	boxThree->SetLayout(new BGroupLayout(B_HORIZONTAL));
	boxThree->AddChild(BGroupLayoutBuilder(B_VERTICAL, 0)
		.AddGlue()
		.Add(stringWho)
		.Add(BSpaceLayoutItem::CreateVerticalStrut(5))
		.Add(iName)
		.Add(BSpaceLayoutItem::CreateVerticalStrut(5))
		.Add(iNumber)
		.Add(iPassword)
		.AddGlue()
		.SetInsets(15, 5, 15, 5)
	);

	iLayout = new BCardLayout();
	BView *cards = new BView("cardsView", 0, iLayout);
	iLayout->Owner()->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	iLayout->AddView(boxOne);
	iLayout->AddView(boxTwo);
	iLayout->AddView(boxThree);
	iLayout->SetVisibleItem((int32)0);

	// Main layout
	SetLayout(new BGroupLayout(B_HORIZONTAL));

	AddChild(BGroupLayoutBuilder(B_VERTICAL, 0)
		.Add(iLayout)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 0)
			.Add(iCancel)
			.AddGlue()
			.Add(iPrev)
			.Add(iNext)
		)
	);

	// Create the new profile
	iProfile = new Profile();

#if 0
	iBreak = false;
	iProfile = new Profile();
	BRect r = Bounds();
	BRect button = Bounds();
	button.top = button.bottom - 30;
	button.bottom = button.top + 25;
	button.left = button.left + 5;
	button.right = button.left + 60;

	iPage1 = new BView( r, "iPage1", B_FOLLOW_ALL, B_WILL_DRAW );
	iPage2 = new BView( r, "iPage2", B_FOLLOW_ALL, B_WILL_DRAW );
	iPage3 = new BView( r, "iPage3", B_FOLLOW_ALL, B_WILL_DRAW );
	iPage4 = new BView( r, "iPage4", B_FOLLOW_ALL, B_WILL_DRAW );
	iPage1->SetViewColor( 100, 100, 100 );
	iPage2->SetViewColor( 100, 100, 100 );
	iPage3->SetViewColor( 100, 100, 100 );
	iPage4->SetViewColor( 100, 100, 100 );
	
	r = iPage1->Bounds();
	r.left = r.left + 10;
	r.right = r.right - 10;
	r.top = r.top + 10;
	r.bottom = r.bottom - 90;
	iPBox1 = new BBox( r, "iPBox1" );
	iPBox2 = new BBox( r, "iPBox2" );
	iPBox3 = new BBox( r, "iPBox3" );
	iPBox4 = new BBox( r, "iPBox4" );
	iPBox2->SetViewColor( 120, 120, 120 );
	
	BFont *font = new BFont( be_plain_font );
	font->SetEncoding( B_ISO_8859_2 );
	font->SetSize( 15.0 );

	r = iPBox1->Bounds();
	r.top = r.top + 5;
	r.left = r.left + 5;
	r.bottom = r.bottom - 5;
	r.right = r.right - 5;
	BTextView *tv = new BTextView( r, "tv", r, B_FOLLOW_ALL, B_WILL_DRAW );
	tv->MakeEditable( false );
	tv->MakeSelectable( false );
	tv->SetStylable( true );
	font = new BFont( be_bold_font );
	font->SetEncoding( B_ISO_8859_2 );
	font->SetSize( 18.0 );
	rgb_color white = { 255, 255, 255 };
	tv->SetFontAndColor( font, B_FONT_ALL, &white );
	tv->Insert( "Witaj w kreatorze profili\n" );
	font->SetFace( B_REGULAR_FACE );
	font->SetSize( 15.0 );
	tv->SetFontAndColor( font );
	tv->Insert( "\n\nTen kreator pomoże Ci skonfigurować swoje istniejące konto "
				"na serwerze Gadu-Gadu, lub założyć nowe.\n\n"
				"Aby kontynuować, wciśnij " );
	font->SetFace( B_BOLD_FACE );
	tv->SetFontAndColor( font );
	tv->Insert( "Dalej." );
	tv->SetViewColor( 120, 120, 120 );
	iPBox1->AddChild( tv );
	iPage1->AddChild( iPBox1 );
	r = Bounds();
	iCancel1 = new BButton( button, "iCancel1", "Cancel", new BMessage( B_QUIT_REQUESTED ) );
	iPage1->AddChild( iCancel1 );
	button.OffsetBy( 200, 0 );
	iBack1 = new BButton( button, "iBack1", "Back", new BMessage( GO_BACK1 ) );
	iBack1->SetEnabled( false );
	iPage1->AddChild( iBack1 );
	button.OffsetBy( 75, 0 );
	iNext1 = new BButton( button, "iNext1", "Next", new BMessage( GO_NEXT1 ) );
	iNext1->MakeDefault( true );
	iPage1->AddChild( iNext1 );
	AddChild( iPage1 );

	r = iPBox2->Bounds();
	BStringView *sv = new BStringView(	BRect( r.left + 10, r.top + 10, r.right - 10, r.top + 30 ),
										"tc",
										"",
										B_FOLLOW_ALL, B_WILL_DRAW );
	font = new BFont( be_bold_font );
	font->SetEncoding(B_ISO_8859_2);
	font->SetSize( 18.0 );
	sv->SetFont( font );
	sv->SetText( "Co chcesz zrobić ?" );
	sv->SetHighColor( 255, 255, 255 );
	iPBox2->AddChild( sv );
	font->SetSize( 15.0 );
	font->SetFace( B_REGULAR_FACE );
	iHave = new BRadioButton( BRect( r.left + 10, r.top + 60, r.right - 10, r.top + 80 ),
							"iHave",
							"Skonfigurować istniejące konto", new BMessage() );
	iHave->SetValue( 1 );
	iHave->SetFont( font );
	iHave->SetHighColor( 255, 255, 255 );
	iNew = new BRadioButton( BRect( r.left + 10, r.top + 90, r.right - 10, r.top + 110 ),
							"iNew",
							"Założyć nowe konto (obecnie nie zaimplementowane)", new BMessage() );
	iNew->SetValue( 0 );
	iNew->SetFont( font );
	iNew->SetEnabled( false );
	iNew->SetHighColor( 255, 255, 255 );
	iPBox2->AddChild( iHave );
	iPBox2->AddChild( iNew );
	button = Bounds();
	button.top = button.bottom - 30;
	button.bottom = button.top + 25;
	button.left = button.left + 5;
	button.right = button.left + 60;
	iCancel2 = new BButton( button, "iCancel2", "Anuluj", new BMessage( GO_CANCEL ) );
	iPage2->AddChild( iCancel2 );
	button.OffsetBy( 200, 0 );
	iBack2 = new BButton( button, "iBack2", "Wróć", new BMessage( GO_BACK2 ) );
	iPage2->AddChild( iBack2 );
	button.OffsetBy( 75, 0 );
	iNext2 = new BButton( button, "iNext2", "Dalej", new BMessage( GO_NEXT2 ) );
	iPage2->AddChild( iNext2 );
	iPage2->AddChild( iPBox2 );
	AddChild( iPage2 );

	/* strona trzecia */
	r = iPBox3->Bounds();
	r.left = 65;
	r.top = 75;
	r.right = r.left + 220;
	r.bottom = r.top + 25;
	// nazwa profilu
	iName = new BTextControl( r, "iName", "Nazwa profilu: ", "Nowy", NULL );
	iName->SetFont( font );
	iName->SetHighColor( 0, 0, 0 );
	r.top = r.top + 30;
	r.bottom = r.top + 25;
	iNumber = new BTextControl( r, "iNumber", "Numer: ", "0", NULL );
	iNumber->SetFont( font );
	iNumber->SetHighColor( 0, 0, 0 );
	r.top = r.top + 30;
	r.bottom = r.top + 25;
	iPassword = new BTextControl( r, "iPassword", "Haslo ", "", NULL );
	iPassword->TextView()->HideTyping( true );
	iPassword->SetFont( font );
	iPassword->SetHighColor( 0, 0, 0 );
	iPBox3->AddChild( iName );
	iPBox3->AddChild( iNumber );
	iPBox3->AddChild( iPassword );
	button = Bounds();
	button.top = button.bottom - 30;
	button.bottom = button.top + 25;
	button.left = button.left + 5;
	button.right = button.left + 60;
	iCancel3 = new BButton( button, "iCancel3", "Anuluj", new BMessage( GO_CANCEL ) );
	iPage3->AddChild( iCancel3 );
	button.OffsetBy( 200, 0 );
	iBack3 = new BButton( button, "iBack3", "Wróć", new BMessage( GO_BACK3 ) );
	iPage3->AddChild( iBack3 );
	button.OffsetBy( 75, 0 );
	iNext3 = new BButton( button, "iNext3", "Zakoncz", new BMessage( GO_NEXT3 ) );
	iPage3->AddChild( iNext3 );
	iPage3->AddChild( iPBox3 );
	AddChild( iPage3 );

	button = Bounds();
	button.top = button.bottom - 30;
	button.bottom = button.top + 25;
	button.left = button.left + 5;
	button.right = button.left + 60;
	iCancel4 = new BButton( button, "iCancel4", "Anuluj", new BMessage( GO_CANCEL ) );
	iPage4->AddChild( iCancel4 );
	button.OffsetBy( 200, 0 );
	iBack4 = new BButton( button, "iBack4", "Wróć", new BMessage( GO_BACK4 ) );
	iPage4->AddChild( iBack4 );
	button.OffsetBy( 75, 0 );
	iNext4 = new BButton( button, "iNext4", "Zakończ", new BMessage( GO_NEXT4 ) );
	iPage4->AddChild( iNext4 );
	iPage4->AddChild( iPBox4 );
	AddChild( iPage4 );
	iPage2->Hide();
	iPage3->Hide();
	iPage4->Hide();
	iNext1->MakeDefault( true );
#endif
}

ProfileWizard::~ProfileWizard()
{
/*
	delete iNext1;
	delete iNext2;
	delete iNext3;
	delete iNext4;
	delete iBack1;
	delete iBack2;
	delete iBack3;
	delete iBack4;
	delete iCancel1;
	delete iCancel2;
	delete iCancel3;
	delete iCancel4;
	delete iPage1;
	delete iPage2;
	delete iPage3;
	delete iPage4;
	delete iPBox1;
	delete iPBox2;
	delete iPBox3;
	delete iPBox4;
	delete iName;
	delete iPassword;
	delete iNumber;
*/
}

void ProfileWizard::MessageReceived(BMessage* aMessage)
{
	switch (aMessage->what)
	{
		case GO_NEXT:
		{
			iPhase++;
			if (iPhase < 3) {
				iLayout->SetVisibleItem(iPhase);
				if (iPhase == 1)
					iPrev->SetEnabled(true);
				else if (iPhase == 2)
					iNext->SetLabel("Zakończ");
			}
			else {
				// Finish
				// Code left intact
				if (iName->LockLooper()) {
					iProfile->ProfileName()->SetTo(iName->Text());
					iName->UnlockLooper();
				}
				if (iNumber->LockLooper()) {
					iProfile->SetUIN(atoi(iNumber->Text()));
					iNumber->UnlockLooper();
				}
				if (iPassword->LockLooper()) {
					iProfile->iPassword->SetTo(iPassword->Text()); // TODO FIXME Crashes when passwd empty
					iPassword->UnlockLooper();
				}
				iProfile->Save();
				BMessage *mesg = new BMessage(PROFILE_CREATED);
				mesg->AddString("ProfileName", *iProfile->ProfileName());
				BMessenger(be_app).SendMessage(mesg);
				delete mesg;

				if (Lock())
					Quit();
			}
			break;
		}

		case GO_PREV:
		{
			iPhase--;
			if (iPhase >= 0) {
				iLayout->SetVisibleItem(iPhase);
				if (iPhase == 0)
					iPrev->SetEnabled(false);
				else if (iPhase == 1)
					iNext->SetLabel("Dalej");
			}
			else {
				iPhase = 0;
			}
			break;
		}

#if 0
		case GO_NEXT1:
		{
			iPage1->Hide();
			iPage2->Show();
			iNext2->MakeDefault( true );
			break;
		}

		case GO_BACK1:
		{
			break;
		}

		case GO_NEXT2:
		{
			iPage2->Hide();
			iPage3->Show();
			iNext3->MakeDefault( true );
			break;
		}

		case GO_BACK2:
		{
			iPage2->Hide();
			iPage1->Show();
			break;
		}

		case GO_NEXT3:
		{
			if( iName->LockLooper() )
			{
				iProfile->ProfileName()->SetTo( iName->Text() );
				iName->UnlockLooper();
			}
			if( iNumber->LockLooper() )
			{
				iProfile->SetUIN( atoi( iNumber->Text() ) );
				iNumber->UnlockLooper();
			}
			if( iPassword->LockLooper() )
			{
				iProfile->iPassword->SetTo( iPassword->Text() );
				iPassword->UnlockLooper();
			}
			iProfile->Save();
			BMessage *mesg = new BMessage( PROFILE_CREATED );
			mesg->AddString( "ProfileName", *iProfile->ProfileName() );
			BMessenger( be_app ).SendMessage( mesg );
			delete mesg;
			if( Lock() )
			{
				Quit();
			}
			break;
		}

		case GO_BACK3:
		{
			iPage3->Hide();
			iPage2->Show();
			break;
		}

		case GO_NEXT4:
		{
			break;
		}

		case GO_BACK4:
		{
			iPage4->Hide();
			iPage3->Show();
			break;
		}
#endif

		default:
			BWindow::MessageReceived(aMessage);
			break;
	}
}

bool ProfileWizard::QuitRequested()
{
	bool ret = false;
	BAlert *alert = new BAlert("wizard",
	  "By aplikacja mogła działać poprawnie wymagane jest założenie nowego "
	  "profilu. Czy chcesz zakończyć działanie programu?",
	  "Tak", "Nie");

	if (!alert->Go()) {
		BMessenger(be_app).SendMessage(B_QUIT_REQUESTED);
		ret = true;
	}

	return ret;
}

void ProfileWizard::Show()
{
	BScreen *screen = new BScreen( this );
	display_mode mode;
	screen->GetMode( &mode );
	// teraz centrujemy okienko
	int32 szerokosc = ( int32 )( PROFILWIZARD_RECT.right - PROFILWIZARD_RECT.left );
	int32 wysokosc = ( int32 )( PROFILWIZARD_RECT.bottom - PROFILWIZARD_RECT.top ); 
	// obliczamy srodek ekranu /2  - 1/2 szerokosci okna
	int32 x_wind = mode.timing.h_display/2 - ( szerokosc / 2 );
	// obliczamy srodek ekranu /2 - 1/2 wysokosci okna
	int32 y_wind = mode.timing.v_display/2 - ( wysokosc / 2 );
	MoveTo( x_wind, y_wind );
	BWindow::Show();
}
