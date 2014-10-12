#include <_gadget/gadget.button.sticky.h>

_callbackReturn _stickyButton::mouseHandler( _event event )
{
	// Receive Gadget
	_stickyButton* that = event.getGadget<_stickyButton>();
	
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

_stickyButton::_stickyButton( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string text , _style&& style ) :
	_button( x , y , width , height , text , (_style&&)style )
	, group( nullptr )
{
	this->setType( _gadgetType::stickybutton );
	
	// Register some new handlers
	this->removeInternalEventHandler( onMouseRepeat );
	this->removeInternalEventHandler( onMouseLeave );
	this->setInternalEventHandler( onMouseEnter , make_callback( &_stickyButton::mouseHandler ) );
}