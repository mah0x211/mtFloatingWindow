//__________________________________________//
//											//
//	1999年　10月30日							//
//											//
//											//
//__________________________________________//
//											//
//include
#ifndef		____MTFLOATWINDOW____
#include"mtFloatWindow.h"
#endif

#include"mtFloatWindowPri.h"

#pragma mark -
#pragma mark === DISPOSE FLOATINGWINDOW


//全てのFloatingWindowのDispose
void mtClearAllFloatWindow( FWClearRoutine userRoutine )
{
	WindowPtr	theWindow = mtGetFirstFloatWindow();
	
	while( theWindow != nil )
	{
		mtFWDataPtr		fwindowData = (mtFWDataPtr)GetWRefCon( theWindow );

		if( userRoutine != nil ){	userRoutine( &fwindowData->userRefCon );	}
	
		if( fwindowData != nil ){	delete fwindowData;	}
		
		//theWindowに設定されているスクロールバーのDispose
		mtDisposeScrllBarData( theWindow );
		
		KillControls( theWindow );
		DisposeWindow( theWindow );
		
		theWindow = mtGetFirstFloatWindow();
	}
}

//FloatingWindowのDispose
void mtClearFloatWindow( WindowPtr theWindow, FWClearRoutine userRoutine )
{
	if( theWindow != nil and priIsFloatWindow( theWindow ) )
	{
		mtFWDataPtr		fwindowData = (mtFWDataPtr)GetWRefCon( theWindow );

		if( userRoutine != nil ){	userRoutine( &fwindowData->userRefCon );	}
	
		if( fwindowData != nil ){	delete fwindowData;	}
		
		//theWindowに設定されているスクロールバーのDispose
		mtDisposeScrllBarData( theWindow );
		
		KillControls( theWindow );
		DisposeWindow( theWindow );
	}
}

//numberで指定した番号のFloatingWindowのDispose
void mtClearNumOfFloatWindow( short number, FWClearRoutine userRoutine )
{
	WindowPtr	theWindow = mtGetFWinWithNum( number );
	
	if( theWindow != nil )
	{
		mtFWDataPtr		fwindowData = (mtFWDataPtr)GetWRefCon( theWindow );

		if( userRoutine != nil ){	userRoutine( &fwindowData->userRefCon );	}
	
		if( fwindowData != nil ){	delete fwindowData;	}
		
		//theWindowに設定されているスクロールバーのDispose
		mtDisposeScrllBarData( theWindow );
		
		KillControls( theWindow );
		DisposeWindow( theWindow );
	}
}


#pragma mark -
#pragma mark === INIT FLOATING WINDOW DATA

//FloatingWindowDataを新しく作る
mtFWDataPtr priNewFloatData( void )
{
	mtFWDataPtr		fwindowData = new mtFloatData;
	
	if( fwindowData != nil )
	{
		WindowPtr	floatWindow = mtGetFirstFloatWindow();
		
		fwindowData->floatingKind = floatKind;
		fwindowData->userRefCon = 0;
		fwindowData->floatNumber = 1;	//FloatingWindowの番号
		fwindowData->osEvtShowFlag = true;//OSEvent時に表示・非表示のフラグ
		
		while( floatWindow != nil )	//フローティングウィンドウがNULLでは無い間
		{
			fwindowData->floatNumber++;		//FloatingWindowの番号を付ける
			floatWindow = mtGetNextFloatWindow( floatWindow );
		}
	}
	
	return( fwindowData );
}

#pragma mark -
#pragma mark === NEW FLOATING WINDOW
//procIDがFloatingWindowのprocIDかをチェック
Boolean priIsFWindowProcID( const short procID )
{
	Boolean		isFloatID = false;
	
	const short floatProcIDs[24] = {

		floatProc,
		floatGrowProc,
		floatZoomProc,
		floatZoomGrowProc,
		floatSideProc,
		floatSideGrowProc,
		floatSideZoomProc,
		floatSideZoomGrowProc,

		/* Proc IDs for top title bar theme-savvy floating windows */
		kWindowFloatProc,
		kWindowFloatGrowProc,
		kWindowFloatVertZoomProc,
		kWindowFloatVertZoomGrowProc,
		kWindowFloatHorizZoomProc,
		kWindowFloatHorizZoomGrowProc,
		kWindowFloatFullZoomProc,
		kWindowFloatFullZoomGrowProc,
	
		/* Proc IDs for side title bar theme-savvy floating windows */
		kWindowFloatSideProc,
		kWindowFloatSideGrowProc,
		kWindowFloatSideVertZoomProc,
		kWindowFloatSideVertZoomGrowProc,
		kWindowFloatSideHorizZoomProc,
		kWindowFloatSideHorizZoomGrowProc,
		kWindowFloatSideFullZoomProc,
		kWindowFloatSideFullZoomGrowProc
	};
	
	for( int i = 0; i < 24; i++ )
	{
		if( floatProcIDs[i] == procID )
		{
			isFloatID = true;
			break;
		}
	}
	return( isFloatID );
}

//PortRectのチェック
void priCheckWindowRect( WindowPtr theWindow, Rect maxSize )
{
	short		sysWidth = GetWidth( qd.screenBits.bounds );
	short		sysHeight = GetHeight( qd.screenBits.bounds );
	short		maxWidth = GetWidth( maxSize );
	short		maxHeight = GetHeight( maxSize );
	short		width = GetWidth( theWindow->portRect );
	short		height = GetHeight( theWindow->portRect );
	short		aWidth = width;
	short		aHeight = height;
	
	if( width > maxWidth )
	{
		aWidth = maxWidth;
		if( sysWidth < aWidth ){		aWidth = sysWidth;		}
	}
	if( height > maxHeight )
	{
		aHeight = maxHeight;
		if( sysHeight < aHeight ){		aHeight = sysHeight;		}
	}
	
	SizeWindow( theWindow, aWidth, aHeight, false );
}

//ウィンドウ・フローティングウィンドウの作成とrefConの設定
WindowPtr mtNewWindow( short procID, Rect winRect, Boolean closeBox, Str255 title, const Rect windowMaxSize, Boolean visible )
{
	WindowPtr	theWindow = nil;
	WindowPtr	activeWindow = mtGetActiveDocWindow();
	
	theWindow = NewCWindow( nil, &winRect, title, visible, procID,
								kLastWindowOfClass, closeBox, 0 );

	if( theWindow != nil )
	{
		priSettingWindowData( priIsFWindowProcID( procID ), theWindow );
		if( EmptyRect( &windowMaxSize ) == false )
		{
			priCheckWindowRect( theWindow, windowMaxSize );
			SetWindowStandardState( theWindow, &windowMaxSize );	//ズームサイズを設定
		}
	}
	
	return( theWindow );
}

//ウィンドウ・フローティングウィンドウのリソースからの作成とrefConの設定
WindowPtr mtGetNewWindow( short resourceID, const Rect windowMaxSize, Boolean isFloat )
{
	WindowPtr	theWindow = nil;
	RectPtr		windowRect = nil;
	
	theWindow = GetNewCWindow( resourceID, nil, kLastWindowOfClass );

	if( theWindow != nil )
	{
		priSettingWindowData( isFloat, theWindow );
		
		if( EmptyRect( &windowMaxSize ) == false )
		{
			priCheckWindowRect( theWindow, windowMaxSize );
			SetWindowStandardState( theWindow, &windowMaxSize );	//最大ズームサイズを設定
		}
	}
	
	return( theWindow );
}

void priSettingWindowData( Boolean isFloatWindow, WindowPtr theWindow )
{
	if( isFloatWindow )
	{
		mtFWDataPtr	fwindowData = priNewFloatData();
		
		if( fwindowData != nil )
		{
			if( MacIsWindowVisible( theWindow ) == false )
			{
				fwindowData->osEvtShowFlag = false;//OSEvent時に表示・非表示のフラグ
			}
			SetWRefCon( theWindow, (long)fwindowData );
			SetPort( theWindow );
			TextSize( 9 );
			priBringFWindow();
		}
		else{		DisposeWindow( theWindow );		}
	}
	else
	{
		priChangeDocWindow( theWindow );	//アクティブだったウィンドウのハイライトを消す
	}
}


/***************************************************************/

#pragma mark -
#pragma mark === UPDATE FLOATING WINDOW
/***************************アップデート処理**************************/
//アップデート処理の時に呼ぶ
Boolean mtUpdateIsFWindow( WindowPtr theWindow, UpDateRoutine userRoutine )
{
	Boolean		yes_no = false;		//true = フローティングウィンドウのアップデート

	if( priIsFloatWindow( theWindow ) )
	{
		yes_no = true;
		
		SetPort( theWindow );
	
		BeginUpdate( theWindow );

			//FloatingWindowのUpdate処理のUserRoutine
			if( userRoutine != nil ){	userRoutine( theWindow );	}
			else{		UpdateControls( theWindow, theWindow->visRgn );	}
	
		EndUpdate( theWindow );
	}
	
	return( yes_no );
}

/******************************************************************/
