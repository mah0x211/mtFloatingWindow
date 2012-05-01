#include"protoType.h"
#include"resourceID.h"
#include"mtFloatWindow.h"


void HandleMouseDown( EventRecord theEvent )
{
	WindowPtr		theWindow;
	short			thePart = 0;
				
	thePart = FindWindow( theEvent.where, &theWindow );

	switch ( thePart = mtMouseEventFloat( theWindow, thePart, theEvent.where,
					CallBackHandleGoAway, CallBackGetZoomGrowSizeData ) )
	{
		case( inMenuBar ) :
			long theChoice = 0;
			
			theChoice = MenuSelect( theEvent.where );
			if ( theChoice != 0 ){	HandleMenuChoice( theChoice );	}
		break;
		
		case( inContent ) :
			MouseDownContent( theWindow, theEvent.where );
		break;
		
		case( inFloatContent ) :	//フローティングウィンドウをクリック
			MouseDownFContent( theWindow, theEvent.where );
		break;
	};
}

void MouseDownFContent( WindowPtr theWindow, Point clickPt )
{
	#pragma unused( theWindow, clickPt )
	
	SysBeep( 1 );
}

void MouseDownContent( WindowPtr theWindow, Point clickPt )
{
	ControlHandle		theCtrl = nil;
	ControlPartCode	thePart = 0;	//クリックしたアイコンを探す
	
	GlobalToLocal( &clickPt );
	
	theCtrl = FindControlUnderMouse( clickPt, theWindow, &thePart );
								 
	if( thePart != kControlNoPart )
	{
		SetPort( theWindow );
		HandleClickCtrl( theWindow, thePart, theCtrl, clickPt );
	}
}

void HandleClickCtrl( WindowPtr theWindow, ControlPartCode theCtrlPart, ControlHandle theCtrl, Point clickPt )
{
	Rect			clipArea = theWindow->portRect;
	Rect			saveArea = mtGetClipRect( theWindow );
	
	switch( theCtrlPart )
	{
		case( kControlUpButtonPart ) :
		case( kControlDownButtonPart ) :
		case( kControlPageUpPart ) :
		case( kControlPageDownPart ) :
		case( kControlIndicatorPart ) :
			short	moveValue = 0;
			clipArea.right -= 15;
			clipArea.bottom -= 15;
			
			moveValue =  mtTrackWindowScrlBar( theCtrl, clickPt, theCtrlPart, clipArea, nil );
		break;
	};
	ClipRect( &saveArea );
}