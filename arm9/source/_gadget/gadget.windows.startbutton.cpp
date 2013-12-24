#include "_gadget/gadget.windows.startbutton.h"
#include "_type/type.system.h"

//! Graphics
#include "_resource/resource.image.windows.startbutton.h"


_callbackReturn _windowsStartButton::mouseHandler( _event event ){
	
	this->startMenu->toggle( this->getAbsoluteX() , this->getAbsoluteY() );
	
	// Let the button handle the rest of the mouseDown event
	return handled;
}


_callbackReturn _windowsStartButton::refreshHandler( _event event )
{
	string sBT = _system::getLocalizedString( "lbl_start_btn" );
	
	// Receive Gadget
	_windowsStartButton* that = event.getGadget<_windowsStartButton>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	if( that->isPressed() || that->startMenu->isOpened() )
		bP.copy( 0 , 0 , BMP_StartButtonPressed() );
	else
		bP.copy( 0 , 0 , BMP_StartButton() );
	
	// "Start"-Text
	bP.drawString( 12 , 1 , _system::getFont() , sBT , _system::getUser().sBTC );
	
	return use_default;
}


_windowsStartButton::_windowsStartButton( _optValue<_coord> x , _optValue<_coord> y , _style&& style ) :
	_button( x , y , 38 , 10 , "" , style | _styleAttr::canNotReceiveFocus | _styleAttr::canNotTakeFocus )
	, startMenu( new _startMenu() )
{
	this->setInternalEventHandler( onMouseClick , make_callback( this , &_windowsStartButton::mouseHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_windowsStartButton::refreshHandler ) );
	
	// Register Handlers to change the startButton to 'pressed' or to 'closed'
	this->startMenu->setInternalEventHandler( onOpen , _gadgetHelpers::eventForwardRefreshGadget( this ) );
	this->startMenu->setInternalEventHandler( onClose , _gadgetHelpers::eventForwardRefreshGadget( this ) );
	
	// Refresh
	this->redraw();
}