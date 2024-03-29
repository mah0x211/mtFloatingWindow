#ifndef	___PROTOTYPE___
#define	___PROTOTYPE___

#include"mtFloatWindow.h"

void InitializeToolbox( void );
void SetUpMenuBar( void );

void SetUpToolWindow( void );
	OSErr SetUpFloatInfoWindow( void );
	OSErr SetUpFloatToolWindow( void );

void EventLoop( void );
void CleanUpMem( void );
void ClearWindow( WindowPtr theWindow );

void EventWhat( EventRecord theEvent );
void HandleUpdate( WindowPtr theWindow );

void HandleKeyDown( EventRecord );

//mouseDown
void HandleMouseDown( EventRecord theEvent );
	void MouseDownContent( WindowPtr theWindow, Point clickPt );
	void MouseDownFContent( WindowPtr theWindow, Point clickPt );
	
		void HandleClickCtrl( WindowPtr theWindow, ControlPartCode theCtrlPart, ControlHandle theCtrl, Point clickPt );


void HandleMenuChoice( long theChoice );
void HandleAppleChoice( short theMenuItem );
void HandleWindowChoice( short theMenuItem );

void HandleFileChoice( short theMenuItem );
	void FileNew( void );
	OSErr SetScrllBar( WindowPtr theWindow );

void HandleEditChoice( short theMenuItem );

//CallBack
mtWindowSizeData CallBackGetZoomGrowSizeData( void );
Boolean CallBackHandleGoAway( short thePart, Boolean goAwayFlag, WindowPtr theWindow );
void CallBackUpdate( WindowPtr theWindow, RgnHandle upRgn );


#endif