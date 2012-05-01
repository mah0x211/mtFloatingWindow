//__________________________________________//
//											//
//	FloatingWindow Function							//
//											//
//											//
//__________________________________________//
//											//

#ifndef		____MTFLOATWINDOW____
#define		____MTFLOATWINDOW____

#define GetWidth( r )		( (r).right - (r).left )
#define GetHeight( r )		( (r).bottom - (r).top )

//ウィンドウスクロール時のアップデート関数へのポインタ
typedef	void (*UpdateRoutine )( WindowPtr theWindow, RgnHandle upRgn );
//Scroll Unit//Windowのスクロール時の単位と種類
typedef struct mtWindowScrlUnit
{
	short			unit;			//1単位
	short			onePageUnit;	//
	short			moveKind;
	Rect			scrlRect;		//内部で使用
	UpdateRoutine	userRoutine;
	long			refCon;

}mtWindowScrlUnit;
typedef mtWindowScrlUnit*	mtScrlUnitPtr;


typedef struct mtFloatData
{
	long				floatingKind;
	short				floatNumber;	//FloatingWindowの作られた順番の番号
	Boolean			osEvtShowFlag;	//OSEvent時に表示・非表示のフラグ
	long				userRefCon;		//userが使用するRefCon

}mtFloatData;
typedef mtFloatData*		mtFWDataPtr;

//GrowWindowの時に使用
typedef struct mtWindowSizeData
{
	short		minWidth;		// 横幅の最小値
	short		minHeight;		// 縦幅の最小値
	Rect		widthSize;		//どれ位ずつ大きくするか、
						// top = 縦 ・ left = 横
						// right・bottom = plusα(スクロールバーの分など)

}mtWindowSizeData;

#define		floatKind		6121520

// mt・・・ = public
// pri・・・ = private
// pro・・・ = protected


#define	inFloatGoAway		-6		//フローティングウィンドウのGoAwayBox
#define	inFloatContent			-3	//フローティングウィンドウのContent
#define	inDeactiveWindow		-9		//ウィンドウの切り替え

//FloatingWindowのDispose処理時のUserRoutine
typedef	void ( *FWClearRoutine )( long* refCon );
//GoAwayBoxをClick時の処理
typedef	Boolean ( *GoAwayRoutine )( short thePart, Boolean goAwayFlag, WindowPtr theWindow );
//FloatingWindowのUpDate処理時のUserRoutine
typedef	void ( *UpDateRoutine )( WindowPtr theFloatWindow );
//ScrlBarMovAndSizChangeRoutine
typedef	void ( *ScrlMoveSizeRoutine )( WindowPtr theWindow, Rect oldWindowPortRect );
//WindowのZoomとGrow時のGetZoomGrowSizeDataRoutine
typedef	mtWindowSizeData ( *GetZoomGrowSizeData )( void );

/********************* Utility **********************************************/

/************************
*	Get FloatWindow Lot Number	*
************************/
short mtGetFloatWindowNumber( WindowPtr theFloatWindow );	//theFloatWindowの作られた番号を返す（FloatingWindowで無ければ0を返す）


/************************
*	Control Activate Deactivate	*
************************/
void mtActivateDeactivateCtrl( WindowPtr theWindow, Boolean isActivate );	//ControlのActivate・Deactivate処理
ControlHandle mtGetWindowCtrlList( WindowPtr theWindow );	//theWindowにあるControlList( 先頭のControlHandleへのポインタ )を得る


/************************
*	Set Get RefCon			*
************************/
void mtSetFWRefCon( WindowPtr theFloatWindow, long data );	//FloatWindowのRefConを設定する
long mtGetFWRefCon( WindowPtr theFloatWindow );	//FloatWindowのRefConを取得する


/************************
*	Window Clipping			*
************************/
Rect mtSetClipRect( WindowPtr theWindow, Rect clipArea );	//theWindowをclipAreaでクリッピングして、クリッピングされていたRectを返す
Rect mtGetClipRect( WindowPtr theWindow );	//theWindowのClipRgnのRectを得る
void mtSetClipRgn( WindowPtr theWindow, RgnHandle clipArea );		//theWindowをclipAreaでクリッピングして、clipAreaをクリッピングされていたRegionにする

/************************
*	ShowWindowRgn			*
************************/
RgnHandle mtShowWindowContentRgn( WindowPtr theWindow );	//theWindowのContentRgnを返す
RgnHandle mtShowWindowStrucRgn( WindowPtr theWindow );	//theWindowのSrucRgnを返す
RgnHandle mtShowWindowUpdateRgn( WindowPtr theWindow );	//theWindowのUpdateRgnを返す
Rect mtGetWindowMaxPortRect( WindowPtr theWindow );		//theWindow->portRectの最大サイズを得る


/************************
*	GetWindowRgnRect		*
************************/
Rect mtGetWindowContentRect( WindowPtr theWindow );		//theWindowのContentRectを返す
Rect mtGetWindowStrucRect( WindowPtr theWindow );		//theWindowのSrucRectを返す



/************************
*	Get WindowFeature Region	*
************************/
enum{	//const short widthCode
		rgnHeight = 1,
		rgnWidth
};
short mtGetWindowFeatureRgnWidth( WindowPtr theWindow, WindowRegionCode typeCode, const short widthCode );	//theWindowのtypeCodeで指定したRgnの幅( か高さ )を返す


/************************
*	FloatingWindow Show Hide	*
************************/
void mtShowHideFWindow( WindowPtr theWindow, Boolean isShow );	//ウィンドウの表示制御
void mtAllShowHideFWindow( Boolean isShow );		//全てのフローティングウィンドウを表示・非表示
short mtAutoShowHideFWindow( WindowPtr theWindow );	//フローティングウィンドウを表示・非表示を自動的に決める	/*表示したなら1を返す FloatingWindowでは無いなら-1*/
enum{
		sFWindow = 1,
		hFWindow = 2,
		noFWindow = -1
};


/************************
*	Find FloatingWindow		*
************************/
WindowPtr mtGetFirstFloatWindow( void );	//一番前にあるFloatingWindowを探す（無ければnilを返す）
WindowPtr mtGetNextFloatWindow( WindowPtr floatWindow ); //floatWindowの次にくるFloatingWindowを返す（無ければnilを返す）
WindowPtr mtGetFWinWithNum( short windowNumber );	//FloatingWindowの作られた番号からFloatingWindowを探して返す（無ければnilを返す）



/************************
*	Find Active DocWindow		*
************************/
WindowPtr mtGetActiveDocWindow( void );	//現在アクティブな(FloatingWindowのすぐ後ろの)ドキュメントウィンドウを返す

/******************************************************************************/


/********************* フローティングウィンドウの作成 と破棄*******************/

/************************
*	NewWindow				*
************************/
WindowPtr mtNewWindow( short procID, Rect winRect, Boolean closeBox, Str255 title, const Rect windowMaxSize, Boolean visble );	//ウィンドウ・フローティングウィンドウの作成	
WindowPtr mtGetNewWindow( short resourceID, const Rect windowMaxSize, Boolean isFloat );	//ウィンドウ・フローティングウィンドウのリソースからの作成

/************************
*	Dispose FloatWindow		*
************************/
void mtClearAllFloatWindow( FWClearRoutine userRoutine );	//全てのFloatingWindowのDispose
void mtClearFloatWindow( WindowPtr theWindow, FWClearRoutine userRoutine );	//FloatingWindowのDispose
void mtClearNumOfFloatWindow( short number, FWClearRoutine userRoutine );	//numberで指定した番号のFloatingWindowのDispose

/******************* ウィンドウ・フローティングウィンドウのイベント ******************/

/********************************
*	Window・FloatWindow Mouse Event		*
********************************/
//返り値 = { inMenuBar, inContent, inFloatContent, inCollapseBox, inSysWindow
//theWindow = 処理対象のWindow
//thePart = FindWindowの返り値
short mtMouseEventFloat( WindowPtr theWindow, short thePart, Point clickPt, GoAwayRoutine goAwayRoutine, GetZoomGrowSizeData zoom_growSize );

/************************
*	FloatWindow Update		*
************************/
Boolean mtUpdateIsFWindow( WindowPtr theWindow, UpDateRoutine userRoutine );	//フローティングウィンドウのアップデート
void mtActiveDeactiveWindow( Boolean active_deactive, Boolean isOSEvt );	//ウィンドウのアクティベート・ディアクティベート


/********************* theWindowのScroll *******************/


/*mtTrackWindow ScrlBarKind*/
enum{
		vScrl = 4,	//Vertical Scroll
		hScrl = 8	//Horizontal Scroll
};

//scrlBarKind = スクロールバーの種類⇧
//縦スクロールの時はwidthは無視、横スクロールの時はheightは無視
ControlHandle mtNewStandardScrollBar( WindowPtr theWindow, Boolean visible,
								const short height, const short width, const short scrlBarKind );

//theWindowに設定されているスクロールバーのDispose
void mtDisposeScrllBarData( WindowPtr theWindow );

//ScrollBarのTracking
short mtTrackWindowScrlBar( ControlHandle theCtrl, Point clickPt, ControlPartCode thePart,
					Rect clipArea, ControlActionUPP userAction );

//theWindowのScroll(原点の移動)
void mtScrollWindow( WindowPtr theWindow, short vMove, short hValue );

//theScrlのスクロール時の単位と種類の設定
void mtSetScrlUnit( ControlHandle theScrl, short unit, short oneUnit, short moveKind, UpdateRoutine userRoutine );

mtWindowScrlUnit mtGetWindowScrlUnit( ControlHandle theScrl );	//Windowのスクロール時の単位と種類の取得
//theScrlのスクロールの種類の取得(mtTrackWindowScrlBarKind)
short mtGetScrlMoveKind( ControlHandle theScrl );

//標準ScrollBar( 右部・下部 )の描画
void mtDrawScrlBar( WindowPtr theWindow );

/*************************************************************/

#endif

/*

//theWindowのスクロールアクションUPP
void priScrollWindowAction( ControlHandle theCtrl, ControlPartCode thePart )
{
	WindowPtr	theWindow = (*theCtrl)->contrlOwner;
	
	if( theWindow != nil )
	{
		SetPort( theWindow );
		
		mtWindowScrlUnit	scrlData = mtGetWindowScrlUnit( theCtrl );
		const short		ctrlValue = GetControlValue( theCtrl );
		short			moveValue = 0;
		Rect				portRect = theWindow->portRect;
		short			moveKind = scrlData.moveKind;
		short			unit = scrlData.unit;
		short			oneUnit = scrlData.onePageUnit;

		
		switch( thePart )
		{
			case( kControlUpButtonPart ) :
				moveValue = -unit;
			break;
			
			case( kControlDownButtonPart ) :
				moveValue = unit;
			break;
			
			case( kControlPageUpPart ) :
				moveValue = -oneUnit;
			break;
			
			case( kControlPageDownPart ) :
				moveValue = oneUnit;
			break;

		};
		
		SetControlValue( theCtrl, ctrlValue + moveValue );
		
		short		maxValue = GetControlMaximum( theCtrl );
	
		if( ( ctrlValue == 0 and moveValue < 0 ) or
			( ctrlValue == maxValue and moveValue > 0 )
		   )
		{
			moveValue = 0;	
		}
		
		if( moveValue != 0 )
		{
			short	vValue = 0;
			short	hValue = 0;
			Rect		scrlArea = scrlData.scrlRect;	//theScrlのスクロールのRectの取得
			Rect		saveClip = mtGetClipRect( theWindow );
			
			if( moveKind == vScrl ){		vValue = moveValue;	}
			else if( moveKind == hScrl ){	hValue = moveValue;	}

			//まず、scrlAreaの左上をportRectの左上に合わせる
			OffsetRect( &scrlArea, portRect.left - scrlArea.left, portRect.top - scrlArea.top );
			//scrlAreaを動かす分だけ先に移動させておく
			OffsetRect( &scrlArea, hValue, vValue );
			//その矩形をクリップ
			ClipRect( &scrlArea );
			//スクロールさせる
			mtScrollWindow( theWindow, vValue, hValue );
			ScrollRect( &scrlArea, -hValue, -vValue, nil );
			
			ClipRect( &saveClip );
		}
	}
}


*/