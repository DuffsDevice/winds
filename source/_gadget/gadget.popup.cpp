#include "_gadget/gadget.contextMenu.h"
#include "_type/type.system.h"


_callbackReturn _popup::blurHandler( _event event )
{
	_popup* that = event.getGadget<_popup>();
	
	// Hide
	that->shelve();
	
	return use_default;
}

void _popup::show( _coord x , _coord y )
{
	this->opened = true;
	
	// Trigger event
	this->handleEvent( onOpen );
	
	_length width = this->getWidth();
	_length height = this->getHeight();
	
	if( x + width >= SCREEN_WIDTH && y + height >= SCREEN_HEIGHT )
	{
		this->x = x - width;
		this->y = y - height;
		this->xDir = _align::left;
		this->yDir = _valign::top;
	}
	else if( x + width >= SCREEN_WIDTH )
	{
		this->x = x - width;
		this->xDir = _align::left;
		this->yDir = _valign::bottom;
	}
	else if( y + height >= SCREEN_HEIGHT )
	{
		this->y = y - height;
		this->xDir = _align::right;
		this->yDir = _valign::top;
	}
	else
	{
		this->y = y;
		this->x = x;
		this->xDir = _align::right;
		this->yDir = _valign::bottom;
	}
	
	// View
	this->setParent( _system::_gadgetHost_ );
	
	// Focus
	this->focus();
}

void _popup::shelve( bool focusOwner )
{
	if( !this->isOpened() )
		return;
	
	this->opened = false;
	
	// Trigger Check-Event
	if( this->handleEvent( onClose ) == prevent_default )
	{
		this->opened = true;
		return;
	}
	
	// Unbind from the tree
	this->setParent( nullptr );
	
	// Give back the focus
	if( focusOwner && this->owner )
		this->owner->focus();
}

void _popup::toggle( _coord x , _coord y )
{
	if( this->isOpened() )
		this->shelve();
	else
		this->show( x , y );
}

_popup::_popup( _optValue<_length> width , _optValue<_length> height , _gadget* owner , _style&& style ) :
	_gadget( _gadgetType::popup , move(width) , move(height) , 0 , 0 , (_style&&)style )
	, owner( owner )
{
	this->setInternalEventHandler( onBlur , make_callback( &_popup::blurHandler ) );
}