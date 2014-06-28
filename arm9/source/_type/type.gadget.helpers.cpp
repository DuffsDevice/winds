#include "_gadget/gadget.fileobject.h"
#include "_gadget/gadget.contextmenu.h"
#include "_type/type.gadget.helpers.h"

namespace _gadgetHelpers
{
	// eventForwardRefresh :: handler
	_callbackReturn eventForwardRefresh::executor( _event event ){
		event.getGadget()->redraw();
		return use_internal;
	}
	
	// eventForwardRefreshGadget :: handler
	_callbackReturn eventForwardRefreshGadget::executor(_event) const {
		if( this->newGadget )
			this->newGadget->redraw();
		return use_internal;
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
	moveBesidePrecedent::moveBesidePrecedent( _direction dir , _length spaceX , _length spaceY , bool breakLine , _length offsetX , _length offsetY , bool respectAutoPosition , bool skipHidden ) :
		_dummyCallback<_eventHandler>( &moveBesidePrecedent::executor )
		, spaceX( spaceX )
		, spaceY( spaceY )
		, offsetX( offsetX )
		, offsetY( offsetY )
		, direction( dir )
		, breakLine( breakLine )
		, respectAutoPosition( respectAutoPosition )
		, skipHidden( skipHidden )
	{} // Funny story: http://www.ima.umn.edu/~arnold/disasters/ariane.html
	
	// :: handler
	_callbackReturn moveBesidePrecedent::executor( _event event ) const
	{		
		_gadget* that = event.getGadget();
		
		_gadget*	parent = that->getParent();
		
		if( !parent )
			return use_internal;
		
		// Variables
		_gadget*	pre = that->getPrecedentChild( this->skipHidden );
		_rect		parentRect = parent->getClientRect();
		_coord		newX;
		_coord		newY;
		
		if( pre )
		{
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
		}
		else
		{
			if( this->direction == _direction::down || this->direction == _direction::right ){
				newX = offsetX;
				newY = offsetY;
			}
			else if( this->direction == _direction::left ){
				newX = parentRect.width - 1 - this->offsetX - that->getWidth();
				newY = offsetY;
			}
			else if( this->direction == _direction::up ){
				newX = offsetX;
				newY = parentRect.height - 1 - this->offsetY - that->getHeight();
			}
			else
				return use_internal;
		}
		
		this->respectAutoPosition ? that->moveToIfAuto( newX , newY ) : that->moveTo( newX , newY );
		
		return use_internal;
	}
	
	// openContextMenu :: handler
	_callbackReturn openContextMenu::executor( _event event ) const
	{
		// Receive Gadget
		_gadget* that = event.getGadget();
		
		_uniquePtr<_contextMenu>& contextMenu = openContextMenu::contextMenu;
		if( contextMenu ){
			contextMenu->shelve( false );
			contextMenu = nullptr;
		}
		contextMenu = new _contextMenu( ignore , this->menuToOpen , that );
		
		switch( event.getType() )
		{
			case onMouseDown:
			case onMouseUp:
			case onMouseClick:
			case onMouseDblClick:
			case onMouseRightClick:
				contextMenu->show( event.getPos() + that->getAbsolutePosition() );
				break;
			default:
				contextMenu->show( that->getAbsoluteDimensions() );
		}
		
		openContextMenu::currentSubject = that;
		
		return use_internal;
	}
	
	_uniquePtr<_contextMenu>	openContextMenu::contextMenu;
	_gadget*					openContextMenu::currentSubject = nullptr;
}