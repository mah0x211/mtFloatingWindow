#include"prototype.h"
#include"resourceID.h"
#include"mtFloatWindow.h"

void HandleFileChoice( short theItem )
{
	extern	Boolean	gDone;
	
	switch ( theItem )
	{
		case iNew :
			FileNew();
		break;
		
		case iOpen :
		break;
		
		case iClose :
		break;
		
		case iSave :
		break;
		
		case iRevert :
		break;
		
		case iPSetting :
		break;
		
		case iPrint :
		break;
		
		case iQuit :
			gDone = true;
		break;
	}
}

void FileNew( void )
{
	Rect				maxWinRect = { 40, 40, 840, 940 };
	WindowPtr		theWindow = mtGetNewWindow( rWindowID, maxWinRect, false );
	extern			Boolean	gDone;

	if( theWindow != nil )
	{
		SetPort( theWindow );
		
		if( SetScrllBar( theWindow ) != noErr )
		{
			gDone = true;
		}
	}
}

OSErr SetScrllBar( WindowPtr theWindow )
{
	ControlHandle		theScrl = nil;
	short			height = 0;
	short			width = 0;
	short			oneUnit = 1;
	short			onePageUnit = 10;

	theScrl = mtNewStandardScrollBar( theWindow, true, height, width, vScrl );
	if( theScrl == nil ){		return( memFullErr );	}
	mtSetScrlUnit( theScrl, oneUnit, onePageUnit, vScrl, CallBackUpdate );


	theScrl = mtNewStandardScrollBar( theWindow, true, height, width, hScrl );
	if( theScrl == nil ){		return( memFullErr );	}
	mtSetScrlUnit( theScrl, oneUnit, onePageUnit, hScrl, CallBackUpdate );
	
	return( noErr );
}

