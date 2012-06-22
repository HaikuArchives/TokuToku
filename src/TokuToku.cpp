/*
	TokuToku.cpp - Client G*adu-G*adu for BeOS
	Code: aljen <aljen@gumisie.org>
	Homepage: http://gadu.beos.pl
*/
#include <stdio.h>

#include <Deskbar.h>
#include <Path.h>
#include <Roster.h>
#include <StorageKit.h>
#include <File.h>
#include <Screen.h>
#include <String.h>
#include <Alert.h>

#include "Msg.h"
#include "TokuToku.h"
#include "Network.h"
#include "Main.h"
#include "Person.h"
#include "Debug.h"
#include "Preferences.h"
#include "ProfileWizard.h"
#include "LoginPrompt.h"

TokuToku::TokuToku() : BApplication( "application/x-vnd.TokuToku" )
	{
	/* we're checking configuration */
	iFirstRun = false;
	iHideAtStart = false;
	iLastProfileName = new BString();
	iLastProfileName->SetTo( "" );
	BPath path;
	BEntry entry;
	status_t error;
	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	BDirectory * bg_conf = new BDirectory( path.Path() );
	
	if( bg_conf->FindEntry( "TokuToku", &entry ) != B_OK )
		{
		path.Append( "TokuToku" );
		bg_conf->CreateDirectory( path.Path(), bg_conf );
		}
	
	if( bg_conf->FindEntry( "Profiles", &entry ) != B_OK )
		{
		find_directory( B_USER_SETTINGS_DIRECTORY, &path );
		path.Append( "TokuToku/Profiles" );
		error = bg_conf->CreateDirectory( path.Path(), bg_conf );
		iFirstRun = true;
		}

	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	path.Append( "TokuToku/Profiles" );
	bg_conf->SetTo( path.Path() );
	
	if( bg_conf->CountEntries() == 0 )
		{
		iFirstRun = true;
		}
	else
		{
		iFirstRun = false;	
		}
	
	/* loading configuration */
	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	path.Append( "TokuToku/Config" );
	BFile file( path.Path(), B_READ_ONLY );
	fprintf( stderr, "Loading configuration from %s\n", path.Path() );
	BMessage *cfgmesg = new BMessage();
	if( file.InitCheck() == B_OK )
		{
		cfgmesg->Unflatten( &file );
		file.Unset();
		}
	if( cfgmesg->FindBool( "iFirstRun", &iFirstRun ) != B_OK )
		{
		iFirstRun =  true;
		}
	if( cfgmesg->FindString( "iLastProfileName", iLastProfileName ) != B_OK )
		{
		iLastProfileName->SetTo( "" );
		}
		
	delete cfgmesg;
	delete bg_conf;
	
	if( iFirstRun )
		{
		fprintf( stderr, "First run, starting Profile Wizard...\n" );
		BMessenger( this ).SendMessage( new BMessage( OPEN_PROFILE_WIZARD ) );
		}
	else
		{
		fprintf( stderr, "Configuration ok.\n" );
		BMessenger( this ).SendMessage( new BMessage( CONFIG_OK ) );
		}
	}

void TokuToku::MessageReceived( BMessage *aMessage )
{
	switch (aMessage->what)
	{
		/* sending mesgs from libgadu to network */
		case GOT_MESSAGE:
		case ADD_HANDLER:
		case DEL_HANDLER:
		{
			BMessenger(iWindow->GetNetwork()).SendMessage(aMessage);
			break;
		}
		
		case ADD_MESSENGER:
		{
			fprintf(stderr, "TokuToku::MessageReceived( ADD_MESSENGER )\n");
			BMessenger messenger;
			aMessage->FindMessenger("messenger", &messenger);
			if (iWindow)
				iWindow->SetMessenger(messenger);
			break;
		}

		case SET_AVAIL:
		case SET_BRB:
		case SET_INVIS:
		case SET_NOT_AVAIL:
		case SET_DESCRIPTION:
		case BEGG_ABOUT:
		case SHOW_MAIN_WINDOW:
		{
			if (iWindow)
				BMessenger(iWindow).SendMessage(aMessage);
			break;
		}

		case OPEN_PROFILE_WIZARD:
		{
			ProfileWizard *pw = new ProfileWizard();
			pw->Show();
			break;
		}

		case PROFILE_CREATED:
		{
			aMessage->FindString("ProfileName", iLastProfileName);
			fprintf(stderr, "Setting last profile to %s\n", iLastProfileName->String());

			iFirstRun = false;
			// Fall through
		}

		case CONFIG_OK:
		{
			iProfile = new Profile();
			iProfile->Load(iLastProfileName);

			// XXX Here it's failing!
			if (!iProfile->iRememberPassword) {
				// We need to display the password prompt and try connecting
				// Only when everything's fine, we'll show the main window
				iPrompt = new LoginPrompt(new BMessenger(this));
				iPrompt->Show();
			}
			else {
				// In case where the password is remembered, just show the main window
				_ShowMainWindow(iProfile);
			}

			break;
		}

		case TRY_LOGIN:
		{
			LoginPrompt::ID id;
			aMessage->FindInt32("uin", (int32 *)&(id.UIN));
			aMessage->FindString("password", &(id.password));

			if (!_TryConnecting(&id))
				iPrompt->Show(); // Wrong UIN/password, try again

			// Correct password was given, show the main window
			delete iPrompt;
			_ShowMainWindow(iProfile);

			break;
		}

		case BEGG_QUIT:
		{
			BMessenger(iWindow).SendMessage(aMessage);
			break;
		}

		default:
		{
			BApplication::MessageReceived(aMessage);
			break;
		}
	}
}

bool TokuToku::_TryConnecting(LoginPrompt::ID *id)
{
	// TODO: This shouldn't be in TokuToku class
	struct gg_session *session;
	struct gg_login_params params;

	memset(&params, 0, sizeof(params));
	params.uin = id->UIN;
	params.password = (char *)id->password.String();
	params.async = 0;
	params.status = GG_STATUS_INVISIBLE;
	params.server_addr = inet_addr("91.214.237.10");

	session = gg_login(&params);
	if (session != NULL) {
		gg_logoff(session);
		gg_free_session(session);
		return true;
	}

	return false;
}

void TokuToku::_ShowMainWindow(Profile *profile)
{
	iWindow = new MainWindow(profile);

	if (iWindow->LockLooper()) {
		if (iWindow->IsHidden()) {
			iWindow->Show();
		} else {
			iWindow->Activate();
		}

		// XXX: Does this make any sense? Shouldn't I just not do Show()?
		if (iHideAtStart)
			iWindow->Hide();

		iWindow->UnlockLooper();
	}
}

void TokuToku::ReadyToRun()
	{
	fprintf( stderr, "TokuToku::ReadyToRun()\n" );
	AddDeskbarIcon();
	}

bool TokuToku::QuitRequested()
	{
	DelDeskbarIcon();
	if( iWindow->Lock() )
		{
		iWindow->Quit();
		}
		
	/* saving configuration */
	BMessage * cfgmesg = new BMessage();
	cfgmesg->AddBool( "iFirstRun", iFirstRun );
	cfgmesg->AddBool( "iHideAtStart", iHideAtStart );
	cfgmesg->AddString( "iLastProfileName", *iLastProfileName );
	
	BPath path;
	find_directory( B_USER_SETTINGS_DIRECTORY, &path );
	path.Append( "TokuToku/Config" );
	fprintf( stderr, "Last profile: %s\n", iLastProfileName->String() );
	fprintf( stderr, "Saving configuration to %s\n", path.Path() );
	BFile file( path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE );
	if( file.InitCheck() == B_OK )
		{
		cfgmesg->Flatten( &file );
		file.Unset();
		}
	
	delete cfgmesg;
	BApplication::QuitRequested();
	return true;
	}

void TokuToku::AddDeskbarIcon()
	{
	fprintf( stderr, "TokuToku::AddDeskbarIcon()\n" );
	BDeskbar deskbar;
	if( !deskbar.HasItem( "BGDeskbar" ) )
		{
		BRoster roster;
		entry_ref ref;
		status_t status = roster.FindApp( "application/x-vnd.TokuToku", &ref );
		if( status != B_OK )
			{
			fprintf( stderr, "Can't find TokuToku running %s\n", strerror( status ) );
			return;
			}
		status = deskbar.AddItem( &ref );
		if( status != B_OK )
			{
			fprintf( stderr, "Can't put TokuToku into Deskbar: %s\n", strerror( status ) );
			return;
			}
		}
	}

void TokuToku::DelDeskbarIcon()
	{
	fprintf( stderr, "TokuToku::DelDeskbarIcon()\n" );
	BDeskbar deskbar;
	if( deskbar.HasItem( "BGDeskbar" ) )
		{
		deskbar.RemoveItem( "BGDeskbar" );
		}
	}

MainWindow* TokuToku::GetMainWindow() const
	{
	return iWindow;
	}
	
DebugWindow* TokuToku::GetDebugWindow() const
	{
	return iDebugWindow;
	}

bool TokuToku::FirstRun()
	{
	return iFirstRun;
	}

bool TokuToku::HideAtStart()
{
	return iHideAtStart;
}

void TokuToku::SetHideAtStart(bool hide)
{
	iHideAtStart = hide;
}

BString* TokuToku::LastProfile()
	{
	return iLastProfileName;
	}

int main(void)
{
	TokuToku *tt = new TokuToku();

	tt->Run();

	delete tt;
	return 0;
}
