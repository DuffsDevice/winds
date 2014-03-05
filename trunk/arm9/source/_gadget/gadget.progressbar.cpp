#include "_gadget/gadget.progressbar.h"
#include "func.memory.h"
#include "_type/type.system.h"
#include "_type/type.callback.derives.h"


_pixel paletteBlue[4] = {
	_color::fromRGB8( 94 , 119 , 238 )
	, _color::fromRGB8( 125 , 145 , 210 )
	, _color::fromRGB8( 94 , 119 , 238 )
	, _color::fromRGB8( 44 , 62 , 160 )
};
_pixel paletteNormal[4] = {
	_color::fromRGB8( 154 , 233 , 156 )
	, _color::fromRGB8( 78 , 218 , 80 )
	, _color::fromRGB8( 46 , 211 , 49 )
	, _color::fromRGB8( 121 , 227 , 123 )
};


_callbackReturn _progressBar::refreshHandler( _event event ){
	
	_progressBar* that = event.getGadget<_progressBar>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_coord myW = bP.getWidth();
	_coord myH = bP.getHeight();
	
	bP.drawRect( 0 , 0 , myW , myH , _color::fromRGB( 16 , 16 , 16 ) );
	
	bP.drawPixel( 0 , 0 , _color::transparent );
	bP.drawPixel( 0 , myH - 1 , _color::transparent );
	bP.drawPixel( myW - 1 , 0 , _color::transparent );
	bP.drawPixel( myW - 1 , myH - 1 , _color::transparent );
	bP.drawFilledRect( 1 , 1 , myW - 2 , myH - 2 , that->blue ? _color::black : _color::white );
	
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


void _progressBar::step()
{
	if( ++this->state >= ( ( this->getWidth() - 2 ) >> 2 ) + 3 )
		this->state = 0;
	
	// Refresh
	this->redraw();
}


void _progressBar::setBarType( bool type )
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

_progressBar::_progressBar( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , bool type , _style&& style ) :
	_gadget( _gadgetType::progressbar , x , y , width , 8 , style | _style::notClickable )
	, type( !type )
	, value( 70 )
	, blue( false )
	, timer( make_callback( this , &_progressBar::step ) , 120 , true ) // Progressbar-update-frequency: 120ms
{	
	this->setBarType( type );
	
	// Register my handler as the default Refresh-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_progressBar::refreshHandler ) );
	
	// refresh!
	this->redraw();
}