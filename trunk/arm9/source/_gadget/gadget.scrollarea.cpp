#include "_gadget/gadget.scrollArea.h"
#include "_gadget/gadget.button.h"

_scrollArea::_scrollArea( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _scrollType scrollTypeX , _scrollType scrollTypeY , _style&& style ) :
	_gadget( _gadgetType::scrollarea , x , y , width , height , (_style&&)style )
	, scrollTypeX( scrollTypeX )
	, scrollTypeY( scrollTypeY )
	, scrollBarX( new _scrollBar( 0 , 0 , 8 , 1 , 1 , _dimension::horizontal ) )
	, scrollBarY( new _scrollBar( 0 , 0 , 8 , 1 , 1 , _dimension::vertical ) )
	, offset( _padding( 0 , 0 , 0 , 0 ) )
	, canvasWidth( width )
	, canvasHeight( height )
	, leaveCorner( false )
{
	
	// Set minimum size
	this->setMinHeight( 14 );
	this->setMinWidth( 14 );
	
	// Create scrollbars
	this->scrollBarX->setStep( 5 );
	this->scrollBarX->setInternalEventHandler( onScroll , make_callback( &_scrollArea::scrollHandler ) );
	
	this->scrollBarY->setStep( 5 );
	this->scrollBarY->setInternalEventHandler( onScroll , make_callback( &_scrollArea::scrollHandler ) );
	
	// Register 'em
	this->addEnhancedChild( this->scrollBarX );
	this->addEnhancedChild( this->scrollBarY );
	
	// Register my handler as the default Refresh-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_scrollArea::refreshHandler ) );
	this->setInternalEventHandler( onUpdate , make_callback( &_scrollArea::updateHandler ) );
	this->setInternalEventHandler( onResize , make_callback( &_scrollArea::updateHandler ) );
	this->setInternalEventHandler( onChildAdd , make_callback( &_scrollArea::childHandler ) );
	this->setInternalEventHandler( onChildRemove , make_callback( &_scrollArea::childHandler ) );
	this->setInternalEventHandler( onChildResize , make_callback( &_scrollArea::childHandler ) );
	this->setInternalEventHandler( onChildMove , make_callback( &_scrollArea::childHandler ) );
	this->setInternalEventHandler( onChildShow , make_callback( &_scrollArea::childHandler ) );
	this->setInternalEventHandler( onChildHide , make_callback( &_scrollArea::childHandler ) );
	this->setInternalEventHandler( onChildFocus , make_callback( &_scrollArea::childHandler ) );
	
	// Refresh Me
	this->updateNow();
}

void _scrollArea::updateScrollBars()
{
	bool needY = this->getWidth() != this->clipWidth;
	bool needX = this->getHeight() != this->clipHeight;
	
	if( needX )
		this->scrollBarX->show();
	else
		this->scrollBarX->hide();
	
	if( this->scrollTypeX != _scrollType::prevent )
	{
		this->scrollBarX->setY( this->getHeight() - 8 - this->offset.top );
		this->scrollBarX->setWidth( this->getWidth() - ( needY || this->leaveCorner ? 8 : 0 ) - this->offset.left - this->offset.right );
		this->scrollBarX->setLength( this->clipWidth - this->offset.left - this->offset.right );
		this->scrollBarX->setLength2( this->canvasWidth );
	}
	
	if( needY )
		this->scrollBarY->show();
	else
		this->scrollBarY->hide();
	
	if( this->scrollTypeY != _scrollType::prevent )
	{
		this->scrollBarY->setX( this->getWidth() - 8 - this->offset.right );
		this->scrollBarY->setHeight( this->getHeight() - ( needX || this->leaveCorner ? 8 : 0 ) - this->offset.top - this->offset.bottom );
		this->scrollBarY->setLength( this->clipHeight - this->offset.top - this->offset.bottom );
		this->scrollBarY->setLength2( this->canvasHeight );
	}
	
	_padding p = _padding( this->offset.left , this->offset.top , ( needY ? 8 : 0 ) + this->offset.right , ( needX ? 8 : 0 ) + this->offset.bottom );
	
	// Crop children Area
	this->setPadding( p );
}


void _scrollArea::computeClipSize()
{
	this->clipWidth = this->getWidth();
	this->clipHeight = this->getHeight();
	
	switch( this->scrollTypeX )
	{
		case _scrollType::meta:
			if( this->canvasWidth > this->clipWidth )
			{
				// Ich brauch ihn in X-Richtung
				this->clipHeight -= 8;
				
				// Brauch ich ihn (jetzt) auch in Y-Richtung?
				if( this->scrollTypeY == _scrollType::scroll || ( this->scrollTypeY == _scrollType::meta && this->canvasHeight > this->clipHeight ) )
					this->clipWidth -= 8;
			}
			else if( this->scrollTypeY == _scrollType::scroll || ( this->scrollTypeY == _scrollType::meta && this->canvasHeight > this->clipHeight ) )
			{
				// Ich brauch ihn (erst mal) nicht in X-Richtung aber ich brauch ihn in Y-Richtung
				this->clipWidth -= 8;
				
				// Brauch ich jetzt vielleicht ihn in X-Richtung?
				if( this->canvasWidth > this->clipWidth )
					this->clipHeight -= 8;
			}
			break;
		case _scrollType::scroll:
			this->clipHeight -= 8;
			
			// Brauch ich die Scrollbar in Y-Richtung (auch)?
			if( this->scrollTypeY == _scrollType::scroll || ( this->scrollTypeY == _scrollType::meta && this->canvasHeight > this->clipHeight ) )
				this->clipWidth -= 8;
			break;
		default:
			break;
	}
}


_callbackReturn _scrollArea::refreshHandler( _event event )
{
	_scrollArea* that = event.getGadget<_scrollArea>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( _color::white );
	
	return use_default;
}


_callbackReturn _scrollArea::updateHandler( _event event )
{
	_scrollArea* that = event.getGadget<_scrollArea>();
	
	_coord maxX = 0;
	_coord maxY = 0;
	
	for( const _gadget* child : that->getChildren( false ) )
	{
		if( child->isHidden() )
			continue;
		
		const _rect& dim = child->getDimensions();
		maxX = max( dim.getX2() , maxX );
		maxY = max( dim.getY2() , maxY );
	}
	
	// auto-compute the canvas size
	maxX += 2 + that->scrollBarX->getValue();
	maxY += 2 + that->scrollBarY->getValue();
	that->canvasWidth = maxX;
	that->canvasHeight = maxY;
	
	// Since the canvas size might have changed, update the scrollbars
	that->computeClipSize();
	that->updateScrollBars();
	
	return handled;
}

_callbackReturn _scrollArea::childHandler( _event event )
{
	// Fetch Instance
	_scrollArea* that = event.getGadget<_scrollArea>();
	
	// Abort if one of the scrollbars triggered the event
	if( event.getGadgetParam() == that->scrollBarX || event.getGadgetParam() == that->scrollBarY )
		return handled;
	
	if( event == onChildAdd )
	{
		// Get the child that was added
		_gadget* child = event.getGadgetParam();
		
		if( child )
			child->moveRelative( - that->scrollBarX->getValue() , - that->scrollBarY->getValue() , true );
	}
	else if( event == onChildRemove )
	{
		// Get the child that was added
		_gadget* child = event.getGadgetParam();
		
		// child == nullptr implicitly checks whether 'removeChildren' was called
		if( child == nullptr )
			that->scrollTo( 0 , 0 , false );
	}
	else if( event == onChildFocus )
	{
		// Get the child that was focused
		_gadget* child = event.getGadgetParam();
		
		// Get the Child's dimensions
		_rect childRect = child->getDimensions();
		childRect.toRelative( -that->getScrollX() , -that->getScrollY() );
		
		// Scroll, so that the child is visible
		_s32 scrollX = min<_s32>( childRect.x - 1 , max<_s32>( that->getScrollX() , childRect.getX2() - that->clipWidth + 2 ) );
		_s32 scrollY = min<_s32>( childRect.y - 1 , max<_s32>( that->getScrollY() , childRect.getY2() - that->clipHeight + 2 ) );
		that->scrollTo( scrollX , scrollY , false );
		
		return handled;
	}
	
	// Force update of canvas size
	that->update();
	
	return handled;
}


_callbackReturn _scrollArea::resizeHandler( _event event )
{
	// Fetch Instance
	_scrollArea* that = event.getGadget<_scrollArea>();
	
	that->computeClipSize();
	that->updateScrollBars();
	
	return handled;
}
_callbackReturn _scrollArea::scrollHandler( _event event )
{
	// Receive Gadget
	_scrollArea* area = (_scrollArea*)(event.getGadget()->getParent());
	
	if( event.getDeltaX() || event.getDeltaY() )
		for( _gadget* child : area->getChildren( false ) )
			child->moveRelative( - event.getDeltaX() , - event.getDeltaY() , true );
	
	// Forward onScroll-Event!
	area->triggerEvent( move(event) );
	
	return handled;
}


void _scrollArea::setScrollTypeX( _scrollType typeX ){
	this->scrollTypeX = typeX;
	this->computeClipSize();
	this->updateScrollBars();
}


void _scrollArea::setScrollTypeY( _scrollType typeY ){
	this->scrollTypeY = typeY;
	this->computeClipSize();
	this->updateScrollBars();
}

_fromStr<_scrollType> string2scrollType = {
	{ "scroll" , _scrollType::scroll },
	{ "meta" , _scrollType::meta },
	{ "hidden" , _scrollType::hidden },
	{ "prevent" , _scrollType::prevent }
};

_toStr<_scrollType> scrollType2string = {
	{ _scrollType::scroll , "scroll" },
	{ _scrollType::meta , "meta" },
	{ _scrollType::hidden , "hidden" },
	{ _scrollType::prevent , "prevent" }
};