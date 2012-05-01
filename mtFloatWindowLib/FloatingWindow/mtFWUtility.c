#ifndef		____MTFLOATWINDOW____
#include"mtFloatWindow.h"
#endif

#include"mtFloatWindowPri.h"


#pragma mark -
#pragma mark === GET & SET WINDOW CLIPING

//theWindowをclipAreaでクリッピングして、クリッピングされていたRectを返す
Rect mtSetClipRect( WindowPtr theWindow, Rect clipArea )
{
	Rect		saveClip = mtGetClipRect( theWindow );
	
	if( theWindow != nil )
	{
		WindowPtr	savePort = nil;
		
		GetPort( &savePort );
		SetPort( theWindow );
		
		ClipRect( &clipArea );
		
		SetPort( savePort );
	}
	
	return( saveClip );
}

//theWindowのClipRgnのRectを得る
Rect mtGetClipRect( WindowPtr theWindow )
{
	WindowPtr	savePort = nil;
	Rect			clipArea = { 0 };
	
	if( theWindow != nil )
	{
		RgnHandle		clipRgn = NewRgn();
		
		GetPort( &savePort );
		SetPort( theWindow );
		
		GetClip( clipRgn );
		
		SetPort( savePort );
		
		clipArea = (**clipRgn).rgnBBox;
		
		DisposeRgn( clipRgn );
	}
	return( clipArea );
}

//theWindowをclipAreaでクリッピングして、clipAreaをクリッピングされていたRegionにする
void mtSetClipRgn( WindowPtr theWindow, RgnHandle clipArea )
{
	if( theWindow != nil and clipArea != nil )
	{
		WindowPtr	savePort = nil;
		RgnHandle		clipRgn = clipArea;
		
		GetPort( &savePort );
		SetPort( theWindow );
		
		GetClip( clipArea );
		SetClip( clipRgn );
		
		SetPort( savePort );
	}
}


#pragma mark -
#pragma mark === Get WINDOW REGION RECT

//theWindowのContentRectを返す
Rect mtGetWindowContentRect( WindowPtr theWindow )
{
	return( (*( WindowPeek(theWindow)->contRgn ) )->rgnBBox );
}

//theWindowのSrucRectを返す
Rect mtGetWindowStrucRect( WindowPtr theWindow )
{
	return( (*( WindowPeek(theWindow)->strucRgn ) )->rgnBBox );
}

#pragma mark -
#pragma mark === SHOW WINDOW REGION 

//theWindowのContentRgnを返す
RgnHandle mtShowWindowContentRgn( WindowPtr theWindow )
{
	return( ( WindowPeek(theWindow)->contRgn ) );
}

//theWindowのUpdateRgnを返す
RgnHandle mtShowWindowUpdateRgn( WindowPtr theWindow )
{
	return( ( WindowPeek(theWindow)->updateRgn ) );
}

//theWindowのSrucRgnを返す
RgnHandle mtShowWindowStrucRgn( WindowPtr theWindow )
{
	return( WindowPeek(theWindow)->strucRgn );
}

//theWindow->portRectの最大サイズを得る
Rect mtGetWindowMaxPortRect( WindowPtr theWindow )
{
	Rect		maxPortRect = { 0 };
	
	if( theWindow != nil )
	{
		GetWindowStandardState( theWindow, &maxPortRect );	//theWindowの最大サイズを得る
		
		maxPortRect.bottom -= maxPortRect.top;
		maxPortRect.right -= maxPortRect.left;
		maxPortRect.top = 0;
		maxPortRect.left = 0;
	}
	return( maxPortRect );
}


#pragma mark -
#pragma mark === GET WINDOW FEATURE REGION
//typeCodeが指すRgnをtheWindowが持っているか調べる
Boolean priCanGetThisWindowFeatureRgn( WindowPtr theWindow, WindowRegionCode typeCode )
{
	Boolean		canGet = false;
	unsigned long	featureCode = 0;
	
	if( theWindow != nil )
	{
		switch( typeCode )
		{
			case( kWindowTitleTextRgn ) :
			case( kWindowTitleBarRgn ) :
				featureCode = kWindowHasTitleBar;
			break;
		
			case( kWindowZoomBoxRgn ) :
				featureCode = kWindowCanZoom;
			break;
		
			case( kWindowGrowRgn ) :
				featureCode = kWindowCanGrow;
			break;
		
			case( kWindowCollapseBoxRgn ) :
				featureCode = kWindowCanCollapse;
			break;
			
			case( kWindowCloseBoxRgn ) :
			case( kWindowDragRgn ) :
			case( kWindowStructureRgn ) :
			case( kWindowContentRgn ) :
				featureCode = kWindowCanGetWindowRegion;
			break;
		};

		if( theWindow != nil )
		{
			unsigned long	outFeatures = 0;
		
			if( GetWindowFeatures( theWindow, &outFeatures ) == noErr )
			{
				//特定のビットの値を調べる場合はBitAndを使用
				canGet = BitAnd( outFeatures, featureCode );
			}
		}
	}
	
	return( canGet );
}

/************
*	未完成	*
************/
//theWindowのtypeCodeで指定したRgnのwidthCode(幅か高さの指定)を返す
short mtGetWindowFeatureRgnWidth( WindowPtr theWindow,
						WindowRegionCode typeCode, const short widthCode )
{
	short		resultWidth = 0;
	
	if( theWindow != nil )
	{
		unsigned long	winFeatures = 0;
		
		if( priCanGetThisWindowFeatureRgn( theWindow, typeCode ) )
		{
			RgnHandle			featureRgn = NewRgn();
		
			if( GetWindowRegion( theWindow, typeCode, featureRgn ) == noErr )
			{
				Rect		rgnRect = (*featureRgn)->rgnBBox;
			
				switch( typeCode )
				{
					case( kWindowGrowRgn ) :
						//theWindowのGrowIconRgnの幅( 高さも同じ値 )を返す
						//growRgnの( bottom - top )か( right - left )から17を引いて
						//FloatingWindowのGrowIconの幅を足すと幅が得られる
						/*
						const short	floatGrowWidth = 9;		//FloatingWindowのGrowIconの幅
						const short	diffValue = 17;
						resultWidth = ( rgnRect.right - rgnRect.left ) - diffValue + floatGrowWidth;
						*/
						if( priIsFloatWindow( theWindow ) ){	resultWidth = 9;	}
						else{		resultWidth = 14;	}
					break;
					
					case( kWindowCloseBoxRgn ) :
					case( kWindowZoomBoxRgn ) :
					case( kWindowCollapseBoxRgn ) :
						resultWidth = rgnRect.bottom - rgnRect.top;
					break;
					
					case( kWindowTitleTextRgn ) :
					case( kWindowTitleBarRgn ) :
					case( kWindowDragRgn ) :
					case( kWindowStructureRgn ) :
					case( kWindowContentRgn ) :
						if( widthCode == rgnHeight )
						{
							resultWidth = rgnRect.bottom - rgnRect.top;
						}
						else if( widthCode == rgnWidth )
						{
							resultWidth = rgnRect.right - rgnRect.left;
						}
					break;
				};
			}
			DisposeRgn( featureRgn );
		}
	}
	return( resultWidth );
}

#pragma mark -
#pragma mark === GET FLOATING WINDOW DATA MENBER

//theFloatWindowの作られた番号を返す（FloatingWindowで無ければ0を返す）
short mtGetFloatWindowNumber( WindowPtr theFloatWindow )
{
	short	floatNumber = 0;
	
	if( theFloatWindow != nil and priIsFloatWindow( theFloatWindow ) )
	{
		floatNumber = ( (mtFWDataPtr)GetWRefCon( theFloatWindow ) )->floatNumber;
	}
	return( floatNumber );
}



#pragma mark -
#pragma mark === SET GET FLOATINGWINDOW REFCON

//FloatWindowのRefConを設定する
void mtSetFWRefCon( WindowPtr theFloatWindow, long data )
{
	if( theFloatWindow != nil and priIsFloatWindow( theFloatWindow ) )
	{
		((mtFWDataPtr)GetWRefCon( theFloatWindow ))->userRefCon = data;
	}
}

//FloatWindowのRefConを取得する
long mtGetFWRefCon( WindowPtr theFloatWindow )
{
	long		data = 0;
	
	if( theFloatWindow != nil and priIsFloatWindow( theFloatWindow ) )
	{
		data = ((mtFWDataPtr)GetWRefCon( theFloatWindow ))->userRefCon;
	}
	
	return( data );
}


#pragma mark -
#pragma mark === CHACK FLOATING WINDOW


//OSEvent時に表示・非表示のフラグを返す
Boolean priIsFloatOSShowFlag( WindowPtr theWindow )
{
	Boolean	showFlag = false;
	
	if( theWindow != nil and priIsFloatWindow( theWindow ) )
	{
		showFlag = ((mtFWDataPtr)GetWRefCon( theWindow ))->osEvtShowFlag;
	}
	
	return( showFlag );
}

//OSEvent時に表示・非表示のフラグを設定する
void priSetFloatOSShowFlag( WindowPtr theWindow, Boolean showFlag )
{
	if( theWindow != nil and priIsFloatWindow( theWindow ) )
	{
		((mtFWDataPtr)GetWRefCon( theWindow ))->osEvtShowFlag = showFlag;
	}
}

//theWindowがFloatingWindowかを調べる
Boolean priIsFloatWindow( WindowPtr theWindow )
{
	Boolean		isFWindow = false;
	
	if( theWindow != nil and GetWindowKind( theWindow ) != kDialogWindowKind )
	{
		if( ((mtFWDataPtr)GetWRefCon( theWindow ))->floatingKind == floatKind and 
			( (mtFWDataPtr)GetWRefCon( theWindow ) )->floatNumber > 0 )
		{
			isFWindow = true;
		}
	}
	return( isFWindow );
}

//theWindowがFloatingWindowかをTitleBarの幅を使って調べる
Boolean priIsFloatWindowUseTitleBar( WindowPtr theWindow )
{
	Boolean		isFloating = false;
	const short	floatTitleWidth = 14;
	
	if( theWindow != nil )
	{
		short titleWidth = mtGetWindowFeatureRgnWidth( theWindow, kWindowTitleBarRgn, rgnWidth );
		short titleHeight = mtGetWindowFeatureRgnWidth( theWindow, kWindowTitleBarRgn, rgnHeight );
	
		//TitleBarの幅でFloatingWindowかDocumentWindowなのかを調べる
		if( titleWidth == floatTitleWidth or titleHeight == floatTitleWidth )
		{
			isFloating = true;
		}
	}
	return( isFloating );
}

//どのWindowをクリックしたか調べる
Boolean priIsClickFWindow( WindowPtr theWindow )
{
	Boolean		isFloatWin = false;
	WindowPtr	frontWin = FrontWindow();
	
	if( theWindow != nil )
	{
		SetPort( theWindow );		//ClickしたWindowをカレントにする
		//FloatWindowなら
		if( priIsFloatWindow( theWindow ) )
		{
			isFloatWin = true;
		
			if( theWindow != frontWin )	//FloatWindowが一番手前では無いなら手前にする
			{
				BringToFront( theWindow );
			}
		}
	}
	return( isFloatWin );
}



#pragma mark -
#pragma mark === SHOW HIDE FLOATINGWINDOW

//フローティングウィンドウを表示・非表示
void mtShowHideFWindow( WindowPtr theWindow, Boolean isShow )
{
	if( theWindow != nil and priIsFloatWindow( theWindow ))
	{
		ShowHide( theWindow, isShow );
		priSetFloatOSShowFlag( theWindow, isShow );
		
		priBringFWindow();
	}
}

//フローティングウィンドウを表示・非表示を自動的に決める	/*表示したなら1を返す FloatingWindowでは無いなら-1*/
short mtAutoShowHideFWindow( WindowPtr theWindow )
{
	short		isShow = noFWindow;	//表示したなら1を返す
	
	if( theWindow != nil and priIsFloatWindow( theWindow ))
	{
		if( MacIsWindowVisible( theWindow ) )
		{
			ShowHide( theWindow, false );	//Show
			priSetFloatOSShowFlag( theWindow, false );
			isShow = hFWindow;
		}
		else
		{
			ShowHide( theWindow, true );	//Hide
			priSetFloatOSShowFlag( theWindow, true );
			isShow = sFWindow;
		}
		priBringFWindow();
	}
	
	return( isShow );
}

//全てのフローティングウィンドウを表示・非表示
void mtAllShowHideFWindow( Boolean isShow )
{
	WindowPtr	theWindow = mtGetFirstFloatWindow();
	
	while( theWindow != nil )
	{
		ShowHide( theWindow, isShow );
		
		priSetFloatOSShowFlag( theWindow, isShow );
		
		theWindow = mtGetNextFloatWindow( theWindow );
	}
	
	if( isShow ){		priBringFWindow();		}
}

#pragma mark -
#pragma mark === FIND FLOATINGWINDOW

//FloatingWindowの作られた番号からFloatingWindowを探して返す（無ければnilを返す）
WindowPtr mtGetFWinWithNum( short windowNumber )
{
	WindowPtr	windowList = nil;
	
	if( windowNumber > 0 )
	{
		windowList = mtGetFirstFloatWindow();
		
		while( windowList != nil )
		{
			if( windowNumber == mtGetFloatWindowNumber( windowList ) )
			{
				break;
			}
			windowList = mtGetNextFloatWindow( windowList );
		}
	}
	return( windowList );
}

//一番前にあるFloatingWindowを探す（無ければnilを返す）
WindowPtr mtGetFirstFloatWindow( void )
{
	//FrontWindow() は表示されていないウィンドウは飛ばすので
	WindowPtr	windowList = LMGetWindowList();

	while( windowList != nil )
	{
		if( priIsFloatWindow( windowList ) ){		break;	}
		windowList = GetNextWindow( windowList );
	}

	return( windowList );
}

//floatWindowの次にくるFloatingWindowを返す（無ければnilを返す）
WindowPtr mtGetNextFloatWindow( WindowPtr floatWindow )
{
	WindowPtr	windowList = nil;

	if( floatWindow != nil and priIsFloatWindow( floatWindow ) )
	{
		windowList = floatWindow;

		while( windowList != nil )
		{
			if( windowList != floatWindow )
			{
				if( priIsFloatWindow( windowList ) )
				{
					break;
				}
			}
			windowList = GetNextWindow( windowList );
		}
	}
	return( windowList );
}

#pragma mark -
#pragma mark === GET ACTIVE DOCWINDOW

//現在アクティブなドキュメントウィンドウへのポインタを返す
WindowPtr mtGetActiveDocWindow( void )
{
	WindowPtr	activeWin = FrontWindow();

	while( activeWin != nil )	
	{
		if( priIsFloatWindow( activeWin ) == false and	//FloatingWindowでは無く
			GetWindowKind( activeWin ) != dialogKind )	//DialogWindowでは無いなら
		{
			break;
		}
		activeWin = GetNextWindow( activeWin );
	}
	
	return( activeWin );
}

#pragma mark -
#pragma mark === WINDOW UTILITY

//フローティングウィンドウを前に持ってくる
void priBringFWindow( void )
{
	WindowPtr	floatWindow = mtGetFirstFloatWindow();

	while( floatWindow != nil )	//フローティングウィンドウがNULLでは無い間
	{
		WindowPtr	nextFWindow = mtGetNextFloatWindow( floatWindow );
		
		if( MacIsWindowVisible( floatWindow ) and 
			IsWindowHilited( floatWindow ) == false )	//もしハイライトしてないなら
		{
			HiliteWindow( floatWindow, true );
		}
		
		if( MacIsWindowVisible( floatWindow ) and 
			floatWindow != FrontWindow() )//ウィンドウが一番手前では無く、表示されているなら
		{
			BringToFront( floatWindow );	//一番手前にする
		}
		floatWindow = nextFWindow;
	}

	SetPort( floatWindow );
}


#pragma mark -
#pragma mark === WINDOW CONTROL UTILITY

//ControlのActivate・Deactivate処理
void mtActivateDeactivateCtrl( WindowPtr theWindow, Boolean isActivate )
{
	if( theWindow != nil )
	{
		SetPort( theWindow );
		
		if( isActivate ){	priActivateCtrl( theWindow );	}
		else{		priDeactivateCtrl( theWindow );	}
	}
}


//ActivateControl処理
void priActivateCtrl( WindowPtr theWindow )
{
	if( theWindow != nil )
	{
		ControlHandle		theControl = mtGetWindowCtrlList( theWindow );
	
		if( theControl != nil )
		{
			if( GetSuperControl( theControl, &theControl ) == noErr )
			{
				ActivateControl( theControl );
			}
		}
	}
}

//DeactivateControl処理
void priDeactivateCtrl( WindowPtr theWindow )
{
	if( theWindow != nil )
	{
		ControlHandle		theControl = mtGetWindowCtrlList( theWindow );

		if( theControl != nil )
		{
			if( GetSuperControl( theControl, &theControl ) == noErr )
			{
				DeactivateControl( theControl );
			}
		}
	}
}

//theWindowにあるControlList( 先頭のControlHandleへのポインタ )を得る
ControlHandle mtGetWindowCtrlList( WindowPtr theWindow )
{
	if( theWindow != nil )
	{
		return( (ControlHandle) WindowPeek( theWindow )->controlList );
	}
	else{		return( nil );	}
};