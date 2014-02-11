// Check if already included
#ifndef _WIN_T_COLOR_
#define _WIN_T_COLOR_

#include "type.h"

class _color{
	
	private:
		
		//! Color as 555-Triplet
		_pixel	color;
		
		/**
		 * Hue: 0 to 360
		 * Saturation: 0 to 100 (very saturated)
		 * Luminance: 0 to 100 (bright)
		 *
		 * Red, Green, Blue: 0 to 31
		**/
		
		_u8		minRGB;
		_u8		maxRGB;
		
	public:
		
		//! Constructor
		_color() :
			color( 0 )
			, minRGB( 0 )
			, maxRGB( 0 )
		{}
		
		//! Constructor taking an initial Color
		_color( _pixel col ){
			this->setColor( col );
		}
		
		//! Named Ctor: Create Color from 5bit rgb values
		static _color fromRGB( _u8 red , _u8 green , _u8 blue ){
			return _color().setRGB( red , green , blue );
		}
		
		//! Create color from HSL Values
		static _color fromHSL( _u16 hue , _u16 sat , _u16 lum ){
			return _color().setHSL( hue , sat , lum );
		}
		
		//! Extract the raw Color value (convert to _pixel)
		_pixel getColor() const { return this->color; }
		operator _pixel() const { return this->color; }
		
		//! Set a RGB-Color value
		_color& setColor( _pixel col ){
			this->setRGB( RGB_GETR( col ) , RGB_GETG( col ) , RGB_GETB( col ) );
			return *this;
		}
		
		//! Set RGB-Values of the currently edited Color
		_color& setRGB( _u8 red , _u8 green , _u8 blue );
		
		//! Set the HSL-Values of the currently edited Color
		_color& setHSL( _u16 hue , _u16 sat , _u16 lum );
		
		//! Receive the Red part of the color
		_u8 getR() const {
			return RGB_GETR( this->color );
		}
		
		//! Receive the Green part of the color
		_u8 getG() const {
			return RGB_GETG( this->color );
		}
		
		//! Receive the Blue part of the color
		_u8 getB() const {
			return RGB_GETB( this->color );
		}
		
		//! Receive the Saturation of the color
		_u16 getS() const 
		{
			if( this->maxRGB != 0 )
				return ( ( this->maxRGB - this->minRGB ) * 100 + ( this->maxRGB >> 1 ) ) / this->maxRGB;
			return 0;
		}
		
		//! Receive the Luminance of the color
		_u16 getL() const {
			return ( ( this->maxRGB + this->minRGB ) * 100 + 31 ) / 62;
		}
		
		//! Receive the Hue of the color
		_u16 getH();
		
		//! Set the Hue of the Color
		_color& setH( _u16 hue ){
			this->setHSL( hue , this->getS() , this->getL() );
			return *this;
		}
		
		//! Set the Saturation of the Color
		_color& setS( _u16 sat ){
			this->setHSL( this->getH() , sat , this->getL() );
			return *this;
		}
		
		//! Set the Luminace of the Color
		_color& setL( _u16 lum ){
			this->setHSL( this->getH() , this->getS() , lum );
			return *this;
		}
		
		//! Set the red part of the Color
		_color& setR( _u8 red ){
			this->setRGB( red , this->getG() , this->getB() );
			return *this;
		}
		
		//! Set the green part of the Color
		_color& setG( _u8 green ){
			this->setRGB( this->getR() , green , this->getB() );
			return *this;
		}
		
		//! Set the blue part of the Color
		_color& setB( _u8 blue ){
			this->setRGB( this->getR() , this->getG() , blue );
			return *this;
		}
		
		//! Set the blue part of the Color
		_color& adjustHSL( int hue , int sat , int lum );
		
		//! Set the blue part of the Color
		_color& adjustRGB( _s8 red , _s8 green , _s8 blu );
};

extern _map<string,_pixel> string2color;

#endif