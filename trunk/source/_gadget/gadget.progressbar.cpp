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
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
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
	_u32 time = _system::getTime();
	if( time - this->lastTime > 150 )
	{
		if( ++this->state >= ( ( this->getWidth() - 2 ) >> 2 ) + 3 )
			this->state = 0;
		this->lastTime = time;
		bubbleRefresh( true );
	}
}

//if( val == this->state ) return; this->state = val; this->bubbleRefresh( true ); }

#define method_cast( t ) reinterpret_cast<void (unknownClass::*)()>(t)
#define class_cast( t ) reinterpret_cast<unknownClass*>(t)

_progressbar::_progressbar( _length width , _length height , _coord x , _coord y  , bool type , _style style ) :
	_gadget( _gadgetType::progressbar , width , 8 , x , y , style )
	, type( type )
	, value( 70 )
	, blue( false )
	, lastTime( _system::getTime() )
{
	_system::addVblListener( new _classCallback( class_cast( this ) , method_cast( &_progressbar::step ) ) );
	
	// Register my handler as the default Refresh-Handler
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( refresh , &_progressbar::refreshHandler );
	
	// refresh!
	this->refreshBitmap();
}

_progressbar::_progressbar( _length width , _coord x , _coord y  , bool type , _style style ) : 
	_progressbar( width , 8 , x , y , type , style )
{ }

_progressbar::~_progressbar(){
	_system::removeVblListener( _classCallback( class_cast( this ) , method_cast( &_progressbar::step ) ) );
}

	