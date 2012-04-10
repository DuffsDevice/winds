#include "_gadget/gadget.scrollArea.buttons.h"
#include "_graphic/BMP_ScrollButtons.h"

const _bitmap icon_scroll_top = BMP_ScrollButtonTop();
const _bitmap icon_scroll_bottom = BMP_ScrollButtonBottom();
const _bitmap icon_scroll_right = BMP_ScrollButtonRight();
const _bitmap icon_scroll_left = BMP_ScrollButtonLeft();

_pixel palette[5] = { RGB255( 222 , 231 , 247 ) , RGB255( 206 , 222 , 255 ) , RGB255( 198 , 214 , 255 ) , RGB255( 181 , 206 , 247 ) , RGB255( 206 , 222 , 231 ) };
_pixel line_bmp[8] = { palette[0] , palette[2] , palette[2] , palette[2] , palette[2] , palette[3] , palette[3] , palette[4] };

const _bitmap line = { line_bmp , 8 , 1 };
const _bitmap line_ = { line_bmp , 1 , 8 };

void _scrollButton::onResize(){
	if( this->type < _scrollButtonType::buttonHandleX )
		return;
	this->bubbleRefresh( true );
}

_gadgetEventReturnType _scrollButton::refreshHandler( _gadgetEvent event ) {
	// Receive Gadget
	_scrollButton* that = (_scrollButton*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().isBubbleRefresh() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	switch( that->type ){
		case _scrollButtonType::buttonTop:
			bP.copy( 0 , 0 , &icon_scroll_top );
			break;
		case _scrollButtonType::buttonBottom:
			bP.copy( 0 , 0 , &icon_scroll_bottom );
			break;
		case _scrollButtonType::buttonRight:
			bP.copy( 0 , 0 , &icon_scroll_right );
			break;
		case _scrollButtonType::buttonLeft:
			bP.copy( 0 , 0 , &icon_scroll_left );
			break;
		case _scrollButtonType::buttonHandleY:
			bP.drawPixel( 1 , 0 , palette[0] );
			bP.drawPixel( 2 , 0 , palette[1] );
			bP.drawPixel( 3 , 0 , palette[1] );
			bP.drawPixel( 4 , 0 , palette[2] );
			bP.drawPixel( 5 , 0 , palette[2] );
			bP.drawPixel( 6 , 0 , palette[3] );
			bP.copyVerticalStretch( 0 , 1 , myH - 2 , &line );
			bP.drawHorizontalLine( 1 , myH - 1 , 6 , palette[0] );
			break;
		case _scrollButtonType::buttonHandleX:
			bP.drawPixel( 0 , 1 , palette[0] );
			bP.drawPixel( 0 , 2 , palette[1] );
			bP.drawPixel( 0 , 3 , palette[1] );
			bP.drawPixel( 0 , 4 , palette[2] );
			bP.drawPixel( 0 , 5 , palette[2] );
			bP.drawPixel( 0 , 6 , palette[3] );
			bP.copyHorizontalStretch( 1 , 0 , myW - 2 , &line_ );
			bP.drawVerticalLine( myW - 1 , 1 , 6 , palette[0] );
			break;
		default:
			break;
	}
	
	return use_default;
}

_scrollButton::_scrollButton( _scrollButtonType type , _coord x , _coord y , _length length , _gadgetStyle style ) :
	_gadget( _gadgetType::scrollbutton , 8 , 8 , x , y , style )
	, type( type )
{
	if( this->type == _scrollButtonType::buttonHandleY )
		this->setHeight( length );
	else if( this->type == _scrollButtonType::buttonHandleX )
		this->setWidth( length );
	
	// Reister Event Handlers...
	this->registerEventHandler( refresh , &_scrollButton::refreshHandler );
	
	// Refresh my bitmap
	this->refreshBitmap();
}