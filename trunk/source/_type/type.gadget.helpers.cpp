#include "_type/type.gadget.helpers.h"

namespace _gadgetHelpers
{
	// eventForwardRefresh :: handler
	_callbackReturn eventForwardRefresh::refreshForwardHandler( _event event ){
		event.getGadget()->redraw();
		return handled;
	}
	
	// eventForwardRefreshGadget :: handler
	_callbackReturn eventForwardRefreshGadget::refreshForwardHandler(_event){
		((_gadget*)this)->redraw();
		return handled;
	}
	
	// eventForward :: handler
	_callbackReturn eventForward::eventForwardHandler( _event event ){
		return event.getGadget()->handleEvent( (_event&&)event.setType( (_eventType)(int)this ) );
	}
	
	// eventForwardGadget :: handler
	_callbackReturn eventForwardGadget::eventForwardHandler( _event event ){
		if( this )
			return ((_gadget*)this)->handleEvent( (_event&&)event );
		return not_handled;
	}
	
	
	// sizeParent :: ctor
	sizeParent::sizeParent( _optValue<_length>&& smallerX , _optValue<_length>&& smallerY ) :
		_classCallback(
			(sizeParent*)(
				( smallerX.isValid() ? (_length)smallerX : (-1) ) << 16
				| ( smallerY.isValid() ? (_length)smallerY : (-1) )
			)
			, &sizeParent::eventHandler )
	{}
	
	// :: handler
	_callbackReturn sizeParent::eventHandler( _event event )
	{
		if( !event.getGadget() )
			return not_handled;
		
		_gadget* parent = event.getGadget()->getParent();
		_padding pad = parent->getPadding();
		_length smallerX = (int)this >> 16;
		_length smallerY = (int)this & ( ( 1 << 17 ) - 1 );
		
		if( smallerX >= 0 )
		{
			if( smallerY >= 0 )
				event.getGadget()->setSize( parent->getWidth() - pad.left - pad.right - smallerX , parent->getHeight() - pad.top - pad.bottom - smallerY );
			else
				event.getGadget()->setWidth( parent->getWidth() - pad.left - pad.right - smallerX );
		}
		else if( smallerY >= 0 )
			event.getGadget()->setHeight( parent->getHeight() - pad.top - pad.bottom - smallerY );
		
		return handled;
	}
	
	// moveBesidePrecedent :: ctor
	moveBesidePrecedent::moveBesidePrecedent( _dimension dim , _u8 spaceX , _u8 spaceY , bool skipHidden , _optValue<_u16>&& lbOffset ) :
		_classCallback(
			(moveBesidePrecedent*)(
				(internalDataFormatStruct)
				{
					(_u8)dim
					, skipHidden
					, spaceX
					, spaceY
					, lbOffset.isValid() ? _u16(lbOffset) + 1 : _u16(0)
				} // About the warning: http://www.ima.umn.edu/~arnold/disasters/ariane.html
			)
			, &moveBesidePrecedent::eventHandler
		)
	{}
	
	// :: handler
	_callbackReturn moveBesidePrecedent::eventHandler( _event event )
	{		
		_gadget* that = event.getGadget();
		
		union{
			moveBesidePrecedent* ptr;
			internalDataFormatStruct data;
		} var;
		
		var.ptr = this;
		
		_dimension dim = (_dimension) var.data.dim;
		bool skipHidden = var.data.skipHidden;
		_u8 spaceX = var.data.spaceX;
		_u8 spaceY = var.data.spaceY;
		
		_gadget* parent = that->getParent();
		_gadget* pre = that->getPrecedentChild( skipHidden );
		_rect parentRect = parent ? parent->getClientRect() : _rect();
		
		if( pre )
		{
			_coord myY = pre->getDimensions().getY2() + 1 + spaceY;
			_coord myX = pre->getDimensions().getX2() + 1 + spaceX;
			_coord myY2 = myY + that->getHeight();
			_coord myX2 = myX + that->getWidth();
			
			if( dim == _dimension::vertical )
			{
				// Check if i we have to break line
				if( var.data.lBOffset && parentRect.isValid() && parentRect.height < myY2 )
					that->moveToInternal( myX , var.data.lBOffset-1 );
				else
					that->moveToInternal( pre->getX() , myY );
			}
			else
			{
				// Check if i we have to break line
				if( var.data.lBOffset && parentRect.isValid() && parentRect.width < myX2 )
					that->moveToInternal( var.data.lBOffset-1 , myY );
				else
					that->moveToInternal( myX , pre->getY() );
			}
		}
		else if( dim == _dimension::vertical )
			that->moveToInternal( spaceX , var.data.lBOffset ? var.data.lBOffset - 1 : spaceY );
		else
			that->moveToInternal( var.data.lBOffset ? var.data.lBOffset - 1 : spaceX , spaceY );
		
		return handled;
	}
}