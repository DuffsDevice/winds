#include "_gadget/gadget.scrollBar.button.h"
#include "_resource/BMP_ScrollButtons.h"
_bitmap btn_background_released = BMP_ScrollButton();
_bitmap btn_background_pressed 	= BMP_ScrollButtonPressed();
_pixel 	snip_released_bmp[8] 	= { 64311, 65369, 65368, 65368, 65367, 65367, 64311, 60015 };
_pixel 	snip_pressed_bmp[8]		= { 61008, 63153, 63121, 63122, 63154, 63155, 63187, 64278 };

void drawArrow( _bitmapPort& bP , _scrollButtonType dir )
{
	#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
	static _u8 x[10];
	static _u8 y[10];
	switch( dir )
	{
		case _scrollButtonType::buttonTop :
		case _scrollButtonType::buttonBottom:
			x[0] = 3;x[1] = 4;x[2] = 2;x[3] = 3;x[4] = 4;x[5] = 5;x[6] = 1;x[7] = 2;x[8] = 5;x[9] = 6;
			break;
		case _scrollButtonType::buttonRight:
			x[0] = 5;x[1] = 5;x[2] = 4;x[3] = 4;x[4] = 4;x[5] = 4;x[6] = 3;x[7] = 3;x[8] = 3;x[9] = 3;
			break;
		case _scrollButtonType::buttonLeft:
			x[0] = 2;x[1] = 2;x[2] = 3;x[3] = 3;x[4] = 3;x[5] = 3;x[6] = 4;x[7] = 4;x[8] = 4;x[9] = 4;
			break;
		default:
			break;
	}
	switch( dir )
	{
		case _scrollButtonType::buttonTop:
			y[0] = 2;y[1] = 2;y[2] = 3;y[3] = 3;y[4] = 3;y[5] = 3;y[6] = 4;y[7] = 4;y[8] = 4;y[9] = 4;
			break;
		case _scrollButtonType::buttonBottom:
			y[0] = 5;y[1] = 5;y[2] = 4;y[3] = 4;y[4] = 4;y[5] = 4;y[6] = 3;y[7] = 3;y[8] = 3;y[9] = 3;
			break;
		case _scrollButtonType::buttonRight:
		case _scrollButtonType::buttonLeft:
			y[0] = 3;y[1] = 4;y[2] = 2;y[3] = 3;y[4] = 4;y[5] = 5;y[6] = 1;y[7] = 2;y[8] = 5;y[9] = 6;
			break;
		default:
			break;
	}
	bP.drawPixel( x[0] , y[0] , RGB255( 77 , 97 , 133 ) );
	bP.drawPixel( x[1] , y[1] , RGB255( 77 , 97 , 133 ) );
	bP.drawPixel( x[2] , y[2] , RGB255( 77 , 97 , 133 ) );
	bP.drawPixel( x[3] , y[3] , RGB255( 107 , 129 , 169 ) );
	bP.drawPixel( x[4] , y[4] , RGB255( 107 , 129 , 169 ) );
	bP.drawPixel( x[5] , y[5] , RGB255( 77 , 97 , 133 ) );
	bP.drawPixel( x[6] , y[6] , RGB255( 148 , 164 , 192 ) );
	bP.drawPixel( x[7] , y[7] , RGB255( 107 , 129 , 169 ) );
	bP.drawPixel( x[8] , y[8] , RGB255( 107 , 129 , 169 ) );
	bP.drawPixel( x[9] , y[9] , RGB255( 148 , 164 , 192 ) );
	#pragma GCC diagnostic warning "-Wmaybe-uninitialized"
}

_callbackReturn _scrollButton::refreshHandler( _event event ) {
	
	// Receive Gadget
	_scrollButton* that = event.getGadget<_scrollButton>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().relativate( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	//printf("Hello:%d",that->type);
	
	switch( that->type )
	{
		case _scrollButtonType::buttonTop :
		case _scrollButtonType::buttonLeft :
		case _scrollButtonType::buttonRight :
		case _scrollButtonType::buttonBottom :
		
			if( that->isPressed() )
				bP.copy( 0 , 0 , btn_background_pressed  );
			else
				bP.copy( 0 , 0 , btn_background_released  );
			drawArrow( bP , that->type );
			break;
		case _scrollButtonType::buttonHandleY :
			
			if( that->isPressed() )
			{
				_bitmap bm = _bitmap( snip_pressed_bmp , 8 , 1 );
				bP.copyVerticalStretch( 0 , 2 , myH - 4 , bm );
				
				bP.deleteClippingRects();
				bP.addClippingRects( _rect( 0 , 0 , 8 , 2 ) );
				bP.copy( 0 , 0 , btn_background_pressed );
				
				bP.deleteClippingRects();
				bP.addClippingRects( _rect( 0 , myH - 2 , 8 , 2 ) );
				bP.copy( 0 , myH - 8 , btn_background_pressed );
				
				if( myH > 9 )
				{
					bP.normalizeClippingRects();
					if( myH < 12 )
					{
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 2 , 4 , RGB( 25 , 25 , 31 ) );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 1 , 4 , 60975 );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) , 4 , RGB( 25 , 25 , 31 ) );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) + 1 , 4 , 60975 );
					}
					else
					{
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 3 , 4 , RGB( 25 , 25 , 31 ) );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 2 , 4 , 60975 );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 1 , 4 , RGB( 25 , 25 , 31 ) );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) , 4 , 60975 );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) + 1 , 4 , RGB( 25 , 25 , 31 ) );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) + 2 , 4 , 60975 );
					}
				}
			}
			else
			{
				_bitmap bm = _bitmap( snip_released_bmp , 8 , 1 );
				bP.copyVerticalStretch( 0 , 2 , myH - 4 , bm );
				
				bP.deleteClippingRects();
				bP.addClippingRects( _rect( 0 , 0 , 8 , 2 ) );
				bP.copy( 0 , 0 , btn_background_released );
				
				bP.deleteClippingRects();
				bP.addClippingRects( _rect( 0 , myH - 2 , 8 , 2 ) );
				bP.copy( 0 , myH - 8 , btn_background_released );
				
				if( myH > 9 )
				{
					bP.normalizeClippingRects();
					if( myH < 12 )
					{
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 2 , 4 , RGB( 29 , 29 , 31 ) );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 1 , 4 , 61074 );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) , 4 , RGB( 29 , 29 , 31 ) );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) + 1 , 4 , 61074 );
					}
					else
					{
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 3 , 4 , RGB( 29 , 29 , 31 ) );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 2 , 4 , 61074 );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) - 1 , 4 , RGB( 29 , 29 , 31 ) );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) , 4 , 61074 );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) + 1 , 4 , RGB( 29 , 29 , 31 ) );
						bP.drawHorizontalLine( 2 , ( myH >> 1 ) + 2 , 4 , 61074 );
					}
				}
			}
			break;
			
		case _scrollButtonType::buttonHandleX:
			if( that->isPressed() )
			{
				_bitmap bm = _bitmap( snip_pressed_bmp , 1 , 8 );
				bP.copyHorizontalStretch( 2 , 0 , myW - 4 , bm );
				
				bP.deleteClippingRects();
				bP.addClippingRects( _rect( 0 , 0 , 2 , 8 ) );
				bP.copy( 0 , 0 , btn_background_pressed );
				
				bP.deleteClippingRects();
				bP.addClippingRects( _rect( myW - 2 , 0 , 2 , 8 ) );
				bP.copy( myW - 8 , 0 , btn_background_pressed );
				
				if( myW > 9 )
				{
					bP.normalizeClippingRects();
					if( myW < 12 )
					{
						bP.drawVerticalLine( ( myW >> 1 ) - 2 , 2 , 4 , RGB( 25 , 25 , 31 ) );
						bP.drawVerticalLine( ( myW >> 1 ) - 1 , 2 , 4 , 60975 );
						bP.drawVerticalLine( ( myW >> 1 ) , 2 , 4 , RGB( 25 , 25 , 31 ) );
						bP.drawVerticalLine( ( myW >> 1 ) + 1 , 2 , 4 , 60975 );
					}
					else
					{
						bP.drawVerticalLine( ( myW >> 1 ) - 3 , 2 , 4 , RGB( 25 , 25 , 31 ) );
						bP.drawVerticalLine( ( myW >> 1 ) - 2 , 2 , 4 , 60975 );
						bP.drawVerticalLine( ( myW >> 1 ) - 1 , 2 , 4 , RGB( 25 , 25 , 31 ) );
						bP.drawVerticalLine( ( myW >> 1 ) , 2 , 4 , 60975 );
						bP.drawVerticalLine( ( myW >> 1 ) + 1 , 2 , 4 , RGB( 25 , 25 , 31 ) );
						bP.drawVerticalLine( ( myW >> 1 ) + 2 , 2 , 4 , 60975 );
					}
				}
			}
			else
			{
				_bitmap bm = _bitmap( snip_released_bmp , 1 , 8 );
				bP.copyHorizontalStretch( 2 , 0 , myW - 4 , bm );
				
				bP.deleteClippingRects();
				bP.addClippingRects( _rect( 0 , 0 , 2 , 8 ) );
				bP.copy( 0 , 0 , btn_background_released );
				
				bP.deleteClippingRects();
				bP.addClippingRects( _rect( myW - 2 , 0 , 2 , 8 ) );
				bP.copy( myW - 8 , 0 , btn_background_released );
				
				if( myW > 9 )
				{
					bP.normalizeClippingRects();
					if( myW < 12 )
					{
						bP.drawVerticalLine( ( myW >> 1 ) - 2 , 2 , 4 , RGB( 29 , 29 , 31 ) );
						bP.drawVerticalLine( ( myW >> 1 ) - 1 , 2 , 4 , 61074 );
						bP.drawVerticalLine( ( myW >> 1 ) , 2 , 4 , RGB( 29 , 29 , 31 ) );
						bP.drawVerticalLine( ( myW >> 1 ) + 1 , 2 , 4 , 61074 );
					}
					else
					{
						bP.drawVerticalLine( ( myW >> 1 ) - 3 , 2 , 4 , RGB( 29 , 29 , 31 ) );
						bP.drawVerticalLine( ( myW >> 1 ) - 2 , 2 , 4 , 61074 );
						bP.drawVerticalLine( ( myW >> 1 ) - 1 , 2 , 4 , RGB( 29 , 29 , 31 ) );
						bP.drawVerticalLine( ( myW >> 1 ) , 2 , 4 , 61074 );
						bP.drawVerticalLine( ( myW >> 1 ) + 1 , 2 , 4 , RGB( 29 , 29 , 31 ) );
						bP.drawVerticalLine( ( myW >> 1 ) + 2 , 2 , 4 , 61074 );
					}
				}
			}
			break;
	}
	
	return use_default;
}

_scrollButton::_scrollButton( _length width , _length height , _coord x , _coord y , _scrollButtonType type , _style style ) :
	_button( width , height , x , y , "" , style )
	, type( type )
{
	this->registerEventHandler( refresh , new _staticCallback( _scrollButton::refreshHandler ) );
}