#include "_gadget/gadget.clockgadget.h"
#include <math.h>
#define M_PI		3.14159265358979323846f

void _clockgadget::setTime( _u8 hour , _u8 minutes , _u8 seconds )
{
	// Check if something needs to be refreshed
	if( this->hours == hour && this->minutes == minutes && this->seconds == seconds )
		return;
	
	this->hours = hour;
	this->minutes = minutes;
	this->seconds = seconds;
	
	// Refresh!
	this->redraw();
}

_callbackReturn _clockgadget::refreshHandler( _event event )
{
	// Receive Gadget
	_clockgadget* that = event.getGadget<_clockgadget>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	_coord centerX = ( myW - 1 ) >> 1;
	_coord centerY = ( myH - 1 ) >> 1;
	_length radius = min( centerX , centerY );
	
	// Reset Clock-Image
	bP.fill( that->outerBgColor );
	bP.drawFilledCircle( centerX , centerY , radius , that->innerBgColor );
	
	// Dots
	for( float i = 0 ; i < 4 ; i+= 1 )
	{
		float co = cos( i * 30 / 180 * M_PI );
		float si = sin( i * 30 / 180 * M_PI );
		int c1 = co * (radius-1);
		int s1 = si * (radius-1);
		int c2 = co * radius;
		int s2 = si * radius;
		bP.drawPixel( centerX - c1 , centerY - s1 , COLOR_WHITE );
		bP.drawPixel( centerX + c1 , centerY - s1 , COLOR_WHITE );
		bP.drawPixel( centerX - c1 , centerY + s1 , COLOR_WHITE );
		bP.drawPixel( centerX + c1 , centerY + s1 , COLOR_WHITE );
		bP.drawPixel( centerX - c2 , centerY - s2 , COLOR_WHITE );
		bP.drawPixel( centerX + c2 , centerY - s2 , COLOR_WHITE );
		bP.drawPixel( centerX - c2 , centerY + s2 , COLOR_WHITE );
		bP.drawPixel( centerX + c2 , centerY + s2 , COLOR_WHITE );
	}
	
	// Second
	bP.drawLine( centerX , centerY
		, centerX - cos( float( that->seconds + 15 ) * 6 / 180 * M_PI ) * (radius-2) + 0.5
		, centerY - sin( float( that->seconds + 15 ) * 6 / 180 * M_PI ) * (radius-2) + 0.5
		, that->secondsColor
	);
	
	float minAngleInRad = ( float( that->minutes + 15 ) * 6 + float( that->seconds ) / 10 ) / 180 * M_PI;
	
	// Minute
	bP.drawLine( centerX , centerY
		, centerX - cos( float( minAngleInRad ) ) * (radius-5) + 0.5
		, centerY - sin( float( minAngleInRad ) ) * (radius-5) + 0.5
		, that->minutesColor
	);
	
	float hrAngleInRad = ( float( that->hours + 3 ) * 30 + float( that->minutes ) / 2 ) / 180 * M_PI;
	int hoursLength = ( radius + 2 ) >> 1;
	
	// Hour
	bP.drawLine( centerX , centerY 
		, centerX - cos( hrAngleInRad ) * hoursLength + 0.5
		, centerY - sin( hrAngleInRad ) * hoursLength + 0.5
		, that->hoursColor
	);
	
	return use_default;
}

void _clockgadget::timerCallback()
{
	this->seconds++;
	if( this->seconds > 59 ) // Check seconds overflow
	{
		this->minutes++;
		if( this->minutes > 59 ) // Check minutes overflow
		{
			this->hours++;
			if( this->hours > 23 )
				this->hours -= 24;
			this->minutes -= 60;
		}
		this->seconds -= 60;
	}
	
	// Show that something changed!
	this->triggerEvent( onEdit );
	
	// Update image
	this->redraw();
}

_clockgadget::_clockgadget( _length width , _length height , _coord x , _coord y , _u8 hours , _u8 minutes , _u8 seconds , bool autoIncrease , _style&& style ) :
	_gadget( _gadgetType::clockimage , width , height , x , y , style | _styleAttr::notClickable )
	, outerBgColor( COLOR_TRANSPARENT )
	, innerBgColor( COLOR_WHITE )
	, secondsColor( COLOR_RED )
	, minutesColor( COLOR_BLACK )
	, hoursColor( COLOR_BLACK )
	, markingsColor( COLOR_GRAY )
	, seconds( seconds )
	, minutes( minutes )
	, hours( hours )
	, timer( make_callback( this , &_clockgadget::timerCallback ) , 1000 , true )
{
	if( autoIncrease )
		this->timer.start();
	
	// Register my handlers
	this->setInternalEventHandler( onDraw , make_callback( &_clockgadget::refreshHandler ) );
	
	// Refresh
	this->redraw();
}