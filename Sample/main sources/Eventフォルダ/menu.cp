#include"protoType.h"
#include"resourceID.h"
#include"mtFloatWindow.h"


void HandleMenuChoice( long  theChoice ) 
{
	short  theMenu;
	short  theMenuItem;
	
	theMenu = HiWord( theChoice ); 
	theMenuItem = LoWord( theChoice ); 

	switch ( theMenu )
	{
		case mAppleID :
			HandleAppleChoice( theMenuItem );
		break;
				
		case mFileID :
			HandleFileChoice( theMenuItem );
		break;

		case mEditID :
			HandleEditChoice( theMenuItem );
		break;
		
		case mWindowID :
			HandleWindowChoice( theMenuItem );
		break;
	}
	HiliteMenu(0);
}


void  HandleAppleChoice( short theMenuItem )
{
	Str255		theItemName;
	short		theItemNumber;
	MenuHandle	theAppleMenu;

	switch ( theMenuItem )
	{
		case iAbout :
			SysBeep( 1 );
		break;

		default:
			theAppleMenu = GetMenuHandle( mAppleID );
			GetMenuItemText( theAppleMenu, theMenuItem, theItemName );
			theItemNumber = OpenDeskAcc( theItemName );
		break;
	}
}

void  HandleEditChoice( short theMenuItem )
{
	switch ( theMenuItem )
	{
		case iUndo :
		break;

		case iCut :
		break;

		case iCopy :
		break;

		case iPaste :
		break;

		case iClear :
		break;
	}
}


void HandleWindowChoice( short theMenuItem )
{
	WindowPtr	floatWindow = mtGetFWinWithNum( theMenuItem );
	MenuHandle	windowMenu = GetMenuHandle( mWindowID );
	Str255		menuText = { 0 };
	
	if( floatWindow != nil )
	{
		short	isShowData = mtAutoShowHideFWindow( floatWindow );
		
		if( isShowData != noFWindow )
		{
			switch( theMenuItem )
			{
				case( iTool ) :
					GetIndString( menuText, rWinMenuStrID, isShowData );	//STR#リソースから読み込む
					SetMenuItemText( windowMenu, iTool, menuText );
				break;
				
				case( iInfo ) :
					GetIndString( menuText, rWinMenuStrID, isShowData + 2 );	//STR#リソースから読み込む
					SetMenuItemText( windowMenu, iInfo, menuText );
				break;
			}
		}
	}
}

