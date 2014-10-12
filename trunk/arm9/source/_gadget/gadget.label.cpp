#include <_gadget/gadget.label.h>
#include <_controller/controller.font.h>

_callbackReturn _label::updateHandler( _event event )
{
	// Receive Gadget
	_label* that = event.getGadget<_label>();
	
	// Check, if the text needs to be updated
	that->checkUpdate();
	
	// Set preferred Size
	that->setSizeIfAuto(
		that->text.getPreferredTextWidth()
		, that->text.getTextHeight()
	);
	
	// Set Text Size
	that->text.indicateNewDimensions();
	
	// Check, if the text needs to be updated
	that->checkUpdate();
	
	return handled;
}

_callbackReturn _label::refreshHandler( _event event )
{
	// Receive Gadget
	_label* that = event.getGadget<_label>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Draw Background
	bP.fill( that->bgColor );
	
	// Draw Text
	that->text.drawTo( that->getClientRect() , bP );
	
	return use_default;
}


_label::_label( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value , _style&& style ) :
	_gadget( _gadgetType::label , x , y , width , height , (_style&&)style )
	, bgColor( _color::transparent )
	, text( move(value) , _fontController::getStandardFont() , _color::black , _fontController::getStandardFontSize() )
{
	// Adjust guiString object
	this->text.setAlign( _align::center );
	this->text.setVAlign( _valign::middle );
	this->text.setEllipsis(0);
	
	// Register an onUpdate-handler
	this->setInternalEventHandler( onUpdate , make_callback( &_label::updateHandler ) );
	
	// Force an update of the size
	this->updateNow();
	
	// Register my handler as the default Refresh-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_label::refreshHandler ) );
	this->setInternalEventHandler( onResize , make_callback( &_label::updateHandler ) );
	
	// Refresh and 
	this->redraw();
}