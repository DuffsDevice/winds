// Check if already included
#ifndef _WIN_T_COLOR_
#define _WIN_T_COLOR_

#include "type.h"

class _color{
	
	private:
		
		//! Color
		_pixel	color;
		
		_u16	hue; // from 0 to 360
		_u8		sat; // from 0 to 100 (satt)
		_u8		lum; // from 0 to 100 (hell)
	
		_u8		red; // from 0 to 31
		_u8		gre; // from 0 to 31
		_u8		blu; // from 0 to 31
			
		_u8		maxRGB;
		_u8		minRGB;
		
	public:
		
		//! Constructor
		_color();
		
		//! Copy-Constructor
		_color( _color &color );
		
		//! Extract the Color
		_pixel getColor();
		
		//! Set a RGB-Color to be the edited one
		_color* setColor( _pixel color );
		
		//! Set RGB-Values of the currently edited Color
		_color* setRGB( _u8 red , _u8 green , _u8 blue );
		
		//! Set the HSL-Values of the currently edited Color
		_color* setHSL( _u16 hue , _u8 sat , _u8 lum );
		
		//! Receive the Red part of the color
		_u8 getR();
		
		//! Receive the Green part of the color
		_u8 getG();
		
		//! Receive the Blue part of the color
		_u8 getB();
		
		//! Receive the Saturation of the color
		_u8 getS();
		
		//! Receive the Luminance of the color
		_u8 getL();
		
		//! Receive the Hue of the color
		_u16 getH();
		
		//! Set the Hue of the Color
		_color* setH( _u8 hue );
		
		//! Set the Saturation of the Color
		_color* setS( _u8 sat );
		
		//! Set the Luminace of the Color
		_color* setL( _u8 lum );
		
		//! Set the red part of the Color
		_color* setR( _u8 red );
		
		//! Set the green part of the Color
		_color* setG( _u8 green );
		
		//! Set the blue part of the Color
		_color* setB( _u8 blue );
		
		//! Set the blue part of the Color
		_color* adjustHSL( int hue , int sat , int lum );
		
		//! Set the blue part of the Color
		_color* adjustRGB( _s8 red , _s8 gre , _s8 blu );
};

_color Color( void );

#endif