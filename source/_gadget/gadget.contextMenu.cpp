#include "_gadget/gadget.contextMenu.h"
#include "_type/type.system.h"


_callbackReturn _contextMenu::blurHandler( _event event )
{
	_contextMenu* that = event.getGadget<_contextMenu>();
	
	// Hide
	that->shelve();
	
	// Trigger Dummy-Event
	that->handleEvent( onClose );
	
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
	
	// View
	this->setParent( _system::_gadgetHost_ );
	
	// Focus
	this->handleEvent( focus );
}

void _contextMenu::shelve()
{
	if( !this->isOpened() )
		return;
	
	// Unbind from the tree
	this->setParent( nullptr );
}

void _contextMenu::toggle( _coord x , _coord y )
{
	if( this->isOpened() )
		this->shelve();
	else
		this->show( x , y );
}

_contextMenu::_contextMenu( _length width , _length height , _style style ) :
	_gadget( _gadgetType::contextmenu , width , height , 0 , 0 , style )
{
	this->registerEventHandler( onBlur , new _staticCallback( &_contextMenu::blurHandler ) );
}