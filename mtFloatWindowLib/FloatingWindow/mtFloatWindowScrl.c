#ifndef		____MTFLOATWINDOW____
#include"mtFloatWindow.h"
#endif

#include"mtFloatWindowPri.h"

#pragma mark - CREATE ・ DISPOSE SCROLL BAR


//WindowにScrollBarをつける
ControlHandle mtNewStandardScrollBar( WindowPtr theWindow, Boolean visible,
								const short height, const short width, const short scrlBarKind )
{
	ControlHandle	scrllBar = nil;
	short		growIconWidth = 0;
	
	growIconWidth = mtGetWindowFeatureRgnWidth( theWindow, kWindowGrowRgn, nil );
	
	//theWindowがnilでは無くてGrowIconがtheWindowの付いているなら
	if( theWindow != nil and growIconWidth != 0 )
	{
		Rect				ctrlRect = theWindow->portRect;
		Rect				theWindowMaxSize = mtGetWindowMaxPortRect( theWindow );	//theWindowの最大サイズ
		short			min = 0;
		short			max = 0;
		unsigned short		value = 0;
		short			size = growIconWidth + 2;

		if( scrlBarKind == vScrl )	//垂直
		{
			value = ctrlRect.top;
			if( ctrlRect.top < 0 ){	value = 0;		}
			
			max = theWindowMaxSize.bottom - theWindow->portRect.bottom;
			
			ctrlRect.top = -1 + height + ctrlRect.top;
			ctrlRect.left = ctrlRect.right - size + 1;
			ctrlRect.right = ctrlRect.left + size;
			ctrlRect.bottom -= growIconWidth;
		}
		if( scrlBarKind == hScrl )	//　水平
		{
			value = ctrlRect.left;
			if( ctrlRect.left < 0 ){	value = 0;		}
			
			max = theWindowMaxSize.right - theWindow->portRect.right;

			ctrlRect.top = ctrlRect.bottom - size + 1;
			ctrlRect.left = -1 + width + ctrlRect.left;
			ctrlRect.right -= growIconWidth;
			ctrlRect.bottom = ctrlRect.top + size;
		}
		scrllBar = NewControl( theWindow, &ctrlRect, "\p", visible, value, min, max, kControlScrollBarProc, 0 );
	}
	return( scrllBar );
}

//theWindowに設定されているスクロールバーのDispose
void mtDisposeScrllBarData( WindowPtr theWindow )
{
	if( theWindow != nil )
	{
		mtScrlUnitPtr		scrlData = nil;
		ControlHandle		theScrl = nil;
		Size				size = 0;
		
		theScrl = priGetStandardScrlBar( theWindow, theWindow->portRect, vScrl );
		if( theScrl != nil )		//右に設定されているスクロールバー
		{
			scrlData = (mtScrlUnitPtr)GetControlReference( theScrl );
			size = GetPtrSize( (Ptr)scrlData );
			
			if( size > 0 )
			{
				delete	scrlData;
			}
		}
		
		theScrl = priGetStandardScrlBar( theWindow, theWindow->portRect, hScrl );
		if( theScrl != nil )		//下に設定されているスクロールバー
		{
			scrlData = (mtScrlUnitPtr)GetControlReference( theScrl );
			size = GetPtrSize( (Ptr)scrlData );
			
			if( size > 0 )
			{
				delete	scrlData;
			}
		}
	}
}

#pragma mark -
#pragma mark === GET SET mtWindowScrlUnit

//theScrlのスクロールの種類の取得
short mtGetScrlMoveKind( ControlHandle theScrl )
{
	short	result = 0;
	
	if( theScrl != nil )
	{
		result = ((mtScrlUnitPtr)GetControlReference( theScrl ))->moveKind;
	}
	
	return( result );
}

//theScrlのスクロールのRectの取得
Rect priGetScrlRect( ControlHandle theScrl )
{
	Rect		result = { 0 };
	
	if( theScrl != nil )
	{
		result = ((mtScrlUnitPtr)GetControlReference( theScrl ))->scrlRect;
	}
	
	return( result );
}

//theScrlのスクロールのRectの設定
void priSetScrlRect( ControlHandle theScrl, Rect scrlRect )
{
	if( theScrl != nil )
	{
		((mtScrlUnitPtr)GetControlReference( theScrl ))->scrlRect = scrlRect;
	}
}


//theScrlのスクロール時の単位と種類の設定
void mtSetScrlUnit( ControlHandle theScrl, short unit, short oneUnit, short moveKind, UpdateRoutine userRoutine )
{
	if( theScrl != nil )
	{
		mtScrlUnitPtr	 scrlData = new mtWindowScrlUnit;
		
		if( theScrl != nil )
		{
			scrlData->unit = unit;
			scrlData->onePageUnit = oneUnit;
			scrlData->moveKind = moveKind;
			scrlData->userRoutine = userRoutine;
			scrlData->refCon = 0;
			
			priSetScrlBarMax( theScrl, *scrlData );
			
			priSetWindowScrlUnit( theScrl, scrlData );
		}
	}
}

//theScrlのmtWindowScrlUnitをのReConに設定
void priSetWindowScrlUnit( ControlHandle theScrl, mtScrlUnitPtr scrlData  )
{
	if( theScrl != nil )
	{
		SetControlReference( theScrl, (long)scrlData );
	}
}

//theScrlのスクロール時の単位と種類の取得
mtWindowScrlUnit mtGetWindowScrlUnit( ControlHandle theScrl )
{
	mtScrlUnitPtr	result = { 0 };
	
	if( theScrl != nil )
	{
		result = (mtScrlUnitPtr)GetControlReference( theScrl );
	}
	
	return( *result );
}

#pragma mark -
#pragma mark === WINDOW SCROLL

//theWindowのScroll(原点の移動)
void mtScrollWindow( WindowPtr theWindow, short vValue, short hValue )
{
	if( theWindow != nil )
	{
		WindowPtr	savePort = nil;
		Rect			oldPortRect = theWindow->portRect;
		Rect			portRect = theWindow->portRect;
		short		left = portRect.left + hValue;
		short		top = portRect.top + vValue;
		short		kind = 0;

		GetPort( &savePort );
		SetPort( theWindow );
		
		SetOrigin( left, top );
	
		SetPort( savePort );
				
		if( vValue != 0 ){	kind = vScrl;	}
		else if( hValue != 0 ){	kind = hScrl;	}
		
		if( kind != 0 )
		{
			priMoveScrlBar( theWindow, theWindow->portRect, oldPortRect, kind );
		}
	}
}

//ScrollBarのTracking
short mtTrackWindowScrlBar( ControlHandle theCtrl, Point clickPt,
						ControlPartCode thePart, Rect clipArea,
						ControlActionUPP userAction )
{
	short			moveValue = 0;
	ControlActionUPP	scrollAction = nil;
	
	if( userAction != nil ){		scrollAction = userAction;	}
	else{		scrollAction = NewControlActionProc( priScrollWindowAction );	}
	
	if( theCtrl != nil )
	{
		WindowPtr			theWindow = ( *theCtrl )->contrlOwner;
		short				ctrlValue = GetControlValue( theCtrl );
		short				vMove = 0;
		short				hMove = 0;
		mtWindowScrlUnit		scrlData = mtGetWindowScrlUnit( theCtrl );
		
		//theScrlのスクロールのRectの設定
		priSetScrlRect( theCtrl, clipArea );
		
		SetPort( theWindow );
		
		if( thePart == kControlIndicatorPart )
		{
			TrackControl( theCtrl, clickPt, nil );

			moveValue = GetControlValue( theCtrl );
			
			if( moveValue != ctrlValue )
			{
				if( scrlData.unit > 0 )
				{
					moveValue = short( moveValue / scrlData.unit ) * scrlData.unit;
				}
				
				SetControlValue( theCtrl, moveValue );
			}
			
			if( scrlData.moveKind == vScrl ){	vMove = moveValue = moveValue - ctrlValue;	}
			else if( scrlData.moveKind == hScrl ){	hMove = moveValue = moveValue - ctrlValue;	}
			
			if( moveValue != 0 )
			{
				Rect		portRect = theWindow->portRect;
				Rect		scrlArea = scrlData.scrlRect;	//theScrlのスクロールのRectの取得
				Rect		saveClip = mtGetClipRect( theWindow );
			
				//まず、scrlAreaの左上をportRectの左上に合わせる
				OffsetRect( &scrlArea, portRect.left - scrlArea.left, portRect.top - scrlArea.top );
				//scrlAreaを動かす分だけ先に移動させておく
				OffsetRect( &scrlArea, hMove, vMove );
				//その矩形をクリップ
				ClipRect( &scrlArea );
				
				mtScrollWindow( theWindow, vMove, hMove );
				if( scrlData.userRoutine != nil )
				{
					scrlData.userRoutine( theWindow, nil );
				}
				ClipRect( &saveClip );
			}
			
		}
		else
		{
			TrackControl( theCtrl, clickPt, scrollAction );	
			
			if( scrlData.moveKind == vScrl )
			{
				moveValue = vMove = GetControlValue( theCtrl ) - ctrlValue;
			}
			else if( scrlData.moveKind == hScrl )
			{
				moveValue = hMove = GetControlValue( theCtrl ) - ctrlValue;
			}
		}
	}
	if( scrollAction != nil ){	DisposeRoutineDescriptor( scrollAction );	}
	
	return( moveValue );
}

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
			short		vValue = 0;
			short		hValue = 0;
			Rect			scrlArea = scrlData.scrlRect;	//theScrlのスクロールのRectの取得
			Rect			saveClip = mtGetClipRect( theWindow );
			RgnHandle		upRgn = nil;
			
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
			ScrollRect( &scrlArea, -hValue, -vValue, upRgn );
			
			if( scrlData.userRoutine != nil ){	scrlData.userRoutine( theWindow, upRgn );		}
			ClipRect( &saveClip );
		}
	}
}


#pragma mark -
#pragma mark === SCROLLBAR IMAGING

//標準ScrollBar( 右部・下部 )の描画
void mtDrawScrlBar( WindowPtr theWindow )
{
	if( theWindow != nil )
	{
		ControlHandle		scrlBar = nil;
		
		scrlBar = priGetStandardScrlBar( theWindow, theWindow->portRect, vScrl );
		if( scrlBar != nil )		//右に設定されているスクロールバー
		{
			Draw1Control( scrlBar );
		}
		
		scrlBar = priGetStandardScrlBar( theWindow, theWindow->portRect, hScrl );
		if( scrlBar != nil )		//下に設定されているスクロールバー
		{
			Draw1Control( scrlBar );
		}
	}
}


#pragma mark -
#pragma mark === SCROLLBAR CHANGE SIZE & POSITION

//標準ScrollBar( 右部・下部 )のControlHandleを得る
ControlHandle priGetStandardScrlBar( WindowPtr theWindow, Rect oldPortRect, short kind )
{
	ControlHandle		theScrl = nil;
	
	if( theWindow != nil )
	{
		Point			testPt;
		short			growIconSize = mtGetWindowFeatureRgnWidth( theWindow, kWindowGrowRgn, nil ) + 5;
		ControlPartCode	thePart = 0;
		
		if( kind == vScrl )
		{
			testPt.h = oldPortRect.right;
			testPt.v = oldPortRect.bottom - growIconSize;
		}
		if( kind == hScrl )
		{
			testPt.h = oldPortRect.right - growIconSize;
			testPt.v = oldPortRect.bottom;
		}
		
		theScrl = FindControlUnderMouse( testPt, theWindow, &thePart );
		
		if( theScrl != nil )
		{
			if( mtGetScrlMoveKind( theScrl ) != kind )
			{
				theScrl = nil;
			}
		}
	}
	
	return( theScrl );
}



//Controlの位置とサイズを新しいRect(winRect == theWindow->PortRect)に合わせる
void priMoveSizeScrlBar( ControlHandle theScrl, Rect winRect, Rect oldPortRect,
							short growIconSize )
{
	if( theScrl != nil )
	{
		short	moveV = 0;
		short	moveH = 0;
		short	width = 0;
		short	height = 0;
		short	kind = mtGetScrlMoveKind( theScrl );
		short	topMove = winRect.top - oldPortRect.top;
		short	leftMove = winRect.left - oldPortRect.left;
		Rect		ctrlRect = (*theScrl)->contrlRect;
		
		
		if( kind == vScrl )
		{
			//Controlの幅
			width = ctrlRect.right - ctrlRect.left;
			
			//Controlの移動
			moveV = ctrlRect.top + topMove;
			moveH = winRect.right - width + 1;
			MoveControl( theScrl, moveH, moveV );
			
			//Controlk高さ( theWindowのBottomからコントロールのTopとGrowIconの幅の分だけ引く )
			height = winRect.bottom - moveV  - growIconSize;
			SizeControl( theScrl, width, height );
		}
		else if( kind == hScrl )
		{
			//Controlの幅
			height = ctrlRect.bottom - ctrlRect.top;
			
			//Controlの移動
			moveV = winRect.bottom - height + 1;
			moveH = ctrlRect.left + leftMove;
			MoveControl( theScrl, moveH, moveV );
			
			//Controlk高さ( theWindowのBottomからコントロールのTopとGrowIconの幅の分だけ引く )
			width = winRect.right - moveH - growIconSize;
			SizeControl( theScrl, width, height );
		}
	}
}

//標準ScrollBar( 右部・下部 )の移動とサイズ変更
void priStandardMoveScrlBar( WindowPtr theWindow, Rect oldPortRect )
{
	short	growIconSize = mtGetWindowFeatureRgnWidth( theWindow, kWindowGrowRgn, nil );
	
	if( theWindow != nil and growIconSize != 0 )
	{
		Rect				maxSize = mtGetWindowMaxPortRect( theWindow );
		Rect				winRect = theWindow->portRect;
		ControlHandle		scrlBar = nil;
		mtWindowScrlUnit	scrlData = { 0 };
		short			maxValue = 0;
		Rect				ctrlRect = { 0 };
		
		scrlBar = priGetStandardScrlBar( theWindow, oldPortRect, vScrl );
		
		if( scrlBar != nil )		//右に設定されているスクロールバー
		{
			scrlData = mtGetWindowScrlUnit( scrlBar );
			
			priMoveSizeScrlBar( scrlBar, winRect, oldPortRect, growIconSize );
			
			priSetScrlBarMax( scrlBar, scrlData );
		}
		
		
		scrlBar = priGetStandardScrlBar( theWindow, oldPortRect, hScrl );

		if( scrlBar != nil )		//下に設定されているスクロールバー
		{
			scrlData = mtGetWindowScrlUnit( scrlBar );
			
			priMoveSizeScrlBar( scrlBar, winRect, oldPortRect, growIconSize );
			
			priSetScrlBarMax( scrlBar, scrlData );
		}
	}
}


//標準ScrollBar( 右部・下部 )のMaxValueの変更
void priSetScrlBarMax( ControlHandle theScrl, mtWindowScrlUnit scrlData )
{
	if( theScrl != nil )		//右に設定されているスクロールバー
	{
		WindowPtr	theWindow = ( *theScrl )->contrlOwner;
		short	growIconSize = mtGetWindowFeatureRgnWidth( theWindow, kWindowGrowRgn, nil );
	
		if( theWindow != nil and growIconSize != 0 )
		{
			Rect				maxSize = mtGetWindowMaxPortRect( theWindow );
			Rect				winRect = theWindow->portRect;
			short			maxValue = 0;
			Rect				ctrlRect = (*theScrl)->contrlRect;
			
			
			if( scrlData.moveKind == vScrl )
			{
				short	ctrlHeight = ctrlRect.top - winRect.top;
				short	height = winRect.bottom - winRect.top;
				short	maxHeight = ( maxSize.bottom - maxSize.top ) - ctrlHeight - growIconSize;
				short	dive = maxHeight % scrlData.unit;
			
				maxValue = short( maxHeight / scrlData.unit ) * scrlData.unit + ctrlHeight - height + growIconSize + dive;
			
				SetControlMaximum( theScrl, maxValue );
			}
			else if( scrlData.moveKind == hScrl )
			{
				short	ctrlWidth = ctrlRect.left - winRect.left;
				short	width = winRect.right - winRect.left;
				short	maxWidth = ( maxSize.right - maxSize.left ) - ctrlWidth - growIconSize;
				short	dive = maxWidth % scrlData.unit;
			
				maxValue = short( maxWidth / scrlData.unit ) * scrlData.unit + ctrlWidth - width + growIconSize + dive;
			
				SetControlMaximum( theScrl, maxValue );
			}
		}
	}
}

//Controlの位置を新しいRect(winRect == theWindow->PortRect)に合わせる
void priMoveScrlBar( WindowPtr theWindow, Rect winRect, Rect oldPortRect, short kind )
{
	ControlHandle		theScrlRight = priGetStandardScrlBar( theWindow, oldPortRect, vScrl );
	ControlHandle		theScrlBottom = priGetStandardScrlBar( theWindow, oldPortRect, hScrl );
	
	if( theScrlRight != nil and theScrlBottom != nil )
	{
		short	topMove = winRect.top - oldPortRect.top;
		short	leftMove = winRect.left - oldPortRect.left;
		Rect		rightRect = (*theScrlRight)->contrlRect;
		Rect		bottomRect = (*theScrlBottom)->contrlRect;

		EraseRect( &rightRect );
		EraseRect( &bottomRect );
		
		if( kind == vScrl )
		{
			
			//Controlの移動 right
			OffsetRect( &rightRect, 0, topMove );
			(*theScrlRight)->contrlRect = rightRect;
			
			//Controlの移動 bottom
			OffsetRect( &bottomRect, 0, topMove );
			(*theScrlBottom)->contrlRect = bottomRect;
		}
		else if( kind == hScrl )
		{
			//Controlの移動 right
			OffsetRect( &rightRect, leftMove, 0 );
			(*theScrlRight)->contrlRect = rightRect;
			
			//Controlの移動 bottom
			OffsetRect( &bottomRect, leftMove, 0 );
			(*theScrlBottom)->contrlRect = bottomRect;
		}
	}
}