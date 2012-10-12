#include "_gadget/gadget.progressbar.h"
#include "func.memory.h"

_pixel paletteBlue[4] = { RGB255( 94 , 119 , 238 ) , RGB255( 125 , 145 , 210 ) , RGB255( 94 , 119 , 238 ) , RGB255( 44 , 62 , 160 ) };
_pixel paletteNormal[4] = { RGB255( 154 , 233 , 156 ) , RGB255( 78 , 218 , 80 ) , RGB255( 46 , 211 , 49 ) , RGB255( 121 , 227 , 123 ) };

_gadgetEventReturnType _progressbar::refreshHandler( _gadgetEvent event ){
	
	_progressbar* that = event.getGadget<_progressbar>();

	// Run animation
	if( !that->type && that->anim->finished() )
		that->anim->start();
	else if( that->type && !that->anim->finished() )
		that->anim->terminate();
	
	_bitmapPort bP = that->getBitmapPort();
	_coord myW = bP.getWidth();
	_coord myH = bP.getHeight();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.resetClippingRects();
	
	bP.drawRect( 0 , 0 , myW , myH , RGB( 16 , 16 , 16 ) );
	
	bP.drawPixel( 0 , 0 , COLOR_TRANSPARENT );
	bP.drawPixel( 0 , myH - 1 , COLOR_TRANSPARENT );
	bP.drawPixel( myW - 1 , 0 , COLOR_TRANSPARENT );
	bP.drawPixel( myW - 1 , myH - 1 , COLOR_TRANSPARENT );
	bP.drawFilledRect( 1 , 1 , myW - 2 , myH - 2 , that->blue ? COLOR_BLACK : COLOR_WHITE );
	
	_pixelArray palette = nullptr;
	
	if( that->blue )
		palette = paletteBlue;
	else
		palette = paletteNormal;
	
	_bitmap bmp = _bitmap( palette , 1 , 4 );
	
	if( that->type )
	{
		_length pixelsToPaint = that->value * ( myW - 2 ) >> 7;
		
		for( int f = 3,x = 1; pixelsToPaint ; pixelsToPaint--,x++ )
		{
			if( ++f == 4 )
				f = 0;
			else
				bP.copy( x , 2 , &bmp );
		}
	}
	else
	{
		_length bars = ( myW - 2 + 3 ) >> 2;
		_coord x = ( ( that->state * ( bars + 3 ) >> 7 ) - 3 ) * 4;
		x++;
		_u8 cnt = 4;
		for( int f = 3; x < myW - 2 ; x++ )
		{
			if( ++f == 4 )
			{
				if( !--cnt )
					goto end;
				f = 0;
			}
			else if( x > 1 )
				bP.copy( x , 2 , &bmp );
		}
	}
	
	end:
	
	return handled;
}

_progressbar::_progressbar( _length width , _length height , _coord x , _coord y  , bool type , _gadgetStyle style ) :
	_gadget( _gadgetType::progressbar , width , 8 , x , y , style )
	, type( type )
	, value( 70 )
	, anim( new _animation( 0 , 127 , 1800 ) )
	, blue( false )
{
	anim->setter( [&]( int val ){ if( val == this->state ) return; this->state = val; this->bubbleRefresh( true ); } );
	anim->finish( [&]( int val ){ this->refreshBitmap(); } );
	
	// Register my handler as the default Refresh-Handler
	this->unregisterEventHandler( "mouseDoubleClick" );
	this->registerEventHandler( "refresh" , &_progressbar::refreshHandler );
	
	// refresh!
	this->refreshBitmap();
}

_progressbar::_progressbar( _length width , _coord x , _coord y  , bool type , _gadgetStyle style ) : 
	_progressbar( width , 8 , x , y , type , style )
{ }

_progressbar::~_progressbar(){
	delete this->anim;
}

	