// Check if already included
#ifndef _WIN_T_COLOR_
#define _WIN_T_COLOR_

#include <_type/type.h>

class _color{
	
	private:
	
		struct _colorLab{
			float l, a, b;
		};
		
		//! Color as X555-Triplet (A-R-G-B)
		_pixel	color;
		
		/**
		 * Hue: 0 to 360
		 * Saturation: 0 to 100 (very saturated)
		 * Luminance: 0 to 100 (bright)
		 *
		 * Red, Green, Blue: 0 to 31
		**/
		
		void setRGBInternal( _u8 red , _u8 green , _u8 blue , bool alpha );
		void setHSLInternal( _u16 hue , _u16 sat , _u16 lum , bool alpha );
		
		_color mulDiv32( int mul ) const {
			return _color::fromRGB(
				_u16( this->getR() * mul ) / 32
				, _u16( this->getG() * mul ) / 32
				, _u16( this->getB() * mul ) / 32
			);
		}
		_color operator+( const _color& col ) const {
			return _color::fromRGB(
				this->getR() + col.getR()
				, this->getG() + col.getG()
				, this->getB() + col.getB()
			);
		}
		
		//! Converts the color into Lab-ColorSpace
		_colorLab toLab() const ;
		
		//! Determines the color-distance between two colors (CIE94)
		static _u16 distanceInternalCIE94( _colorLab col1 , _colorLab col2 );
		
		//! Determines the color-distance between two colors (RGB) (fast but inaccurate)
		static _u16 distanceInternalRGB( _color col1 , _color col2 );
		
	public:
	
		//! Static Colors
		static const _color white;
		static const _color silver;
		static const _color gray;
		static const _color black;
		static const _color red;
		static const _color maroon;
		static const _color orange;
		static const _color yellow;
		static const _color olive;
		static const _color lime;
		static const _color green;
		static const _color aqua;
		static const _color teal;
		static const _color blue;
		static const _color navy;
		static const _color fuchsia;
		static const _color purple;
		static const _color transparent;
		
		//! Constructor
		constexpr _color() :
			color( 0 )
		{}
		
		//! Constructor taking an initial Color
		constexpr _color( _pixel col ) :
			color( col )
		{}
		
		//! Constructer to create color from color-name
		//! @note http://www.w3.org/TR/CSS2/syndata.html#color-units
		_color( const string& name );
		
		
		//! Standards: Copy & Move Ctors, Assignment operators
		_color( _color&& ) = default;
		_color( const _color& ) = default;
		_color& operator=( _color&& ) = default;
		_color& operator=( const _color& ) = default;
		
		//! Comparison Operators
		bool operator==( const _color& col ) const { return col.color == this->color; }
		bool operator!=( const _color& col ) const { return col.color != this->color; }
		
		/////////////////
		// Named Ctors //
		/////////////////
		
		//! Named Ctor: Create Color from 24bit HEX Value
		static _color fromHex( _u32 value , bool alpha = true ){
			return
				( ( value & 0xff0000 ) >> 19 )
				| ( ( ( value & 0x00ff00 ) >> 11 ) << 5 )
				| ( ( ( value & 0x0000ff ) >> 3 ) << 10 )
				| ( alpha << 15 )
			;
		}
		
		//! Named Ctor: Create Color from 5bit brightness value (0-31)
		static _color fromBW( _u8 brightness , bool alpha = true ){
			return
				_u16( brightness ) * ( 1 + ( 1 << 5 ) + ( 1 << 10 ) )
				| ( alpha << 15 )
			;
		}
		
		//! Named Ctor: Create Color from 8bit brightness value (0-255)
		static _color fromBW8( _u8 brightness , bool alpha = true ){
			return
				_u16( ( brightness + ( 1 << 2 ) ) >> 3 ) * ( 1 + ( 1 << 5 ) + ( 1 << 10 ) )
				| ( alpha << 15 )
			;
		}
		
		//! Named Ctor: Create Color from 5bit rgb values and 1bit alpha channel
		static _color fromRGB( _u8 red , _u8 green , _u8 blue , bool alpha = true ){
			return _pixel(
				red
				| ( green << 5 )
				| ( blue << 10 )
				| ( alpha << 15 )
			);
		}
		
		//! Named Ctor: Creat color from 8bit RGB Values and a 1bit alpha channel
		static _color fromRGB8( _u8 red , _u8 green , _u8 blue , bool alpha = true ){
			return _pixel(
				( red >> 3 )
				| ( ( green >> 3 ) << 5 )
				| ( ( blue >> 3 ) << 10 )
				| ( alpha << 15 )
			);
		}
		
		//! Create color from HSL Values and 1bit alpha
		static _color fromHSL( _u16 hue , _u16 sat , _u16 lum , bool alpha = true ){
			return _color().setHSL( hue , sat , lum , alpha );
		}
		
		//! Mix two colors; factor: 0 (only color1) to 32 (only color2)
		static _color mix( _color color1 , _color color2 , _u8 factor = 16 ){
			if( factor == 0 )
				return color1;
			else if( factor == 32 )
				return color2;
			return color1.mulDiv32( factor ) + color2.mulDiv32( 32 - factor );
		}
		
		//! Determines the color distance between colors
		//! If fastInaccurate is true, determines distance in rgb-space
		//! Else: Computes the very accurate CIE94 color distance algorithm
		static inline _u16 distance( _color col1 , _color col2 , bool fastInAccurate = false ){
			if( fastInAccurate )
				return _color::distanceInternalRGB( col1 , col2 );
			return _color::distanceInternalCIE94( col1.toLab() , col2.toLab() );
		}
		
		//! Extract the raw Color value (convert to _pixel)
		_pixel getColor() const { return this->color; }
		operator _pixel() const { return this->color; }
		
		//! Set a RGB-Color value
		_color& setColor( _pixel col ){
			this->color = col;
			return *this;
		}
		
		//! Set RGB-Values of the currently edited Color
		_color& setRGB( _u8 red , _u8 green , _u8 blue , _optValue<bool> alpha = ignore ){
			this->setRGBInternal( red , green , blue , alpha.isValid() ? (bool)alpha : this->getAlpha() );
			return *this;
		}
		
		//! Set the HSL-Values of the currently edited Color
		_color& setHSL( _u16 hue , _u16 sat , _u16 lum , _optValue<bool> alpha = ignore ){
			this->setHSLInternal( hue , sat , lum , alpha.isValid() ? (bool)alpha : this->getAlpha() );
			return *this;
		}
		
		//! Receive the Red channel of the color
		_u8 getR() const { return this->color & 0x1F; }
		
		//! Receive the Green channel of the color
		_u8 getG() const { return ( this->color >> 5 ) & 0x1F; }
		
		//! Receive the Blue channel of the color
		_u8 getB() const { return ( this->color >> 10 ) & 0x1F; }
		
		//! Get Alpha Channel of the color
		bool getAlpha() const { return this->color >> 15; }
		
		//! Receive the Saturation of the color
		_u16 getS() const ;
		
		//! Receive the Luminance of the color
		_u16 getL() const ;
		
		//! Receive the Hue of the color
		_u16 getH();
		
		//! Set the Hue of the Color
		_color& setH( _u16 hue );
		
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
			this->color = ( this->color & ~0x1F ) | red;
			return *this;
		}
		
		//! Set the green part of the Color
		_color& setG( _u8 green ){
			this->color = ( this->color & ~( 0x1F << 5 ) ) | ( green << 5 );
			return *this;
		}
		
		//! Set the blue part of the Color
		_color& setB( _u8 blue ){
			this->color = ( this->color & ~( 0x1F << 10 ) ) | ( blue << 10 );
			return *this;
		}
		
		//! Sets the alpha channel to either true (opaque) or false (transparent)
		_color& setAlpha( bool alpha ){
			this->color = ( this->color & ~( 1 << 15 ) ) | ( alpha << 15 );
			return *this;
		}
		
		//! Set the blue part of the Color
		_color& adjustHSL( int hue , int sat , int lum , _optValue<bool> alpha = ignore );
		
		//! Set the blue part of the Color
		_color& adjustRGB( _s8 red , _s8 green , _s8 blu , _optValue<bool> alpha = ignore );
		
		//! Inverts the color
		_color& invert(){
			this->color ^= 0x7FFF;
			return *this;
		}
};

//! Allows inversion by simply doing '!color'
static unused _color operator!(_color color){
	return color.invert();
}

#endif