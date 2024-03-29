#include"protoType.h"
#include"resourceID.h"
#include"mtFloatWindow.h"


//
void EventWhat( EventRecord theEvent )
{
	switch ( theEvent.what )
	{
		case mouseDown:
			HandleMouseDown( theEvent );
		break;

		case keyDown:
			HandleKeyDown( theEvent );
		break;

		case updateEvt:
			HandleUpdate( (WindowPtr)theEvent.message );
		break;
	
		case activateEvt :
			if( BitAnd( theEvent.modifiers, activeFlag ) )
			{
				mtActiveDeactiveWindow( true, false);
			}
			else{	mtActiveDeactiveWindow( false, false);		}
		break;
		
		case osEvt :
			if((theEvent.message & osEvtMessageMask) == (suspendResumeMessage << 24))
			{
				//アプリケーションがアクティブになったら
				if(theEvent.message & resumeFlag){	mtActiveDeactiveWindow( true, true );	}
			        else{		mtActiveDeactiveWindow( false, true );	}
			}
		break;
	}
}
//____________________________________________________________


//ウィンドウのアップデート
void HandleUpdate( WindowPtr theWindow )
{
	if( mtUpdateIsFWindow( theWindow, nil ) == false )
	{
		SetPort( theWindow );
		
		BeginUpdate( theWindow );
		
			EraseRect( &theWindow->portRect );

			PicHandle		pict = GetPicture( 128 );
	
			if( pict )
			{
				DrawPicture( pict, &(*pict)->picFrame );	
				ReleaseResource( (Handle)pict );
			}
			
			mtDrawScrlBar( theWindow );
		
		EndUpdate( theWindow );
	}
}
//____________________________________________________________


