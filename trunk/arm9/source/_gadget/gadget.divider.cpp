#include <_gadget/gadget.divider.h>
#include <_controller/controller.gui.h>

void _dividerGadget::setDimension( _dimension dim )
{
	if( this->dim == dim )
		return;
	
	_optValue<_coord> pos1 = this->hasAutoX() ? _optValue<_coord>( ignore ) : _optValue<_coord>( this->getX() );
	_optValue<_coord> pos2 = this->hasAutoY() ? _optValue<_coord>( ignore ) : _optValue<_coord>( this->getY() );
	
	// Swap X and Y Dimension
	if( pos1.isValid() )
		this->setY( pos1 );
	else
		this->requestAutoY();
	
	if( pos2.isValid() )
		this->setX( pos2 );
	else
		this->requestAutoX();
	
	this->dim = dim;
	this->update();
}

_callbackReturn _dividerGadget::refreshHandler( _event event )
{
	// Receive Gadget
	_dividerGadget* that = event.getGadget<_dividerGadget>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myW = bP.getWidth();
	_length myH = bP.getHeight();
	
	if( that->dim == _dimension::horizontal ){
		bP.drawHorizontalLine( 0 , 0 , myW , _guiController::getControlFg() );
		bP.drawHorizontalLine( 0 , 1 , myW , _color::white );
	}else{
		bP.drawVerticalLine( 0 , 0 , myH , _guiController::getControlFg() );
		bP.drawVerticalLine( 0 , 1 , myH , _color::white );
	}
	
	return use_default;
}

_callbackReturn _dividerGadget::updateHandler( _event event )
{
	// Receive Gadget
	_dividerGadget* that = event.getGadget<_dividerGadget>();
	_gadget* parent = that->getParent();
	
	if( !parent )
		return not_handled;
	
	_padding pad = parent->getPadding();
	
	if( that->dim == _dimension::vertical ){
		that->setYIfAuto( that->margin + ( that->isEnhanced() ? pad.top : 0 ) );
		that->setSizeIfAuto( 2 , parent->getHeight() - pad.top - pad.bottom - that->margin * 2 );
	}
	else{
		that->setXIfAuto( that->margin + ( that->isEnhanced() ? pad.left : 0 ) );
		that->setSizeIfAuto( parent->getWidth() - pad.left - pad.right - that->margin * 2 , 2 );
	}
	
	return handled;
}

_dividerGadget::_dividerGadget( _dimension dim , _optValue<_coord> position , _u8 margin , _style&& style ) :
	_gadget(
		_gadgetType::divider
		, dim != _dimension::vertical ? _optValue<_coord>(ignore) : _optValue<_coord>(position)
		, dim == _dimension::vertical ? _optValue<_coord>(ignore) : _optValue<_coord>(position)
		, ignore
		, ignore
		, style | _style::notClickable | _style::noTransparentParts
	)
	, margin( margin )
	, dim( dim )
{
	// Register updatehandler
	this->setInternalEventHandler( onUpdate , make_callback( &_dividerGadget::updateHandler ) );
	
	this->updateNow();
	
	// Regsiter other handlers
	this->setInternalEventHandler( onParentResize , make_callback( &_dividerGadget::updateHandler ) );
	this->setInternalEventHandler( onParentAdd , make_callback( &_dividerGadget::updateHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_dividerGadget::refreshHandler ) );
}