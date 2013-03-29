#include "_gadget/gadget.scrollArea.h"
#include "_gadget/gadget.button.h"

map<_string,_scrollType> string2scrollType = {
	{ "scroll" , _scrollType::scroll },
	{ "meta" , _scrollType::meta },
	{ "hidden" , _scrollType::hidden },
	{ "prevent" , _scrollType::prevent }
};

map<_scrollType,_string> scrollType2string = {
	{ _scrollType::scroll , "scroll" },
	{ _scrollType::meta , "meta" },
	{ _scrollType::hidden , "hidden" },
	{ _scrollType::prevent , "prevent" }
};


//_callbackReturn handler( _event e )
//{
//	if( e.getGadget()->getStyle().data )
//		((_scrollArea*)e.getGadget<_button>()->getParent())->scrollTo( 0 , 0 , true );
//	else
//		((_scrollArea*)e.getGadget<_button>()->getParent())->scrollTo( 4342 , 23423 , true );
//	
//	return handled;
//}


_scrollArea::_scrollArea( _length width , _length height , _coord x , _coord y , _scrollType scrollTypeX , _scrollType scrollTypeY , _style style ) :
	_gadget( _gadgetType::scrollarea , width , height , x , y , style )
	, scrollTypeX( scrollTypeX )
	, scrollTypeY( scrollTypeY )
	, scrollBarX( new _scrollBar( 0 , 0 , 8 , 1 , 1 , _dimension::horizontal ) )
	, scrollBarY( new _scrollBar( 0 , 0 , 8 , 1 , 1 , _dimension::vertical ) )
	, offset( _padding( 0 , 0 , 0 , 0 ) )
	, canvasWidth( width )
	, canvasHeight( height )
	, computeCanvasWidth( 1 )
	, computeCanvasHeight( 1 )
{
	// Create scrollbars
	this->scrollBarX->setStep( 5 );
	this->scrollBarX->registerEventHandler( onScroll , new _staticCallback( &_scrollArea::scrollHandler ) );
	
	this->scrollBarY->setStep( 5 );
	this->scrollBarY->registerEventHandler( onScroll , new _staticCallback( &_scrollArea::scrollHandler ) );
	
	// Register 'em
	this->addEnhancedChild( this->scrollBarX );
	this->addEnhancedChild( this->scrollBarY );
	
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( onResize , new _staticCallback( &_scrollArea::resizeHandler ) );
	this->registerEventHandler( refresh , new _staticCallback( &_scrollArea::resizeHandler ) );
	this->registerEventHandler( _internal_ , new _staticCallback( &_scrollArea::resizeHandler ) );
	
	// Refresh Me	
	this->computeClipSize();
	this->updateScrollBars();
	
	//auto b = new _button( 20 , 30 , 200 , 210 , "Hallo" , _style::storeInt( 1 ) );
	//b->registerEventHandler( onAction , new _staticCallback( &handler ) );
	//auto b2 = new _button( 50 , 50 , 1 , 1 , "Hallo" );
	//b2->registerEventHandler( onAction , new _staticCallback( &handler ) );
	//this->addChild( b );
	//this->addChild( b2 );
}


void _scrollArea::addChild( _gadget* child )
{
	child->setParent( nullptr );
	child->moveRelative( - this->scrollBarX->getValue() , - this->scrollBarY->getValue() );
	_gadget::addChild( child );
	
	// Re-compute Scrollbar and innerSize
	if( this->computeCanvasWidth == 1 || this->computeCanvasHeight == 1 )
		this->triggerEvent( _event( _internal_ ) );
	
	if( this->computeCanvasWidth == 1 )
		this->computeCanvasWidth = 2;
	if( this->computeCanvasHeight == 1 )
		this->computeCanvasHeight = 2;
}


void _scrollArea::removeChild( _gadget* child )
{
	_gadget::removeChild( child );
	
	if( child->isEnhanced() )
		return;
	
	// Re-compute Scrollbar and innerSize
	if( this->computeCanvasWidth == 1 || this->computeCanvasHeight == 1 )
		this->triggerEvent( _event( _internal_ ) );
	
	if( this->computeCanvasWidth == 1 )
		this->computeCanvasWidth = 2;
	if( this->computeCanvasHeight == 1 )
		this->computeCanvasHeight = 2;
}


void _scrollArea::removeChildren( bool remove )
{
	_gadget::removeChildren( remove );
	
	// Re-compute Scrollbar and innerSize
	if( this->computeCanvasWidth == 1 || this->computeCanvasHeight == 1 )
		this->triggerEvent( _event( _internal_ ) );
	
	if( this->computeCanvasWidth == 1 )
		this->computeCanvasWidth = 2;
	if( this->computeCanvasHeight == 1 )
		this->computeCanvasHeight = 2;
}


void _scrollArea::removeEnhancedChildren( bool remove )
{
	// Remove scrollbars so that they don't get deleted
	this->removeChild( this->scrollBarX );
	this->removeChild( this->scrollBarY );
	
	_gadget::removeEnhancedChildren( remove );
	
	// Re-compute Scrollbar and innerSize
	if( this->computeCanvasWidth == 1 || this->computeCanvasHeight == 1 )
		this->triggerEvent( _event( _internal_ ) );
	
	if( this->computeCanvasWidth == 1 )
		this->computeCanvasWidth = 2;
	if( this->computeCanvasHeight == 1 )
		this->computeCanvasHeight = 2;
	
	// Add them back again!
	this->addEnhancedChild( this->scrollBarX );
	this->addEnhancedChild( this->scrollBarY );
}


void _scrollArea::updateScrollBars()
{
	bool needY = this->dimensions.width != this->clipWidth;
	bool needX = this->dimensions.height != this->clipHeight;
	
	if( needX )
		this->scrollBarX->show();
	else
		this->scrollBarX->hide();
	
	if( this->scrollTypeX != _scrollType::prevent )
	{
		this->scrollBarX->setY( this->dimensions.height - 8 - this->offset.top );
		this->scrollBarX->setWidth( this->dimensions.width - ( needY ? 8 : 0 ) - this->offset.left - this->offset.right );
		this->scrollBarX->setLength( this->clipWidth - this->offset.left - this->offset.right );
		this->scrollBarX->setLength2( this->canvasWidth );
	}
	
	if( needY )
		this->scrollBarY->show();
	else
		this->scrollBarY->hide();
	
	if( this->scrollTypeY != _scrollType::prevent )
	{
		this->scrollBarY->setX( this->dimensions.width - 8 - this->offset.right );
		this->scrollBarY->setHeight( this->dimensions.height - ( needX ? 8 : 0 ) - this->offset.top - this->offset.bottom );
		this->scrollBarY->setLength( this->clipHeight - this->offset.top - this->offset.bottom );
		this->scrollBarY->setLength2( this->canvasHeight );
	}
	
	_padding p = _padding( this->offset.left , this->offset.top , ( needY ? 8 : 0 ) + this->offset.right , ( needX ? 8 : 0 ) + this->offset.bottom );
	
	// Crop children Area
	this->setPadding( p );
}


void _scrollArea::computeClipSize()
{
	this->clipWidth = this->dimensions.width;
	this->clipHeight = this->dimensions.height;
	
	switch( this->scrollTypeX )
	{
		case _scrollType::meta:
			if( this->canvasWidth > this->clipWidth )
			{
				// Ich brauch ihn in X-Richtung
				this->clipHeight -= 8;
				
				// Brauch ich ihn (jetzt) auch in Y-Richtung?
				if( this->scrollTypeY == _scrollType::scroll || ( this->scrollTypeY == _scrollType::meta && this->canvasWidth > this->clipWidth ) )
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


_callbackReturn _scrollArea::resizeHandler( _event event )
{
	_scrollArea* that = event.getGadget<_scrollArea>();
	
	if( event.getType() == refresh )
	{
		_bitmapPort bP = that->getBitmapPort();
		
		if( event.hasClippingRects() )
			bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
		else
			bP.normalizeClippingRects();
		
		bP.fill( COLOR_WHITE );
		
		return use_default;
		
	}
	else if( event.getType() == _internal_ )
		that->computeCanvasSize();
	
	that->computeClipSize();
	that->updateScrollBars();
	
	return handled;
}


void _scrollArea::computeCanvasSize(){
	// Return if there's nothing to do
	if( this->computeCanvasWidth == 2 || this->computeCanvasHeight == 2 )
	{
	
		_coord maxX = 0;
		_coord maxY = 0;
		
		for( _gadget* child : this->children )
		{
			const _rect& dim = child->getDimensions();
			maxX = max( dim.getX2() , maxX );
			maxY = max( dim.getY2() , maxY );
		}
		
		// Check for auto-compute of Width
		if( this->computeCanvasWidth == 2 ){
			maxX += 2 + this->scrollBarX->getValue();
			this->canvasWidth = maxX;
			this->computeCanvasWidth = 1;
		}
		
		// Check for auto-compute of Height
		if( this->computeCanvasHeight == 2 ){
			maxY += 2 + this->scrollBarY->getValue();
			this->canvasHeight = maxY;
			this->computeCanvasHeight = 1;
		}
	}
}


_callbackReturn _scrollArea::scrollHandler( _event event ){
	
	// Receive Gadget
	_scrollArea* area = (_scrollArea*)(event.getGadget()->getParent());
	
	if( event.getDeltaX() || event.getDeltaY() )
		for( _gadget* child : area->children )
			child->moveRelative( - event.getDeltaX() , - event.getDeltaY() );
	
	// Forward onScroll-Event!
	area->triggerEvent( event );
	
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