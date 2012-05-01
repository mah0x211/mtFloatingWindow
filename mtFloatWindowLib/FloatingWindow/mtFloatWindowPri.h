

/*************************** Utility関数 *************************/

Boolean priCanGetThisWindowFeatureRgn( WindowPtr theWindow, WindowRegionCode typeCode );	//typeCodeが指すRgnをtheWindowが持っているか調べる
Boolean priIsFloatOSShowFlag( WindowPtr theWindow );	//OSEvent時に表示・非表示のフラグを返す
void priSetFloatOSShowFlag( WindowPtr theWindow, Boolean showFlag );	//OSEvent時に表示・非表示のフラグを設定する
Boolean priIsFloatWindow( WindowPtr theWindow );	//theWindowがFloatingWindowかを調べる
Boolean priIsFloatWindowUseTitleBar( WindowPtr theWindow );	//theWindowがFloatingWindowかをTitleBarの幅を使って調べる

void priBringFWindow( void );		//フローティングウィンドウを前に持ってくる
Boolean priIsClickFWindow( WindowPtr window );	//FloatingWindowをクリックしたか調べる

void priActivateCtrl( WindowPtr theWindow );	//ActivateControl処理
void priDeactivateCtrl( WindowPtr theWindow );	//DeactivateControl処理

Boolean priIsFWindowProcID( const short procID );	//procIDがFloatingWindowのprocIDかをチェック
mtFWDataPtr priNewFloatData( void );		//FloatingWindowDataを新しく作る
void priCheckWindowRect( WindowPtr theWindow, Rect maxSize );	//PortRectのチェック
void priSettingWindowData( Boolean isFloatWindow, WindowPtr theWindow );	//FloatingWindowのrefConの設定

//_______________________ ドラッグ _____________________________//
void priClickFWDrag( WindowPtr window, Point mouse );	//フローティングウィンドウのドラッグ
	
void priDragDocWindow( WindowPtr window, Point mouse );		//標準ウィンドウのドラッグ処理
void priChangeDocWindow( WindowPtr window );	//標準ウィンドウのドラッグ後のActive・Deactive処理
//__________________________________________________________//


void priWindowGrow( WindowPtr window, Point clickPt, GetZoomGrowSizeData zoom_growSize );	//標準ウィンドウ・サイズ処理
void priFWindowGrow( WindowPtr theWindow, Point clickPt );	//フローティングウィンドウ・サイズ処理

void priWindowZoom( WindowPtr theWindow, short partCode, Point clickPt );	//標準ウィンドウ・ズームボックス処理
void priWindowGoAway( WindowPtr theWindow, Point clickPt, GoAwayRoutine userRoutine );	//標準ウィンドウ・goAwayボックス処理

void priActivateWindow( Boolean isOSEvt );	//ActivateWindow処理（OSEvent処理を含む）
void priDeactivateWindow( Boolean isOSEvt );	//Deactivate処理（OSEvent処理を含む）
/**************************************************************/

//theWindowのスクロールアクションUPP
void priScrollWindowAction( ControlHandle theCtrl, ControlPartCode thePart );
//標準ScrollBar( 右部・下部 )の移動とサイズ変更
void priStandardMoveScrlBar( WindowPtr theWindow, Rect oldPortRect );
//Controlの位置とサイズを新しいRect(winRect == theWindow->PortRect)に合わせる
void priMoveSizeScrlBar( ControlHandle theScrl, Rect winRect, Rect oldPortRect,
							short growIconSize );
//標準ScrollBar( 右部・下部 )のControlHandleを得る
ControlHandle priGetStandardScrlBar( WindowPtr theWindow, Rect oldPortRect, short kind );
void priSetWindowScrlUnit( ControlHandle theScrl, mtScrlUnitPtr scrlData  );	//Windowのスクロール時の単位と種類の設定
void priSetScrlBarMax( ControlHandle theScrl, mtWindowScrlUnit scrlData );	//標準ScrollBar( 右部・下部 )のMaxValueの変更
//theScrlのスクロールのRectの取得
Rect priGetScrlRect( ControlHandle theScrl );
//theScrlのスクロールのRectの設定
void priSetScrlRect( ControlHandle theScrl, Rect scrlRect );
//Controlの位置を新しいRect(winRect == theWindow->PortRect)に合わせる
void priMoveScrlBar( WindowPtr theWindow, Rect winRect, Rect oldPortRect, short kind );
