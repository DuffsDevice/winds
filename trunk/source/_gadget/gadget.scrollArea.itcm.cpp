#include "_gadget/gadget.scrollArea.h"
#include "_gadget/gadget.button.h"
#include "_graphic/BMP_ScrollButtons.h"
#include "nds/arm9/math.h"

const _bitmap icon_scroll_bg = BMP_ScrollBg();
const _bitmap icon_scroll_bg_ = BMP_ScrollBg_();

void _scrollArea::addChild( _gadget* child ){
	child->moveRelative( - this->scrollX , - this->scrollY );
	
	_gadget::addChild( child );
	if( !child->isEnhanced() )
		this->nonEnhancedChildren.push_back( child );
	else
		return;
	
	// Re-compute Scrollbar and innerSize
	this->computeInnerSize();
	this->computeRatio();
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
	this->computeInnerSize();
	this->computeRatio();
}

void _scrollArea::onResize(){
	// Move Buttons
	this->buttonTop->setX( this->dimensions.width - 8 );
	this->buttonHandleY->setX( this->dimensions.width - 8 );
	this->buttonBottom->moveTo( this->dimensions.width - 8 , this->dimensions.height - 8 );
	this->buttonRight->moveTo( this->dimensions.width - ( !this->scrollTypeY ? 17 : 8 ) , this->dimensions.height - 8 );
	this->buttonHandleX->setY( this->dimensions.height - 8 );
	this->buttonLeft->setY( this->dimensions.height - 8 );
	
	// Additional data...
	this->visibleWidth = this->dimensions.width - ( !this->scrollTypeY ? 8 : 0 );
	this->visibleHeight = this->dimensions.height - ( !this->scrollTypeX ? 8 : 0 );
	
	// Re-compute Data...
	this->computeRatio();
	this->bubbleRefresh( true );
}

void _scrollArea::computeRatio(){
	this->_ratioWidth_ = div32( ( this->dimensions.width - ( !this->scrollTypeY ? 23 : 15 ) << 16 ) , this->innerWidth );
	this->_ratioHeight_ = div32( ( this->dimensions.height - 15 << 16 ) , this->innerHeight );
	this->buttonHandleX->setWidth( this->getWidthRatio( this->visibleWidth ) );
	this->buttonHandleY->setHeight( this->getHeightRatio( this->visibleHeight ) );
}

inline void _scrollArea::moveScrollHandleX(){
	if( !this->scrollTypeX )
		this->buttonHandleX->setX( 8 + this->getWidthRatio( this->scrollX ) );
}

inline void _scrollArea::moveScrollHandleY(){
	if( !this->scrollTypeY )
		this->buttonHandleY->setY( 8 + this->getHeightRatio( this->scrollY ) );
}

void _scrollArea::computeInnerSize(){
	// Return if there's nothing to do
	if( !this->computeInnerWidth && this->computeInnerHeight )
		return;
	
	int maxX = 0;
	int maxY = 0;
	
	for( _gadget* child : this->nonEnhancedChildren )
	{
		maxX = max( child->getDimensions().getX2() + 1 , maxX );
		maxY = max( child->getDimensions().getY2() + 1 , maxY );
	}
	
	if( this->computeInnerWidth ){
		maxX += 1 + this->scrollX;
		this->innerWidth = maxX;
	}
	if( this->computeInnerHeight ){
		maxY += 1 + this->scrollY;
		this->innerHeight = maxY;
	}
	this->hideOrShowScrollButtons();
}

_gadgetEventReturnType _scrollArea::dragHandler( _gadgetEvent event ){
	
	// Receive Gadget
	_scrollArea* that = (_scrollArea*)event.getGadget();
	
	static int lastX = 0;
	static int lastY = 0;
	
	if( event.getType() == dragStart )
	{
		// fire mouseUp Event on Children
		event.setType( mouseUp );
		that->handleEventDefault( event );
		
		// Set Delta
		lastX = event.getArgs().getPosX() + that->getAbsoluteX();
		lastY = event.getArgs().getPosY() + that->getAbsoluteY();
		
		return handled;
	}
	else if( event.getType() == dragging )
	{
		_s16 dY = 0 , dX = 0;
		
		if( that->scrollTypeX != _scrollType::prevent && that->innerWidth > that->visibleWidth )
		{
			dX = event.getArgs().getPosX() - lastX;
			if( dX < 0 )
				dX = max( (int)dX , - _s16( that->innerWidth ) + _s16( that->visibleWidth + that->scrollX ) );
			else if( dX > 0 )
				dX = min( dX , _s16( that->scrollX ) );
			
			that->scrollX -= dX;
			
			that->moveScrollHandleX();
		}
		
		if( that->scrollTypeY != _scrollType::prevent && that->innerHeight > that->visibleHeight )
		{
			dY = event.getArgs().getPosY() - lastY;
			if( dY < 0 )
				dY = max( (int)dY , - _s16( that->innerHeight ) + _s16( that->visibleHeight + that->scrollY ) );
			else if( dY > 0 )
				dY = min( dY , _s16( that->scrollY ) );
			
			that->scrollY -= dY;
			
			that->moveScrollHandleY();
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
		}
		
		return handled;
	}
	
	return use_default;
}

_gadgetEventReturnType _scrollArea::refreshHandler( _gadgetEvent event ){
	
	// Receive Gadget
	_scrollArea* that = (_scrollArea*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	bP.fill( RGB( 31 , 31 , 31 ) );
	
	// Show Scrollbar-Backgrounds
	if( that->buttonHandleX->getParent() )
		bP.copyHorizontalStretch( 8 , myH - 8 , myW - 17 - 9 , &icon_scroll_bg );
	
	if( that->buttonHandleY->getParent() )
		bP.copyVerticalStretch( myW - 8 , 8 , myH - 9 - 8 , &icon_scroll_bg_ );
	
	return use_default;
}

void _scrollArea::setScrollTypeX( _scrollType typeX ){
	this->scrollTypeX = typeX;
	this->hideOrShowScrollButtons();
}

void _scrollArea::setScrollTypeY( _scrollType typeY ){
	this->scrollTypeY = typeY;
	this->hideOrShowScrollButtons();
}

void _scrollArea::hideOrShowScrollButtons()
{
	if( !this->scrollTypeY && this->innerHeight > this->visibleHeight )
	{
		if( this->buttonTop->getParent() )
			goto breakup;
		this->addChild( this->buttonTop );
		this->addChild( this->buttonBottom );
		this->addChild( this->buttonHandleY );
	}
	else if( this->buttonTop->getParent() )
	{
		this->removeChild( this->buttonTop );
		this->removeChild( this->buttonBottom );
		this->removeChild( this->buttonHandleY );
	}
	
	breakup:
	
	if( !this->scrollTypeX && this->innerWidth > this->visibleWidth )
	{
		if( this->buttonRight->getParent() )
			goto breakupTwo;
		this->addChild( this->buttonRight );
		this->addChild( this->buttonLeft );
		this->addChild( this->buttonHandleX );
	}
	else if( this->buttonRight->getParent() )
	{
		this->removeChild( this->buttonRight );
		this->removeChild( this->buttonLeft );
		this->removeChild( this->buttonHandleX );
	}
	
	// Breakpoint
	breakupTwo:
	
	// Crop children Area
	this->setPadding( _padding( 0 , 0 , !this->scrollTypeY ? 8 : 0 , !this->scrollTypeX ? 8 : 0 ) );
	
	this->onResize();
}

_scrollArea::_scrollArea( _length width , _length height , _coord x , _coord y , _scrollType scrollTypeX , _scrollType scrollTypeY , _gadgetStyle style ) :
	_gadget( _gadgetType::scrollarea , width , height , x , y , style )
	, scrollX( 0 )
	, scrollY( 0 )
	, innerWidth( width )
	, innerHeight( height )
	, computeInnerWidth( true )
	, computeInnerHeight( true )
	, scrollTypeX( scrollTypeX )
	, scrollTypeY( scrollTypeY )
	, buttonTop( new _scrollButton( _scrollButtonType::buttonTop , this->dimensions.width - 8 , 0 ) )
	, buttonBottom( new _scrollButton( _scrollButtonType::buttonBottom , this->dimensions.width - 8 , this->dimensions.height - 8 ) )
	, buttonRight( new _scrollButton( _scrollButtonType::buttonRight , this->dimensions.width - ( !this->scrollTypeY ? 17 : 8 ) , this->dimensions.height - 8 ) )
	, buttonLeft( new _scrollButton( _scrollButtonType::buttonLeft , 0 , this->dimensions.height - 8 ) )
	, buttonHandleX( new _scrollButton( _scrollButtonType::buttonHandleX , 12 , this->dimensions.height - 8 , 20) )
	, buttonHandleY( new _scrollButton( _scrollButtonType::buttonHandleY , this->dimensions.width - 8 , 12 , 14) )
{
	this->buttonTop->setEnhanced();
	this->buttonBottom->setEnhanced();
	this->buttonHandleX->setEnhanced();
	this->buttonHandleY->setEnhanced();
	this->buttonRight->setEnhanced();
	this->buttonLeft->setEnhanced();
	
	this->computeInnerSize();
	
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( refresh , &_scrollArea::refreshHandler );

	this->registerEventHandler( dragStart , &_scrollArea::dragHandler );
	this->registerEventHandler( dragging , &_scrollArea::dragHandler );
	this->registerEventHandler( dragStop , &_scrollArea::dragHandler );
	
	// Refresh Me
	this->hideOrShowScrollButtons();
	
	//this->addChild( new _button( 30 , 120 , "HEllo" ) );
}

_scrollArea::~_scrollArea(){
	delete this->buttonTop;
	delete this->buttonBottom;
	delete this->buttonRight;
	delete this->buttonLeft;
	delete this->buttonHandleX;
	delete this->buttonHandleY;
}