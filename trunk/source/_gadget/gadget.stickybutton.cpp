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
		that->triggerEvent( onEdit );
	}
	
	return handled;
}

_stickybutton::_stickybutton( _optValue<_length> width , _optValue<_length> height , _coord x , _coord y , string text , _style&& style ) :
	_button( move(width) , move(height) , x , y , text , (_style&&)style )
	, group( nullptr )
{
	this->setType( _gadgetType::stickybutton );
	
	// Register some new handlers
	this->removeInternalEventHandler( onMouseRepeat );
	this->removeInternalEventHandler( onMouseLeave );
	this->setInternalEventHandler( onMouseEnter , make_callback( &_stickybutton::mouseHandler ) );
}