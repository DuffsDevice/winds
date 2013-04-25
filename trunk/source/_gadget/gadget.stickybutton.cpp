#include "_gadget/gadget.stickybutton.h"

_callbackReturn _stickybutton::mouseHandler( _event event )
{
	// Receive Gadget
	_stickybutton* that = event.getGadget<_stickybutton>();
	
	// Ensure it is not check already
	if( that->getIntValue() )
		return handled;
	
	if( that->group )
	{
		that->group->enableSelector( that );
		that->triggerEvent( _event( onChange ) );
	}
	
	return handled;
}

_stickybutton::_stickybutton( _length width , _length height , _coord x , _coord y , string text , _style style ) :
	_button( width , height , x , y , text , style )
	, group( nullptr )
{
	// Register some new handlers
	this->unregisterEventHandler( onMouseLeave );
	this->registerEventHandler( onMouseEnter , new _staticCallback( &_stickybutton::mouseHandler ) );
	this->unregisterEventHandler( mouseRepeat );
}

_stickybutton::_stickybutton( _coord x , _coord y , string text , _style style ) :
	_button( x , y , text , style )
	, group( nullptr )
{
	// Register some new handlers
	this->unregisterEventHandler( onMouseLeave );
	this->registerEventHandler( onMouseEnter , new _staticCallback( &_stickybutton::mouseHandler ) );
	this->unregisterEventHandler( mouseRepeat );
}