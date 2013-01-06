#include "_gadget/gadget.contextMenu.h"
#include "_type/type.system.h"

bool _contextMenu::isOpened(){
	return this->opened;
}

void _contextMenu::setOwner( _gadget* owner )
{
	this->owner = owner;
}

_gadget* _contextMenu::getOwner(){
	return this->owner;
}

_callbackReturn _contextMenu::closeAndSend( _event event )
{
	_contextMenu* that = event.getGadget<_contextMenu>();
	
	// Delete
	that->opened = false;
	that->setParent( nullptr );
	
	if( that->owner )
		that->owner->handleEvent( event );
	
	return handled;
}

_callbackReturn _contextMenu::blurHandler( _event event )
{
	_contextMenu* that = event.getGadget<_contextMenu>();
	
	that->handleEvent( blur );
	that->triggerEvent( _event::dialogClose( -1 , "" ) );
	
	return use_default;
}

void _contextMenu::show( _coord x , _coord y )
{
	_length width = this->dimensions.width;
	_length height = this->dimensions.height;
	
	if( x + width >= SCREEN_WIDTH && y + height >= SCREEN_HEIGHT )
	{
		this->dimensions.x = x - width;
		this->dimensions.y = y - height;
		this->xDir = _align::left;
		this->yDir = _valign::top;
	}
	else if( x + width >= SCREEN_WIDTH )
	{
		this->dimensions.x = x - width;
		this->xDir = _align::left;
		this->yDir = _valign::bottom;
	}
	else if( y + height >= SCREEN_HEIGHT )
	{
		this->dimensions.y = y - height;
		this->xDir = _align::right;
		this->yDir = _valign::top;
	}
	else
	{
		this->dimensions.y = y;
		this->dimensions.x = x;
		this->xDir = _align::right;
		this->yDir = _valign::bottom;
	}

	this->opened = true;
	
	// View
	this->setParent( _system::_gadgetHost_ );
	
	// Focus
	this->handleEvent( _event( focus ) );
}

void _contextMenu::hide()
{
	if( this->opened == false )
		return;
	
	this->setParent( nullptr );
	this->opened = false;
}

void _contextMenu::toggle( _coord x , _coord y )
{
	if( this->opened )
		this->hide();
	else
		this->show( x , y );
}

_contextMenu::_contextMenu( _length width , _length height , _gadget* owner , _style style ) :
	_gadget( _gadgetType::contextmenu , width , height , 0 , 0 , style )
	, owner( owner )
	, opened( false )
{	
	this->registerEventHandler( blur , new _staticCallback( &_contextMenu::blurHandler ) );
	this->registerEventHandler( close , new _staticCallback( &_contextMenu::closeAndSend ) );
}