#include "_gadget/gadget.progressbar.h"
#include "func.memory.h"
#include "_type/type.system.h"
#include "_type/type.callback.derives.h"


_pixel paletteBlue[4] = { RGB255( 94 , 119 , 238 ) , RGB255( 125 , 145 , 210 ) , RGB255( 94 , 119 , 238 ) , RGB255( 44 , 62 , 160 ) };
_pixel paletteNormal[4] = { RGB255( 154 , 233 , 156 ) , RGB255( 78 , 218 , 80 ) , RGB255( 46 , 211 , 49 ) , RGB255( 121 , 227 , 123 ) };


_callbackReturn _progressbar::refreshHandler( _event event ){
	
	_progressbar* that = event.getGadget<_progressbar>();
	
	_bitmapPort bP = that->getBitmapPort();
	_coord myW = bP.getWidth();
	_coord myH = bP.getHeight();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
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
				bP.copy( x , 2 , bmp );
		}
	}
	else
	{
		_coord x = ( that->state - 3 ) * 4;
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
				bP.copy( x , 2 , bmp );
		}
	}
	
	end:
	
	return handled;
}


void _progressbar::step()
{
	if( ++this->state >= ( ( this->getWidth() - 2 ) >> 2 ) + 3 )
		this->state = 0;
	bubbleRefresh( true );
}


void _progressbar::setBarType( bool type )
{
	if( type != this->type )
	{
		this->type = type;
		if( type )
		{
			_system::terminateTimer( _classCallback( this , &_progressbar::step ) );
			this->bubbleRefresh( true );
		}
		else
			_system::executeTimer( _classCallback( this , &_progressbar::step ) , 120 , true ); // Progressbar-update-frequency: 120ms
	}
}

_progressbar::_progressbar( _length width , _coord x , _coord y  , bool type , _style&& style ) :
	_gadget( _gadgetType::progressbar , width , 8 , x , y , (_style&&)style )
	, type( !type )
	, value( 70 )
	, blue( false )
{	
	this->setBarType( type );
	
	// Register my handler as the default Refresh-Handler
	this->setInternalEventHandler( refresh , _staticCallback( &_progressbar::refreshHandler ) );
	
	// refresh!
	this->refreshBitmap();
}


_progressbar::~_progressbar(){
	_system::terminateTimer( _classCallback( this , &_progressbar::step ) );
}