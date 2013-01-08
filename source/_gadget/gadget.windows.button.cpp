#include "_gadget/gadget.windows.button.h"
#include "_type/type.system.h"

//! Graphics
#include "_resource/BMP_StartButton.h"

//
//  Startbutton
//

_bitmap _windowsStartButton::startButton = BMP_StartButton();
_bitmap _windowsStartButton::startButtonPressed = BMP_StartButtonPressed();

_callbackReturn _windowsStartButton::mouseHandler( _event event ){
	
	// Refresh the startButton if the _startMenu will be closed
	if( event.getType() == onClose )
	{
		this->bubbleRefresh( true );
		return handled;
	}
	
	this->startMenu->toggle( this->getAbsoluteX() , this->getAbsoluteY() );
	
	return _button::mouseHandler( event );

}

_callbackReturn _windowsStartButton::refreshHandler( _event event )
{
	static string sBT = _system::_runtimeAttributes_->user->getStrAttr( "startButtonText" );
	
	// Receive Gadget
	_windowsStartButton* that = event.getGadget<_windowsStartButton>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	if( that->isPressed() || that->startMenu->isOpened() )
		bP.copy( 0 , 0 , that->startButtonPressed );
	else
		bP.copy( 0 , 0 , that->startButton );
	
	// "Start"-Text
	bP.drawString( 12 , 1 , _system::getFont() , sBT , _system::_runtimeAttributes_->user->sBTC );
	
	return use_default;
}

_windowsStartButton::_windowsStartButton( _coord x , _coord y , _style style ) :
	_button( 38 , 10 , x , y , "" , style )
	, startMenu( new _startMenu() )
{
	_style st = this->getStyle();
	st.canTakeFocus = false;
	st.canReceiveFocus = false;
	this->setStyle( st );
	this->registerEventHandler( onAction , new _classCallback( this , &_windowsStartButton::mouseHandler ) );
	this->registerEventHandler( refresh , new _staticCallback( &_windowsStartButton::refreshHandler ) );
	this->startMenu->registerEventHandler( onClose , new _classCallback( this , &_windowsStartButton::mouseHandler ) );
	
	this->refreshBitmap();
}

//
// Taskbar-Window-Button
//

_callbackReturn _windowsTaskButton::mouseHandler( _event event ){
	
	// Receive Gadget
	_windowsTaskButton* that = event.getGadget<_windowsTaskButton>();
	
	if( that->reference->isMinimized() )
	{
		that->reference->restore();
		that->reference->handleEvent( focus );
	}
	else if( that->reference->hasFocus() )
		that->reference->minimize();
	else
		that->reference->handleEvent( focus );
	
	return _button::mouseHandler( event );

}

_callbackReturn _windowsTaskButton::refreshHandler( _event event )
{
	// Receive Gadget
	_windowsTaskButton* that = event.getGadget<_windowsTaskButton>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	//if( that->isPressed() || that->startMenu->isOpened() )
	//	bP.copy( 0 , 0 , that->startButtonPressed );
	//else
	//	bP.copy( 0 , 0 , that->startButton );
	
	_length myW = bP.getWidth();
	_length myH = bP.getHeight();
	
	if( that->reference->hasFocus() )
	{
		bP.fill( _system::_runtimeAttributes_->windowBar[8] );
		
		// Topper Line Bright
		bP.drawHorizontalLine( 1 , 0 , myW - 2 , _system::_runtimeAttributes_->windowBar[1] );
		
		// Top Line Dark
		bP.drawHorizontalLine( 1 , 1 , myW - 2 , _system::_runtimeAttributes_->windowBar[9] );
		
		// Bottom Dark Line
		bP.drawHorizontalLine( 0 , myH - 1 , myW - 1 , _system::_runtimeAttributes_->windowBar[9] );
		
		// Left Dark Line
		bP.drawVerticalLine( 0 , 2 , myH - 3 , _system::_runtimeAttributes_->windowBar[9] );
		
		// Right Dark Line
		bP.drawVerticalLine( myW - 1 , 2 , myH - 3 , _system::_runtimeAttributes_->windowBar[9] );
	}
	else
	{
		bP.fill( _system::_runtimeAttributes_->windowBar[7] );
		
		// Top Line Dark
		bP.drawHorizontalLine( 1 , 0 , myW - 2 , _system::_runtimeAttributes_->windowBar[8] );
		
		// Top+1 Line Bright
		bP.drawHorizontalLine( 1 , 1 , myW - 2 , _system::_runtimeAttributes_->windowBar[1] );
		
		// Bottom Dark Line
		bP.drawHorizontalLine( 0 , myH - 1 , myW - 1 , _system::_runtimeAttributes_->windowBar[9] );
		
		// Left Line Dark
		bP.drawVerticalLine( 0 , 2 , myH - 3 , _system::_runtimeAttributes_->windowBar[8] );
		
		// Left+1 Line Bright
		bP.drawVerticalLine( 1 , 2 , myH - 3 , _system::_runtimeAttributes_->windowBar[1] );
		
		// Right Dark Line
		bP.drawVerticalLine( myW - 1 , 2 , myH - 3 , _system::_runtimeAttributes_->windowBar[3] );
	}
	
	// Corners!
	//bP.drawPixel( myW - 1 , myH - 1 , NO_COLOR );
	//bP.drawPixel( myW - 1 , 0 , NO_COLOR );
	//bP.drawPixel( 0 , 0 , NO_COLOR );
	//bP.drawPixel( 0 ,  myH - 1 , NO_COLOR );
	
	return use_default;
}

_windowsTaskButton::_windowsTaskButton( _coord x , _coord y , _window* reference , _style style ) :
	_button( 20 , 10 , x , y , reference->getStrValue() , style )
	, reference( reference )
{
	_style st = this->getStyle();
	st.canTakeFocus = false;
	this->setStyle( st );
	this->label->setColor( RGB( 27 , 27 , 27 ) );
	this->setPadding( _padding( 2 , 0 , 2 , 1 ) );
	this->setAlign( _align::left );
	this->registerEventHandler( onAction , new _staticCallback( &_windowsTaskButton::mouseHandler ) );
	this->registerEventHandler( refresh , new _staticCallback( &_windowsTaskButton::refreshHandler ) );
	
	this->refreshBitmap();
}