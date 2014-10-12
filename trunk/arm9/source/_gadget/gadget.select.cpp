#include <_gadget/gadget.select.h>
#include <_controller/controller.font.h>
#include <_controller/controller.gui.h>

_select::_select( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , const _menuEntryList& lst , _int initialValue , _style&& style ) :
	_gadget( _gadgetType::selectbox , x , y , width , _guiController::getSelectHeight() , (_style&&)style )
	, contextMenu( new _contextMenu( ignore , move(lst) , this , true , initialValue ) )
	, arrow( new _scrollButton( 8 , 8 , 0 , 0 , _scrollButtonType::buttonBottom ) )
{
	// Refresh on contextMenu change
	this->contextMenu->setUserEventHandler( onEdit , make_callback( this , &_select::editHandler ) );
	this->contextMenu->setUserEventHandler( onOpen , make_callback( this , &_select::refreshButtonHandler ) );
	this->contextMenu->setUserEventHandler( onClose , make_callback( this , &_select::refreshButtonHandler ) );
	
	// Register handler
	this->setInternalEventHandler( onDraw , make_callback( &_select::refreshHandler ) );
	this->setInternalEventHandler( onMouseDown , make_callback( this , &_select::toggleContextMenuHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_select::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_select::keyHandler ) );
	this->setInternalEventHandler( onFocus , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onBlur , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onResize , make_callback( &_select::resizeHandler ) );
	
	// Toggle the _contextMenu when the arrow button is clicked
	this->arrow->setUserEventHandler( onMouseDown , make_callback( this , &_select::toggleContextMenuHandler ) );
	
	// Refresh Me
	this->redraw();
	
	// Add arrow
	this->updateArrowButton(); // This updates only its position and type
	this->addChild( this->arrow );
}

void _select::toggleContextMenu()
{
	// Open contextMenu
	this->contextMenu->toggle( this->getAbsoluteX() , this->getAbsoluteY() + this->getHeight() );
}

void _select::updateArrowButton()
{
	this->arrow->moveTo( this->getWidth() - 9 , ( this->getHeight() - this->arrow->getHeight() ) >> 1 );
	this->arrow->setButtonType( this->contextMenu->isOpened() ? _scrollButtonType::buttonTop : _scrollButtonType::buttonBottom );
}

_callbackReturn _select::editHandler( _event event )
{
	// Redraw _select
	this->redraw();
	
	// Forward event to _select
	this->handleEvent( (_event&&)event );
	
	return use_internal;
}

_callbackReturn _select::refreshButtonHandler( _event event ){
	// Update hem button
	this->updateArrowButton();	
	return use_internal;
}

_callbackReturn _select::toggleContextMenuHandler( _event event ){
	this->toggleContextMenu();
	return handled;
}

_callbackReturn _select::resizeHandler( _event event )
{
	// Receive Gadget
	_select* that = event.getGadget<_select>();
	
	// Move the arrow to the right position!
	that->updateArrowButton();
	that->contextMenu->setWidth( that->getWidth() );
	
	return handled;
}

_callbackReturn _select::keyHandler( _event event )
{
	// Receive Gadget
	_select* that = event.getGadget<_select>();
	
	// Provide control by Hardware buttons
	if( event.getKeyCode() == _key::a || event.getKeyCode() == _key::down )
		that->toggleContextMenu();
	
	return handled;
}

_callbackReturn _select::refreshHandler( _event event )
{
	// Receive Gadget
	_select* that = event.getGadget<_select>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill(
		that->hasFocus() ? _color::fromRGB8( 10 , 36 , 106 ) : _color::white
	);
	bP.drawRect( 0 , 0 , bP.getWidth() , bP.getHeight() , _color::fromRGB( 9 , 13 , 19 ) );
	
	_fontHandle font = _fontController::getStandardFont();
	_u8 fontSize = _fontController::getStandardFontSize();
	
	bP.drawString(
		2
		, ( ( that->getHeight() - 1 ) >> 1 ) - ( ( font->getAscent( fontSize ) + 1 ) >> 1 )
		, font , that->getStrValue()
		, that->hasFocus() ? _color::white : _color::black
		, fontSize
	);
	
	return use_default;
}