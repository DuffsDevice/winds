#include "_gadget/gadget.progressbar.h"
#include "func.memory.h"
#include "_type/type.system.h"
#include "_type/type.callback.derives.h"


_pixel paletteBlue[4] = { RGB255( 94 , 119 , 238 ) , RGB255( 125 , 145 , 210 ) , RGB255( 94 , 119 , 238 ) , RGB255( 44 , 62 , 160 ) };
_pixel paletteNormal[4] = { RGB255( 154 , 233 , 156 ) , RGB255( 78 , 218 , 80 ) , RGB255( 46 , 211 , 49 ) , RGB255( 121 , 227 , 123 ) };


_callbackReturn _progressbar::refreshHandler( _event event ){
	
	_progressbar* that = event.getGadget<_progressbar>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_coord myW = bP.getWidth();
	_coord myH = bP.getHeight();
	
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
	
	// Refresh
	this->redraw();
}


void _progressbar::setBarType( bool type )
{
	if( type != this->type )
	{
		this->type = type;
		if( type )
		{
			this->timer.stop();
			this->redraw();
		}
		else
			this->timer.start();
	}
}

_progressbar::_progressbar( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , bool type , _style&& style ) :
	_gadget( _gadgetType::progressbar , x , y , width , 8 , (_style&&)style )
	, type( !type )
	, value( 70 )
	, blue( false )
	, timer( make_callback( this , &_progressbar::step ) , 120 , true ) // Progressbar-update-frequency: 120ms
{	
	this->setBarType( type );
	
	// Register my handler as the default Refresh-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_progressbar::refreshHandler ) );
	
	// refresh!
	this->redraw();
}