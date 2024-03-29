//____________________________________________________________
//
//         FloatingWindow Sample Program
//
//		作成者	: 	Masatoshi Teruya
//		開発環境	:	CodeWarrior Pro4
//
//____________________________________________________________

#include"protoType.h"
#include"resourceID.h"
#include"mtFloatWindow.h"

Boolean		gDone = false;

//____________________________________________________________


void  main( void )
{
	InitializeToolbox();
	SetUpMenuBar();
	SetUpToolWindow();
	
	EventLoop();
	
	CleanUpMem();
}

void  InitializeToolbox( void )
{
	InitGraf( &qd.thePort );
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs( 0L );
	FlushEvents( everyEvent, 0 );
	InitCursor();
	
	MoreMasters();
	MoreMasters();
	MoreMasters();
	
	MaxApplZone();
	
	RegisterAppearanceClient();
}
//____________________________________________________________

void  SetUpMenuBar( void )  
{ 
	Handle	theMenuBar;  
	MenuHandle  theAppleMenu;

	theMenuBar = GetNewMBar( rMenuBarID );
	SetMenuBar( theMenuBar );
	DisposeHandle( theMenuBar );

	theAppleMenu = GetMenuHandle( mAppleID );
	AppendResMenu( theAppleMenu, 'DRVR' ); 

	DrawMenuBar(); 
}


//____________________________________________________________

void  EventLoop( void )
{
	EventRecord	theEvent;
	
	
	while ( gDone == false )
	{
		if( WaitNextEvent( everyEvent, &theEvent, 15L, nil ) )
		{
			EventWhat( theEvent );
		}
	}
}

void CleanUpMem( void )
{
	mtClearAllFloatWindow( nil );
	
	WindowPtr	theWindow = FrontWindow();
	
	while( theWindow != nil )
	{
		WindowPtr	nextWindow = GetNextWindow( theWindow );
		
		ClearWindow( theWindow );
		
		theWindow = nextWindow;
	}
}

//WindowのDispose
void ClearWindow( WindowPtr theWindow )
{
	if( theWindow != nil )
	{
		KillControls( theWindow );
		DisposeWindow( theWindow );
	}
}

/*******************************/
