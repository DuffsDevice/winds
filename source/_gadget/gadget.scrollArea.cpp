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

void _scrollArea::updateScrollBars()
{
	bool needY = this->dimensions.width != this->clipWidth;
	bool needX = this->dimensions.height != this->clipHeight;
	
	if( needX )
	{
		this->scrollBarX->setY( this->dimensions.height - 8 );
		this->scrollBarX->setWidth( this->dimensions.width - ( needY ? 8 : 0 ) );
		this->scrollBarX->setLength( this->clipWidth );
		this->scrollBarX->setLength2( this->canvasWidth );
		
		if( !this->scrollBarX->parent )
			this->addEnhancedChild( this->scrollBarX );
	}
	else if( this->scrollBarX->parent )
		this->scrollBarX->setParent( nullptr );
	
	if( needY )
	{
		this->scrollBarY->setX( this->dimensions.width - 8 );
		this->scrollBarY->setHeight( this->dimensions.height - ( needX ? 8 : 0 ) );
		this->scrollBarY->setLength( this->clipHeight );
		this->scrollBarY->setLength2( this->canvasHeight );
		
		
		if( !this->scrollBarY->parent )
			this->addEnhancedChild( this->scrollBarY );
	}
	else if( this->scrollBarY->parent )
		this->scrollBarY->setParent( nullptr );
	
	_padding p = _padding( 0 , 0 , needY ? 8 : 0 , needX ? 8 : 0 );
	
	if( p != this->getPadding() )
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
			bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
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
	if( this->computeCanvasWidth != 2 && this->computeCanvasHeight != 2 )
		return;
	
	int maxX = 0;
	int maxY = 0;
	
	for( _gadget* child : this->children )
	{
		const _rect& dim = child->getDimensions();
		maxX = max( dim.getX2() , maxX );
		maxY = max( dim.getY2() , maxY );
	}
	
	if( this->computeCanvasWidth == 2 ){
		maxX += 2 + this->scrollBarX->getValue();
		this->canvasWidth = maxX;
		this->computeCanvasWidth = 1;
	}
	if( this->computeCanvasHeight == 2 ){
		maxY += 2 + this->scrollBarY->getValue();
		this->canvasHeight = maxY;
		this->computeCanvasHeight = 1;
	}
}

_callbackReturn _scrollArea::scrollHandler( _event event ){
	
	// Receive Gadget
	_scrollArea* area = (_scrollArea*)(event.getGadget()->getParent());
	
	if( event.getDeltaX() || event.getDeltaY() )
		for( _gadget* child : area->children )
			child->moveRelative( - event.getDeltaX() , - event.getDeltaY() );
	
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

//_callbackReturn handler( _event e )
//{
//	if( e.getGadget()->getStyle().data )
//		((_scrollArea*)e.getGadget<_button>()->parent)->scrollTo( 0 , 0 , true );
//	else
//		((_scrollArea*)e.getGadget<_button>()->parent)->scrollTo( 4342 , 23423 , true );
//	
//	return handled;
//}

_scrollArea::_scrollArea( _length width , _length height , _coord x , _coord y , _scrollType scrollTypeX , _scrollType scrollTypeY , _style style ) :
	_gadget( _gadgetType::scrollarea , width , height , x , y , style )
	, scrollTypeX( scrollTypeX )
	, scrollTypeY( scrollTypeY )
	, scrollBarX( new _scrollBar( 0 , 0 , 8 , 1 , 1 , _dimension::horizontal ) )
	, scrollBarY( new _scrollBar( 0 , 0 , 8 , 1 , 1 , _dimension::vertical ) )
	, canvasWidth( width )
	, canvasHeight( height )
	, computeCanvasWidth( 1 )
	, computeCanvasHeight( 1 )
{
	this->scrollBarX->setStep( 5 );
	this->scrollBarX->registerEventHandler( onChange , &_scrollArea::scrollHandler );
	
	this->scrollBarY->setStep( 5 );
	this->scrollBarY->registerEventHandler( onChange , &_scrollArea::scrollHandler );
	
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( onResize , &_scrollArea::resizeHandler );
	this->registerEventHandler( refresh , &_scrollArea::resizeHandler );
	this->registerEventHandler( _internal_ , &_scrollArea::resizeHandler );
	
	// Refresh Me	
	this->computeClipSize();
	this->updateScrollBars();
	
	//auto b = new _button( 20 , 30 , 110 , 90 , "Hallo" , _style::storeData( 1 ) );
	//b->registerEventHandler( onAction , &handler );
	//auto b2 = new _button( 50 , 50 , 1 , 1 , "Hallo" );
	//b2->registerEventHandler( onAction , &handler );
	//this->addChild( b );
	//this->addChild( b2 );
}

_scrollArea::~_scrollArea(){
	delete this->scrollBarX;
	delete this->scrollBarY;
}