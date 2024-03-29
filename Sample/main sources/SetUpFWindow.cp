#include"prototype.h"
#include"resourceID.h"
#include"mtFloatWindow.h"


void SetUpToolWindow( void )
{
	SetUpFloatToolWindow();
	SetUpFloatInfoWindow();
}

OSErr SetUpFloatToolWindow( void )
{
	Rect				windowRect = { 33, 2, 25*11+30, 51 };
	WindowPtr		theWindow = mtGetNewWindow( rFWindow1ID, windowRect, true );
	
	if( theWindow == nil ){	return( 1 );	}
	
	return( noErr );
}

OSErr SetUpFloatInfoWindow( void )
{
	Rect		windowRect = qd.screenBits.bounds; 	//グローバル座標のRectをAreaにいれる
	
	windowRect.top = windowRect.bottom - 155;
	windowRect.left = windowRect.right - 280;
	windowRect.bottom =155 + windowRect.top;
	windowRect.right = 280 + windowRect.left;
	
	WindowPtr	theWindow = mtGetNewWindow( rFWindow2ID, windowRect, true );
	
	if( theWindow != nil )
	{
		return( 1 );
	}
	
	return( noErr );
}

