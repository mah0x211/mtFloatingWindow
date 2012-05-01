#ifndef		____MTFLOATWINDOW____
#include"mtFloatWindow.h"
#endif

#include"mtFloatWindowPri.h"
		
#pragma mark === WINDOW ACTIVATE・DEACTIVATE

//WindowのActivate・Deactivate処理
void mtActiveDeactiveWindow( Boolean active_deactive, Boolean isOSEvt )
{
	WindowPtr	theWindow = mtGetActiveDocWindow();
	FlushEvents( everyEvent, 0 );
	
	if( mtGetFirstFloatWindow() != nil )		//FloatingWindowがあれば
	{
		//active_deactive = true なら Active
		//Activate
		if( active_deactive){	priActivateWindow( isOSEvt );		}
		//Deactivate
		else{		priDeactivateWindow( isOSEvt );	}
	}
	
	if( theWindow != nil )
	{
		mtActivateDeactivateCtrl( theWindow, active_deactive );
		HiliteWindow( theWindow, active_deactive );
		InvalRgn( theWindow->visRgn );
	}
	
}

//ActivateWindow処理
void priActivateWindow( Boolean isOSEvt )
{
	WindowPtr	theWindow = mtGetFirstFloatWindow();	//一番前に表示されているFloatingWindowを得る
	
	while( theWindow != nil )
	{
		if( isOSEvt )
		{
			if( priIsFloatOSShowFlag( theWindow ) and MacIsWindowVisible( theWindow ) == false )
			{
				ShowHide( theWindow, true );
			}
		}
		
		if( isOSEvt == false )
		{
			if( MacIsWindowVisible( theWindow ) )
			{
				priActivateCtrl( theWindow );	//OSEventではWindowを隠すからControlのActivateは必要無い
				InvalRgn( theWindow->visRgn );
			}
		}
		
		if( MacIsWindowVisible( theWindow ) and IsWindowHilited( theWindow ) == false )
		{
			HiliteWindow( theWindow, true );
		}
		
		theWindow = mtGetNextFloatWindow( theWindow );
	}
}

//Deactivate処理
void priDeactivateWindow( Boolean isOSEvt )
{
	WindowPtr	theWindow = mtGetFirstFloatWindow();
	
	while( theWindow != nil )
	{
		if( isOSEvt and MacIsWindowVisible( theWindow ) )
		{
			ShowHide( theWindow, false );
		}
		
		if( isOSEvt == false )
		{
			if( MacIsWindowVisible( theWindow ) )
			{
				priDeactivateCtrl( theWindow );	//OSEventではWindowを隠すからControlのActivateは必要無い
				InvalRgn( theWindow->visRgn );
				if( IsWindowHilited( theWindow ) ){	HiliteWindow( theWindow, false );	}
			}
		}
		
		theWindow = mtGetNextFloatWindow( theWindow );
	}
}



#pragma mark -
#pragma mark === FLOATING WINDOW MOUSE EVENT
//FloatingWindow及びDocWindowをイベント処理
short mtMouseEventFloat( WindowPtr theWindow, short thePart, Point clickPt,
									GoAwayRoutine goAwayRoutine,
									GetZoomGrowSizeData zoom_growSize )
{
	if( theWindow != nil )
	{
		Boolean		clickFWindow = priIsClickFWindow( theWindow );

		switch ( thePart )
		{
			case( inContent ) :
				//clickしたのがFloatingWindowなら
				if( clickFWindow ){	thePart = inFloatContent;		}
				else
				{
					if( theWindow != mtGetActiveDocWindow() )
					{
						priChangeDocWindow( theWindow );
						thePart = inDeactiveWindow;
					}
				}
			break;
		
			case( inDrag ) :
				thePart = inNoWindow;
				//clickしたのがFloatingWindowなら
				if( clickFWindow ){		priClickFWDrag( theWindow, clickPt );	}//FloatingWindowのDrag
				else
				{
					//DocWindowのDrag
					if( theWindow != mtGetActiveDocWindow() )
					{
						priChangeDocWindow( theWindow );
						thePart = inDeactiveWindow;
					}
					priDragDocWindow( theWindow, clickPt );
				}
			break;
		
			case( inGrow ) :
				thePart = inNoWindow;
				if( clickFWindow ){	priFWindowGrow( theWindow, clickPt );	}
				else
				{	if( theWindow != mtGetActiveDocWindow() )
					{
						priChangeDocWindow( theWindow );
						thePart = inDeactiveWindow;
					}
					priWindowGrow( theWindow, clickPt, zoom_growSize );
				}
			break;
		
			case( inZoomIn ) :
			case( inZoomOut ) :
				short savePart = thePart;
				if( clickFWindow == false )
				{
					if( theWindow != mtGetActiveDocWindow() )
					{
						priChangeDocWindow( theWindow );
						thePart = inDeactiveWindow;
					}
				}
				priWindowZoom( theWindow, savePart, clickPt );
			break;
			
			case( inGoAway ) :
				priWindowGoAway( theWindow, clickPt, goAwayRoutine );
				thePart = inNoWindow;
			break;
		};
	}
	return( thePart );
}

/****************フローティングウィンドウのイベント処理⇩*********************/
//フローティングウィンドウのドラッグ処理
void priClickFWDrag( WindowPtr window, Point mouse )
{
	Rect		Area = qd.screenBits.bounds;	//グローバル座標のRectをAreaにいれる
	
	InsetRect( &Area, 4, 4 );				//Areaの縦・横の値を4ピクセルずつ縮める

	DragWindow( window, mouse, &Area );

	if( priIsFloatWindow( window ) )
	{
		window = GetNextWindow( window );		//１つ後ろのウィンドウを得る
		if( window != nil and MacIsWindowVisible( window ) )		//表示されているなら
		{
			HiliteWindow( window, true );
		}
	}
}

//ウィンドウ・goAwayボックス処理
void priWindowGoAway( WindowPtr theWindow, Point clickPt, GoAwayRoutine userRoutine )
{
	short	thePart = inGoAway;
	Boolean	goAwayFlag = false;
	
	
	goAwayFlag = TrackGoAway( theWindow, clickPt );

	if( priIsClickFWindow( theWindow ) )
	{
		thePart = inFloatGoAway;
		if( userRoutine != nil ){	userRoutine( thePart, goAwayFlag, theWindow );	}
	}
	else
	{
		Boolean	isGoAway = false;
		
		//theWindowの次のWindowへのポインタを得る
		WindowPtr	nextActiveWindow = GetNextWindow( theWindow );

		if( userRoutine != nil ){	isGoAway = userRoutine( thePart, goAwayFlag, theWindow );		}

		//nextActiveWindowがnilでは無く、theWindowを閉じたなら
		if( nextActiveWindow != nil and isGoAway )
		{
			//nextActiveWindowをアクティブ表示にする
			HiliteWindow( nextActiveWindow, true );
			mtActiveDeactiveWindow( true, false );
		}
	}
}

//フローティングウィンドウ・サイズ処理
void priFWindowGrow( WindowPtr theWindow, Point clickPt )
{
	long			afterSize = 0;	//サイズ変更後の高さ・横幅
	Rect			theWindowMaxSize = mtGetWindowMaxPortRect( theWindow );	//theWindowの最大サイズ
	Rect			oldPortRect = theWindow->portRect;
	Rect			portRect = theWindow->portRect;
	RgnHandle		updateRgn = NewRgn();
	
	
	DiffRgn( theWindow->visRgn, theWindow->clipRgn, updateRgn );
	
	theWindowMaxSize.bottom = theWindowMaxSize.bottom - theWindowMaxSize.top + 1;
	theWindowMaxSize.right = theWindowMaxSize.right - theWindowMaxSize.left + 1;
	
	theWindowMaxSize.top = 30;
	theWindowMaxSize.left = 30;
	
	
	//サイズに変更が無ければ0が入る
	afterSize = GrowWindow( theWindow, clickPt, &theWindowMaxSize );

	if( afterSize != 0 )
	{
		short	afterWidth = LoWord( afterSize );
		short	afterHeight = HiWord( afterSize );
		
		SizeWindow( theWindow, afterWidth, afterHeight, true );
		
		priStandardMoveScrlBar( theWindow, oldPortRect );
		
		InvalRgn( updateRgn );
	}
	DisposeRgn( updateRgn );
}

//ウィンドウ・サイズ処理
void priWindowGrow( WindowPtr theWindow, Point clickPt,
					GetZoomGrowSizeData zoom_growSize )
{
	long			afterSize = 0;	//サイズ変更後の高さ・横幅
	Rect			theWindowMaxSize = mtGetWindowMaxPortRect( theWindow );	//theWindowの最大サイズ
	Rect			oldPortRect = theWindow->portRect;
	Rect			portRect = theWindow->portRect;
	RgnHandle		updateRgn = NewRgn();
	mtWindowSizeData		sizeData = { 0 };
	
	if( zoom_growSize != nil ){		sizeData = zoom_growSize();		}
	
	DiffRgn( theWindow->visRgn, theWindow->clipRgn, updateRgn );
	
	theWindowMaxSize.top = sizeData.minHeight;
	theWindowMaxSize.left = sizeData.minWidth;
	
	theWindowMaxSize.bottom = theWindowMaxSize.bottom - portRect.top + 1;
	theWindowMaxSize.right = theWindowMaxSize.right - portRect.left + 1;
	
	//サイズに変更が無ければ0が入る
	afterSize = GrowWindow( theWindow, clickPt, &theWindowMaxSize );

	if( afterSize != 0 )
	{
		short	afterWidth = LoWord( afterSize );
		short	afterHeight = HiWord( afterSize );
		
		short	widthValue = 1;
		short	heightValue = 1;
		
		if( sizeData.widthSize.left > 0 )
		{
			widthValue = afterWidth / sizeData.widthSize.left;
		}
		if( sizeData.widthSize.top > 0 )
		{
			heightValue = afterHeight / sizeData.widthSize.top;
		}
		
		afterWidth = sizeData.widthSize.left * widthValue + sizeData.widthSize.right;
		afterHeight = sizeData.widthSize.top * heightValue + sizeData.widthSize.bottom;
		
		SizeWindow( theWindow, afterWidth, afterHeight, true );
		
		priStandardMoveScrlBar( theWindow, oldPortRect );
		
		InvalRgn( updateRgn );
	}
	DisposeRgn( updateRgn );
}

/****************
*　問題箇所		*
****************/
//ウィンドウ・ズームボックス処理
void priWindowZoom( WindowPtr theWindow, short partCode, Point clickPt )
{
	Boolean		click = false;
	Rect			sysRect = qd.screenBits.bounds;	//最大・最小サイズ
	Rect			maxSize = { 0 };
	Rect			oldPortRect = theWindow->portRect;
	Rect			strucRect = mtGetWindowStrucRect( theWindow );
	short		afterWidth = GetWidth( oldPortRect );
	short		afterHeight = GetHeight( oldPortRect );
	
	GetWindowStandardState( theWindow, &maxSize );
	
	
	sysRect.top += 40;	sysRect.left += 5;
	sysRect.right -= 5;	sysRect.bottom -= 5;

	SetPort( theWindow );
	click = TrackBox( theWindow, clickPt, partCode );

	if( click == true )
	{
		Rect		secondSize = { 0 };
		
		GetWindowUserState( theWindow, &secondSize );
		
		afterWidth += GetWidth( maxSize ) - oldPortRect.right;
		afterHeight += GetHeight( maxSize ) - oldPortRect.bottom;
		
		EraseRect( &theWindow->portRect );//EraseRectをしないとスクロールバーの線が消されない

		if( ( afterWidth + strucRect.left + 17 ) > sysRect.right )
		{
			afterWidth -= ( afterWidth + strucRect.left ) -  sysRect.right + 25;
		}
		if( ( afterHeight + strucRect.top + 17 ) > sysRect.bottom )
		{
			afterHeight -= ( afterHeight + strucRect.top ) - sysRect.bottom + 25;
		}
		
		
		if( GetWidth( strucRect ) == afterWidth + 6 + 7 and
			GetHeight( strucRect ) == afterHeight + 22 + 7 )
		{
			SizeWindow( theWindow, GetWidth( secondSize ),
						GetHeight( secondSize ), true );
		}
		else
		{
			SetWindowUserState( theWindow, &strucRect );
			SizeWindow( theWindow, afterWidth, afterHeight, true );
		}
		
		/****/
	/** ZoomWindow( theWindow, partCode, false );//trueならそのウィンドウをアクティブにする
		/****/
		
		priStandardMoveScrlBar( theWindow, oldPortRect );
	}
}

#pragma mark -
#pragma mark === DOCWINDOW DRAG ROUTINE

//標準ウィンドウのドラッグ処理
void priDragDocWindow( WindowPtr window, Point mouse )
{
	GrafPtr		systemPort = nil;				//画面全体( OSの画面 )
	RgnHandle		saveSystemClip = NewRgn();		//systemPortのクリップ領域の確保用
	Rect			moveArea = qd.screenBits.bounds;	//マウスの移動範囲
	RgnHandle		newSystemClip = NewRgn();		//systemPortの新しいクリップ領域
	RgnHandle		dragRgn = NewRgn();				//ウィンドウの枠の領域
	long			afterDragArea = 0;
	WindowPtr	floatWindow = mtGetFirstFloatWindow();


	InsetRect( &moveArea, 4, 4 );		//moveAreaの縦・横の値を4ピクセルずつ縮める

	GetWMgrPort( &systemPort );		//システムポートを取り出す
	SetPort( systemPort );			//システムポートをカレントポートに設定
	GetClip( saveSystemClip );		//システムポートのクリップ領域を取り出して保存
	
	
	//ウィンドウの枠の領域を得る
	GetWindowStructureRgn( window, dragRgn );
	CopyRgn( GetGrayRgn(), newSystemClip );
	
	/**************************************************************/
	//フローティングウィンドウの枠の部分を引く
	if( floatWindow != nil )
	{
		do
		{
			if( MacIsWindowVisible( floatWindow ) )
			{
				DiffRgn( newSystemClip, mtShowWindowStrucRgn( floatWindow ), newSystemClip );
			}

		}while( nil != ( floatWindow = mtGetNextFloatWindow( floatWindow )));
	}
	
	/**************************************************************/

	SetClip( newSystemClip );		//クリッピングする
	DisposeRgn( newSystemClip );		//もういらないので破棄
	

	//ドラッグ中を示す点線を表示
	afterDragArea = DragGrayRgn( dragRgn, mouse, &moveArea, &moveArea, noConstraint, 0 );

	SetClip( saveSystemClip );		//クリップ領域を元に戻す
	DisposeRgn( saveSystemClip );		//もういらないので破棄

	//マウスが指定した領域・moveArea・からはみ出ていなければ
	if( afterDragArea != kMouseUpOutOfSlop )	
	{
		Rect		contentRect;

		//ウィンドウのContentRgn得た後にcontentRectに代入
		GetWindowRegion( window, kWindowContentRgn, dragRgn );
		contentRect = (*dragRgn)->rgnBBox;

		mouse.v = HiWord(afterDragArea);
		mouse.h = LoWord(afterDragArea);

		mouse.v += contentRect.top;
		mouse.h += contentRect.left;

		MoveWindow( window, mouse.h, mouse.v, false );
	}
	DisposeRgn( dragRgn );

	SetPort( floatWindow );
}
//標準ウィンドウのアクティブウィンドウの交替
void priChangeDocWindow( WindowPtr theWindow )
{
	if( theWindow != nil )
	{
		if( IsWindowHilited( theWindow ) == false )
		{
			HiliteWindow( theWindow, true );		//アクティブ表示にする
		}
		
		WindowPtr	activeWindow = mtGetActiveDocWindow();
		
		if( activeWindow != nil )
		{
			if( theWindow != activeWindow )
			{
				HiliteWindow( activeWindow, false );	//アクティブだったウィンドウのハイライトを消す
				
				BringToFront( theWindow );			//ウィンドウを前に持ってくる
				mtActivateDeactivateCtrl( theWindow, true );	//ウィンドウのコントロールをアクティブ表示に
				priBringFWindow();
				
				mtActivateDeactivateCtrl( activeWindow, false );	//アクティブだったウィンドウのコントロールをディアクティブ表示に
			}
		}
	}
}

/**********************アイコンウィンドウをクリック⇩************************/

/********************フローティングウィンドウのイベント処理⇧********************/
