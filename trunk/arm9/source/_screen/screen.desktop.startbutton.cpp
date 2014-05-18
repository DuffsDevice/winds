#include "_screen/screen.desktop.startbutton.h"
#include "_type/type.gadget.helpers.h"
#include "_controller/controller.font.h"
#include "_controller/controller.localization.h"
#include "_controller/controller.gui.h"

//! Graphics
#include "_resource/resource.image.windows.startbutton.h"


_callbackReturn _desktopScreenStartButton::mouseHandler( _event event ){
	
	this->startMenu->toggle( this->getAbsoluteX() , this->getAbsoluteY() , false );
	
	// Let the button handle the rest of the mouseDown event
	return handled;
}


_callbackReturn _desktopScreenStartButton::refreshHandler( _event event )
{
	const string& sBT = _localizationController::getBuiltInString( "lbl_start_btn" );
	
	// Receive Gadget
	_desktopScreenStartButton* that = event.getGadget<_desktopScreenStartButton>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	if( that->isPressed() || that->startMenu->isOpened() )
		bP.copy( 0 , 0 , BMP_StartButtonPressed() );
	else
		bP.copy( 0 , 0 , BMP_StartButton() );
	
	// "Start"-Text
	bP.drawString( 12 , 0 , _fontController::getStandardFont() , sBT , _guiController::getStartButtonTextColor() );
	
	return use_default;
}


_desktopScreenStartButton::_desktopScreenStartButton( _optValue<_coord> x , _optValue<_coord> y , _style&& style ) :
	_button( x , y , 38 , 10 , "" , style | _style::notFocusable | _style::canNotTakeFocus )
	, startMenu( new _desktopScreenStartMenu() )
{
	this->setInternalEventHandler( onMouseClick , make_callback( this , &_desktopScreenStartButton::mouseHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_desktopScreenStartButton::refreshHandler ) );
	
	// Register Handlers to change the startButton to 'pressed' or to 'closed'
	this->startMenu->setInternalEventHandler( onOpen , _gadgetHelpers::eventForwardRefreshGadget( this ) );
	this->startMenu->setInternalEventHandler( onClose , _gadgetHelpers::eventForwardRefreshGadget( this ) );
	
	// Refresh
	this->redraw();
}

_desktopScreenStartButton::~_desktopScreenStartButton(){
	delete this->startMenu;
}