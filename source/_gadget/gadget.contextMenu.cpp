#include "_gadget/gadget.contextMenu.h"
#include "_type/type.system.h"

#include <nds/arm9/console.h>

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

void _contextMenu::closeAndSend( _s32 intValue , string strValue )
{
	// Delete
	this->opened = false;
	this->setParent( nullptr );
	
	if( this->owner )
		this->owner->handleEvent( _gadgetEvent::dialogClose( this , intValue , strValue ) );
}

_gadgetEventReturnType _contextMenu::blurHandler( _gadgetEvent e )
{
	((_contextMenu*)e.getGadget())->closeAndSend();
	
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
	this->setParent( _system_->_gadgetHost_ );
	
	// Focus
	this->handleEvent( _gadgetEvent( "focus" ) );
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

_contextMenu::_contextMenu( _length width , _length height , _gadget* owner , _gadgetStyle style ) :
	_gadget( _gadgetType::contextmenu , width , height , 0 , 0 , style )
	, owner( owner )
	, opened( false )
{	
	this->unregisterEventHandler( "mouseDoubleClick" );
	this->registerEventHandler( "blur" , &_contextMenu::blurHandler );	
}