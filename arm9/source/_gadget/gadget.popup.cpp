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
	
	if( x + width >= SCREEN_WIDTH && y + height >= SCREEN_HEIGHT ){
		this->x = x - width;
		this->y = y - height;
		this->dir = _direction::upleft;
	}
	else if( x + width >= SCREEN_WIDTH ){
		this->x = x - width;
		this->dir = _direction::downleft;
	}
	else if( y + height >= SCREEN_HEIGHT ){
		this->y = y - height;
		this->dir = _direction::upright;
	}
	else{
		this->y = y;
		this->x = x;
		this->dir = _direction::downright;
	}
	
	// View
	_gadget* par = parent ? parent : _system::_gadgetHost_;
	this->setParent( par );
	
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

void _popup::toggle( _coord x , _coord y )
{
	if( this->isOpened() )
		this->shelve();
	else
		this->show( x , y );
}

_popup::_popup( _optValue<_length> width , _optValue<_length> height , _gadget* owner , _style&& style ) :
	_gadget( _gadgetType::popup , 0 , 0 , width , height , (_style&&)style )
	, owner( owner )
	, opened( false )
{
	this->setInternalEventHandler( onBlur , make_callback( &_popup::blurHandler ) );
}