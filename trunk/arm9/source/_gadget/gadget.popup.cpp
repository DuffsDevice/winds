#include "_gadget/gadget.contextMenu.h"
#include "_controller/controller.gui.h"

_callbackReturn _popup::blurHandler( _event event )
{
	_popup* that = event.getGadget<_popup>();
	
	// Hide
	that->shelve();
	
	return use_default;
}

void _popup::showCentered( _coord x , _coord y )
{
	this->opened = true;
	
	// Trigger event
	this->handleEvent( onOpen );
	
	_length width = this->getWidth();
	_length height = this->getHeight();
	
	this->moveTo( x - width/2 , y - height/2 );
	this->dir = _direction::downright;
	
	// View
	this->setParent( _guiController::getHost() );
	
	// Focus
	this->focus();
}

void _popup::show( _coord x , _coord y , bool rightDownOnly )
{
	this->opened = true;
	
	// Trigger event
	this->handleEvent( onOpen );
	
	_u8 sum = 0;
	
	_length width = this->getWidth();
	_length height = this->getHeight();
	
	if( !rightDownOnly && x + width >= SCREEN_WIDTH ){
		x = x - width;
		sum |= 1;
	}
	
	if( !rightDownOnly && y + height >= SCREEN_HEIGHT ){
		y = y - height;
		sum |= 2;
	}
	
	this->moveTo( x , y );
	
	switch( sum ){
		case 0: // RightDown
			this->dir = _direction::rightdown;
			break;
		case 1: // LeftDown
			this->dir = _direction::leftdown;
			break;
		case 2: // RightUp
			this->dir = _direction::rightup;
			break;
		case 3: // LeftUp
			this->dir = _direction::leftup;
			break;
	}
	
	// View
	this->setParent( _guiController::getHost() );
	
	// Focus
	this->focus();
}

void _popup::shelve( bool focusOwner )
{
	if( !this->isOpened() )
		return;
	
	this->opened = false;
	
	// Trigger Check-Event
	if( this->handleEvent( onClose ) == prevent_default ){
		this->opened = true;
		return;
	}
	
	// Unbind from the tree
	this->setParent( nullptr );
	
	// Give back the focus
	if( focusOwner && this->owner )
		this->owner->focus();
}

void _popup::toggle( _coord x , _coord y , bool rightDownOnly ){
	if( this->isOpened() )
		this->shelve();
	else
		this->show( x , y , rightDownOnly );
}

_popup::_popup( _optValue<_length> width , _optValue<_length> height , _gadget* owner , _style&& style ) :
	_gadget( _gadgetType::popup , 0 , 0 , width , height , (_style&&)style )
	, owner( owner )
	, opened( false )
{
	this->setInternalEventHandler( onBlur , make_callback( &_popup::blurHandler ) );
}