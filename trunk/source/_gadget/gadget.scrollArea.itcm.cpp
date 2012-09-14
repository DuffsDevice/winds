#include "_gadget/gadget.scrollArea.h"
#include "_gadget/gadget.button.h"

map<_string,_scrollType> string2scrollType = {
	{ "scroll" , _scrollType::scroll },
	{ "hidden" , _scrollType::hidden },
	{ "prevent" , _scrollType::prevent }
};
map<_scrollType,_string> scrollType2string = {
	{ _scrollType::scroll , "scroll" },
	{ _scrollType::hidden , "hidden" },
	{ _scrollType::prevent , "prevent" }
};

void _scrollArea::addChild( _gadget* child )
{
	child->moveRelative( - this->scrollX , - this->scrollY );
	
	_gadget::addChild( child );
	if( !child->isEnhanced() )
		this->nonEnhancedChildren.push_back( child );
	else
		return;
	
	// Re-compute Scrollbar and innerSize
	this->computeCanvasSize();
	refresh();
}

void _scrollArea::removeChild( _gadget* child )
{
	_gadget::removeChild( child );
	
	if( !child->isEnhanced() )
	{
		_gadgetList::iterator it = find( this->nonEnhancedChildren.begin() , this->nonEnhancedChildren.end() , child );
		
		// Found?
		if( it == this->nonEnhancedChildren.end() )
			return;
			
		// Erase it
		this->nonEnhancedChildren.erase( it );
	}
	else
		return;
	
	// Re-compute Scrollbar and innerSize
	this->computeCanvasSize();
	refresh();
}

void _scrollArea::refresh()
{
	bool needY = this->dimensions.width != this->clipWidth;
	bool needX = this->dimensions.height != this->clipHeight;
	
	if( needX )
	{
		this->scrollBarX->setValue( this->scrollX );
		this->scrollBarX->setY( this->dimensions.height - 8 );
		this->scrollBarX->setWidth( this->dimensions.width - ( needY ? 8 : 0 ) );
		this->scrollBarX->setLength( this->clipWidth );
		this->scrollBarX->setLength2( this->canvasWidth );
		if( !this->scrollBarX->parent )
			this->scrollBarX->setParent( this );
	}
	else if( this->scrollBarX->parent )
		this->scrollBarX->setParent( nullptr );
	
	if( needY )
	{
		this->scrollBarY->setValue( this->scrollY );
		this->scrollBarY->setX( this->dimensions.width - 8 );
		this->scrollBarY->setHeight( this->dimensions.height - ( needX ? 8 : 0 ) );
		this->scrollBarY->setLength( this->clipHeight );
		this->scrollBarY->setLength2( this->canvasHeight );
		if( !this->scrollBarY->parent )
			this->scrollBarY->setParent( this );
	}
	else if( this->scrollBarY->parent )
			this->scrollBarY->setParent( nullptr );
			
	// Crop children Area
	this->setPadding( _padding( 0 , 0 , needX ? 8 : 0 , needY ? 8 : 0 ) );
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
				this->clipHeight -= 8;
			}
			else if( this->canvasHeight > this->clipHeight )
			{
				this->clipWidth -= 8;
				
				// We might now need a scrollBar
				if( this->canvasWidth > this->clipWidth )
				{
					this->clipHeight -= 8;
				}
			}
			break;
		case _scrollType::scroll:
			this->clipWidth -= 8;
			this->clipHeight -= 8;
			break;
		default:
			break;
	}
}

_gadgetEventReturnType _scrollArea::resizeHandler( _gadgetEvent event )
{
	_scrollArea* that = event.getGadget<_scrollArea>();
	
	if( event.getType() == "refresh" )
	{
		_bitmapPort bP = that->getBitmapPort();
	
		if( event.getArgs().hasClippingRects() )
			bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
		else
			bP.resetClippingRects();
		
		
		bP.fill( COLOR_WHITE );
		
		return use_default;
		
	}
	
	that->computeClipSize();
	
	that->refresh();
	
	that->bubbleRefresh( true );
	
	return handled;
}

void _scrollArea::computeCanvasSize(){
	// Return if there's nothing to do
	if( !this->computeCanvasWidth && !this->computeCanvasHeight )
		return;
	
	int maxX = 0;
	int maxY = 0;
	
	for( _gadget* child : this->nonEnhancedChildren )
	{
		maxX = max( child->getDimensions().getX2() + 1 , maxX );
		maxY = max( child->getDimensions().getY2() + 1 , maxY );
	}
	
	if( this->computeCanvasWidth ){
		maxX += 1 + this->scrollX;
		this->canvasWidth = maxX;
	}
	if( this->computeCanvasHeight ){
		maxY += 1 + this->scrollY;
		this->canvasHeight = maxY;
	}
}

_gadgetEventReturnType _scrollArea::dragHandler( _gadgetEvent event ){
	
	// Receive Gadget
	_scrollArea* that = event.getGadget<_scrollArea>();
	
	_s16 dY = 0 , dX = 0;
	
	static int lastX = 0;
	static int lastY = 0;
	
	if( event.getType() == "dragStart" )
	{
		// fire mouseUp Event on Children
		event.setType( "mouseUp" );
		that->handleEventDefault( event );
		
		// Set Delta
		lastX = event.getArgs().getPosX() + that->getAbsoluteX();
		lastY = event.getArgs().getPosY() + that->getAbsoluteY();
		
		return handled;
	}
	else if( event.getType() == "dragging" )
	{
		
		if( that->scrollTypeX != _scrollType::prevent && that->canvasWidth > that->clipWidth )
		{
			dX = event.getArgs().getPosX() - lastX;
			if( dX < 0 )
			{
				if( that->scrollX - dX > _s16( that->canvasWidth ) + _s16( that->clipWidth + that->scrollX ) )
					dX = 4/dX;
			}
			else if( dX > 0 )
				dX = min( dX , _s16( that->scrollX ) );
			
			that->scrollX -= dX;
		}
		
		if( that->scrollTypeY != _scrollType::prevent && that->canvasHeight > that->clipHeight )
		{
			dY = event.getArgs().getPosY() - lastY;
			if( dY < 0 )
				dY = max( (int)dY , - _s16( that->canvasHeight ) + _s16( that->clipHeight + that->scrollY ) );
			else if( dY > 0 )
				dY = min( dY , _s16( that->scrollY ) );
			
			that->scrollY -= dY;
		}
		
		lastX += dX;
		lastY += dY;
		
		if( dX != 0 || dY != 0 )
		{
			for( _gadget* child : that->nonEnhancedChildren )
			{
				if( child->isEnhanced() )
					continue;
				child->moveRelative( dX , dY );
			}
			
			that->refresh();
		}
		
		return handled;
	}
	
	return use_default;
}

void _scrollArea::setScrollTypeX( _scrollType typeX ){
	this->scrollTypeX = typeX;
	this->computeClipSize();
	this->refresh();
}

void _scrollArea::setScrollTypeY( _scrollType typeY ){
	this->scrollTypeY = typeY;
	this->computeClipSize();
	this->refresh();
}

_scrollArea::_scrollArea( _length width , _length height , _coord x , _coord y , _scrollType scrollTypeX , _scrollType scrollTypeY , _gadgetStyle style ) :
	_gadget( _gadgetType::scrollarea , width , height , x , y , style )
	, scrollX( 0 )
	, scrollY( 0 )
	, canvasWidth( width )
	, canvasHeight( height )
	, computeCanvasWidth( true )
	, computeCanvasHeight( true )
	, scrollTypeX( scrollTypeX )
	, scrollTypeY( scrollTypeY )
	, scrollBarX( new _scrollBar( 0 , 0 , 8 , 0 , 0 , _dimension::horizontal ) )
	, scrollBarY( new _scrollBar( 0 , 0 , 8 , 0 , 0 , _dimension::vertical ) )
{
	this->scrollBarX->setEnhanced();
	this->scrollBarY->setEnhanced();
	
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( "resize" , &_scrollArea::resizeHandler );
	this->registerEventHandler( "dragStart" , &_scrollArea::dragHandler );
	this->registerEventHandler( "dragging" , &_scrollArea::dragHandler );
	this->registerEventHandler( "refresh" , &_scrollArea::resizeHandler );
	
	//this->addChild( new _button( 10 , 90 , 10 , 10 , "Hallo" ) );
	
	// Refresh Me
	this->computeClipSize();
	
	this->refresh();
}

_scrollArea::~_scrollArea(){
	delete this->scrollBarX;
	delete this->scrollBarY;
}