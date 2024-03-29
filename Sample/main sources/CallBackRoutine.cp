#include"protoType.h"
#include"resourceID.h"
#include"mtFloatWindow.h"

//
mtWindowSizeData CallBackGetZoomGrowSizeData( void )
{
	mtWindowSizeData		size = { 0 };
	Rect					growSize = { 1, 1, 0, 0 };

	size.minWidth = 100;
	size.minHeight = 100;
	size.widthSize = growSize;
	
	return( size );
}

Boolean CallBackHandleGoAway( short thePart, Boolean goAwayFlag, WindowPtr theWindow )
{
	switch( thePart )
	{
		case( inGoAway ) :
			//ウィンドウ・クローズボックス処理
			if( goAwayFlag ){		ClearWindow( theWindow );	}
		break;
		
		case( inFloatGoAway ) :
			//FloatingWindow・クローズボックス処理
			if( goAwayFlag )
			{
				HandleWindowChoice( mtGetFloatWindowNumber( theWindow ) );
			}
		break;
	}
	return( goAwayFlag );
}

void CallBackUpdate( WindowPtr theWindow, RgnHandle upRgn )
{
	#pragma unused( theWindow, upRgn )
	
	PicHandle		pict = GetPicture( 128 );
	
	if( pict )
	{
		DrawPicture( pict, &(*pict)->picFrame );	
		ReleaseResource( (Handle)pict );
	}
}
