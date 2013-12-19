#include "_gadget/gadget.fileobject.h"
#include "_type/type.gadget.helpers.h"

namespace _gadgetHelpers
{
	// eventForwardRefresh :: handler
	_callbackReturn eventForwardRefresh::executor( _event event ){
		event.getGadget()->redraw();
		return handled;
	}
	
	// eventForwardRefreshGadget :: handler
	_callbackReturn eventForwardRefreshGadget::executor(_event) const {
		if( this->newGadget )
			this->newGadget->redraw();
		return handled;
	}
	
	// eventForward :: handler
	_callbackReturn eventForward::executor( _event event ) const {
		return event.getGadget()->handleEvent( (_event&&)event.setType( this->newType ) );
	}
	
	// eventForwardGadget :: handler
	_callbackReturn eventForwardGadget::executor( _event event ) const {
		if( this->destination )
			return this->destination->handleEvent( (_event&&)event );
		return not_handled;
	}
	
	
	// rightBottomAlign :: ctor
	rightBottomAlign::rightBottomAlign( _optValue<_length> distanceX , _optValue<_length> distanceY ) :
		_dummyCallback<_eventHandler>( &rightBottomAlign::executor )
		, distanceX( distanceX )
		, distanceY( distanceY )
		, proceedX( distanceX.isValid() )
		, proceedY( distanceY.isValid() )
	{}
	
	// :: handler
	_callbackReturn rightBottomAlign::executor( _event event ) const
	{
		_gadget* that = event.getGadget();
		_gadget* parent = that->getParent();
		
		_length width = parent->getWidth();
		_length height = parent->getHeight();
		
		if( !that->isEnhanced() ){
			_padding pad = parent->getPadding();
			width -= pad.left + pad.right;
			height -= pad.top + pad.bottom;
		}
		
		if( this->proceedX )
		{
			if( this->proceedY )
				that->moveToIfAuto( width - this->distanceX - that->getWidth() , height - this->distanceY - that->getHeight() );
			else
				that->setXIfAuto( width - this->distanceX - that->getWidth() );
		}
		else if( this->proceedY )
			that->setYIfAuto( height - this->distanceY - that->getHeight() );
		
		
		return use_internal;
	}
	
	
	// sizeParent :: ctor
	sizeParent::sizeParent( _optValue<_length> smallerX , _optValue<_length> smallerY ) :
		_dummyCallback<_eventHandler>( &sizeParent::executor )
		, smallerX( smallerX )
		, smallerY( smallerY )
		, proceedX( smallerX.isValid() )
		, proceedY( smallerY.isValid() )
	{}
	
	// :: handler
	_callbackReturn sizeParent::executor( _event event ) const
	{
		_gadget* parent = event.getGadget()->getParent();
		
		_length width = parent->getWidth() - this->smallerX;
		_length height = parent->getHeight() - this->smallerY;
		
		if( !event.getGadget()->isEnhanced() ){
			_padding pad = parent->getPadding();
			width -= pad.left + pad.right;
			height -= pad.top + pad.bottom;
		}
		
		if( this->proceedX )
		{
			if( this->proceedY )
				event.getGadget()->setSizeIfAuto( width , height );
			else
				event.getGadget()->setWidthIfAuto( width );
		}
		else if( this->proceedY )
			event.getGadget()->setHeightIfAuto( height );
		
		return use_internal;
	}
	
	
	// moveBesidePrecedent :: ctor
	moveBesidePrecedent::moveBesidePrecedent( _dimension dim , _length spaceX , _length spaceY , bool breakLine , _length offsetX , _length offsetY ) :
		_dummyCallback<_eventHandler>( &moveBesidePrecedent::executor )
		, dimension( (_u8)dim )
		, breakLine( breakLine )
		, spaceX( spaceX )
		, spaceY( spaceY )
		, offsetX( offsetX )
		, offsetY( offsetY )
	{} // Funny story: http://www.ima.umn.edu/~arnold/disasters/ariane.html
	
	// :: handler
	_callbackReturn moveBesidePrecedent::executor( _event event ) const
	{		
		_gadget* that = event.getGadget();
		
		_dimension	dim = (_dimension) this->dimension;
		_gadget*	parent = that->getParent();
		_gadget*	pre = that->getPrecedentChild();
		_rect		parentRect = parent ? parent->getClientRect() : _rect();
		
		if( pre )
		{
			if( dim == _dimension::vertical )
			{
				_coord newY = pre->getDimensions().getY2() + 1 + this->spaceY;
				_coord newX = pre->hasAutoX() ? pre->getX() : offsetX;
				
				// Check if i we have to break line
				if( this->breakLine && parentRect.isValid() && parentRect.height < ( newY + that->getHeight() ) )
					that->moveToIfAuto( newX , this->offsetY );
				else
					that->moveToIfAuto( newX , newY );
			}
			else
			{
				_coord newX = pre->getDimensions().getX2() + 1 + this->spaceX;
				_coord newY = pre->hasAutoY() ? pre->getY() : offsetY;
				
				// Check if i we have to break line
				if( this->breakLine && parentRect.isValid() && parentRect.width < ( newX + that->getWidth() ) )
					that->moveToIfAuto( this->offsetX , newY );
				else
					that->moveToIfAuto( newX , newY );
			}
		}
		else
			that->moveToIfAuto( offsetX , offsetY );
		
		return use_internal;
	}
}