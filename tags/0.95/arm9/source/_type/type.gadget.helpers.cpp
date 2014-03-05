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
	moveBesidePrecedent::moveBesidePrecedent( _direction dir , _length spaceX , _length spaceY , bool breakLine , _length offsetX , _length offsetY , bool respectAutoPosition ) :
		_dummyCallback<_eventHandler>( &moveBesidePrecedent::executor )
		, spaceX( spaceX )
		, spaceY( spaceY )
		, offsetX( offsetX )
		, offsetY( offsetY )
		, direction( dir )
		, breakLine( breakLine )
		, respectAutoPosition( respectAutoPosition )
	{} // Funny story: http://www.ima.umn.edu/~arnold/disasters/ariane.html
	
	// :: handler
	_callbackReturn moveBesidePrecedent::executor( _event event ) const
	{		
		_gadget* that = event.getGadget();
		
		_gadget*	parent = that->getParent();
		
		if( !parent )
			return use_internal;
		
		_gadget*	pre = that->getPrecedentChild();
		_rect		parentRect = parent->getClientRect();
		
		// Choose right method
		auto usedFunc = this->respectAutoPosition ? &_gadget::moveToIfAuto : &_gadget::moveTo;
		
		if( pre )
		{
			_coord newX;
			_coord newY;
			switch( this->direction )
			{
				case _direction::down:
					newX = pre->hasAutoX() ? pre->getX() : this->offsetX;
					newY = pre->getDimensions().getY2() + 1 + this->spaceY;
					if( this->breakLine && parentRect.isValid() && parentRect.height < ( newY + that->getHeight() ) ){
						newY = this->offsetY;
						newX += this->spaceX + that->getWidth();
					}
					break;
				case _direction::right:
					newX = pre->getDimensions().getX2() + 1 + this->spaceX;
					newY = pre->hasAutoY() ? pre->getY() : this->offsetY;
					if( this->breakLine && parentRect.isValid() && parentRect.width < ( newX + that->getWidth() ) ){
						newX = this->offsetX;
						newY += this->spaceY + that->getHeight();
					}
					break;
				case _direction::up:
					newX = pre->hasAutoX() ? pre->getX() : this->offsetX;
					newY = pre->getY() - this->spaceY - that->getHeight();
					if( this->breakLine && parentRect.isValid() && newY < 1 ){
						newY = parentRect.height - 1 - this->offsetY - that->getHeight();
						newX += this->spaceX + that->getWidth();
					}
					break;
				case _direction::left:
					newX = pre->getX() - this->spaceX - that->getWidth();
					newY = pre->hasAutoY() ? pre->getY() : this->offsetY;
					if( this->breakLine && parentRect.isValid() && newX < 1 ){
						newX = parentRect.width - 1 - this->offsetX - that->getWidth();
						newY += this->spaceY + that->getHeight();
					}
					break;
				default:
					return use_internal;
			}
			(that->*usedFunc)( newX , newY );
		}
		else
		{
			if( this->direction == _direction::down || this->direction == _direction::right )
				(that->*usedFunc)( offsetX , offsetY );
			else if( this->direction == _direction::left )
				(that->*usedFunc)( parentRect.width - 1 - this->offsetX - that->getWidth() , offsetY );
			else if( this->direction == _direction::up )
				(that->*usedFunc)( offsetX , parentRect.height - 1 - this->offsetY - that->getHeight() );
		}
		
		return use_internal;
	}
}