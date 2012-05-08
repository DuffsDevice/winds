#include "_gadget/gadget.contextMenu.h"
#include "_type/type.system.h"

#include <nds/arm9/console.h>

_gadgetEventReturnType _contextMenu::blurHandler( _gadgetEvent e )
{
	_gadget* that = e.getGadget();
	
	// Delete
	that->setParent( nullptr );
	
	return use_default;
}

void _contextMenu::show( _coord x , _coord y )
{
	_length width = this->dimensions.width;
	_length height = this->dimensions.height;
	
	if( x + width >= SCREEN_WIDTH && y + height >= SCREEN_HEIGHT )
	{
		this->dimensions.x = x - width - 1;
		this->dimensions.y = y - height - 1;
		this->xDir = _align::left;
		this->yDir = _valign::top;
	}
	else if( x + width >= SCREEN_WIDTH )
	{
		this->dimensions.x = x - width - 1;
		this->xDir = _align::left;
		this->yDir = _valign::bottom;
	}
	else if( y + height >= SCREEN_HEIGHT )
	{
		this->dimensions.y = y - height - 1;
		this->xDir = _align::right;
		this->yDir = _valign::top;
	}
	else
	{
		this->xDir = _align::right;
		this->yDir = _valign::bottom;
	}

	// View
	this->setParent( _system_->_windows_ );
	
	// Focus
	this->handleEvent( focus );
}

void _contextMenu::hide()
{
	this->setParent( nullptr );
}

void _contextMenu::toggle( _coord x , _coord y ){
	if( this->parent != nullptr )
		this->hide();
	else
		this->show( x , y );
}

_contextMenu::_contextMenu( _length width , _length height , _gadgetStyle style ) :
	_gadget( _gadgetType::contextmenu , width , height , 0 , 0 , style )
{	
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( blur , &_contextMenu::blurHandler );	
}