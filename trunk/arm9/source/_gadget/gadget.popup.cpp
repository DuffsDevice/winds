#include <_gadget/gadget.contextMenu.h>
#include <_controller/controller.gui.h>

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
	this->dir = _direction::center;
	
	// View
	this->enhanceToParent( _guiController::getHost() );
	
	// Focus
	this->focus();
}

void _popup::show( _coord x , _coord y , bool rightDownOnly )
{
	this->opened = true;
	
	// Throw an open-event
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
	
	_s8 offsetX = this->offset.first;
	_s8 offsetY = this->offset.second;
	
	switch( sum ){
		case 0: // RightDown
			this->dir = _direction::rightdown;
			x -= offsetX;
			y -= offsetY;
			break;
		case 1: // LeftDown
			this->dir = _direction::leftdown;
			x += offsetX;
			y -= offsetY;
			break;
		case 2: // RightUp
			this->dir = _direction::rightup;
			x -= offsetX;
			y += offsetY;
			break;
		case 3: // LeftUp
			this->dir = _direction::leftup;
			x += offsetX;
			y += offsetY;
			break;
	}
	
	this->moveTo( x , y );
	
	// Refresh gadget before appearing
	this->handleEvent( onDraw );
	
	// View
	this->enhanceToParent( _guiController::getHost() );
	
	// Focus
	this->focus();
}

void _popup::show( _rect object , bool rightDownOnly )
{
	this->opened = true;
	
	// Throw an open-event
	this->handleEvent( onOpen );
	
	_u8 sum = 0;
	
	_length width = this->getWidth();
	_length height = this->getHeight();
	_coord newX = object.getX2();
	_coord newY = object.getY2();
	
	if( !rightDownOnly && newX + width >= SCREEN_WIDTH ){
		newX = object.x - width;
		sum |= 1;
	}
	
	if( !rightDownOnly && newY + height >= SCREEN_HEIGHT ){
		newY = object.y - height;
		sum |= 2;
	}
	
	_s8 offsetX = this->offset.first;
	_s8 offsetY = this->offset.second;
	
	switch( sum ){
		case 0: // RightDown
			this->dir = _direction::rightdown;
			newX -= offsetX;
			newY -= offsetY;
			break;
		case 1: // LeftDown
			this->dir = _direction::leftdown;
			newX += offsetX;
			newY -= offsetY;
			break;
		case 2: // RightUp
			this->dir = _direction::rightup;
			newX -= offsetX;
			newY += offsetY;
			break;
		case 3: // LeftUp
			this->dir = _direction::leftup;
			newX += offsetX;
			newY += offsetY;
			break;
	}
	
	this->moveTo( newX , newY );
	
	// Refresh gadget before appearing
	this->handleEvent( onDraw );
	
	// View
	this->enhanceToParent( _guiController::getHost() );
	
	// Focus
	this->focus();
}

void _popup::shelve( bool focusOwner )
{
	if( !this->isOpened() )
		return;
	
	this->opened = false;
	
	// Unbind from the tree
	this->setParent( nullptr );
	
	// Give back the focus
	if( focusOwner && this->owner )
		this->owner->focus();
	
	// Trigger Check-Event
	this->triggerEvent( onClose );
}

void _popup::toggle( _rect object , bool rightDownOnly )
{
	if( this->isOpened() )
		this->shelve();
	else
		this->show( object , rightDownOnly );
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
	, offset( 0 )
{
	this->setInternalEventHandler( onBlur , make_callback( &_popup::blurHandler ) );
}