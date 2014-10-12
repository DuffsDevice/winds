#ifndef _WIN_T_COLOR_PALETTE_
#define _WIN_T_COLOR_PALETTE_

#include <_type/type.h>
#include <_type/type.color.h>
#include <_type/type.bitmap.h>

class _colorPalette
{
	private:
		
		_vector<_color> colors;
		bool			hasTransparent;
	
	public:
		
		//! Default Ctor
		_colorPalette() :
			hasTransparent( false )
		{}
		
		//! Ctor with list of colors
		_colorPalette( _initializerList<_color> list );
		
		//! Create Palette from _bitmap
		static _colorPalette fromBitmap( const _bitmap& bmp );
		
		//! Get the color index of the palette color that matches the supplied color best
		_u32 getClosestColor( _color color ) const ;
		
		//! Get the number of colors in the palette
		_u32 getNumColors() const { return colors.size() + this->hasTransparent; }
		
		//! Reduce the number of colors in the palette while trying to preserve palette quality
		void downsample( _u32 numberOfColors );
		
		//! Get list of colors inside the palette
		_vector<_color> getColors() const
		{
			_vector<_color> result = this->colors;
			
			// Add transparent palette entry
			if( this->hasTransparent )
				result.push_back( _color::transparent );
			
			return move(result);
		}
		
		//! Check if the palette has an entry for a transparent color
		bool hasTransparentColor() const {
			return this->hasTransparent;
		}
		
		//! Get the supplied index in the color palette, the last color eventually is the transparent color
		_color getColor( _u32 index ) const {
			if( this->hasTransparent && index == this->colors.size() )
				return _color::transparent;
			return this->colors[index];
		}
		
		//! Get the supplied index in the color palette (you can not get the transparent color index using this way!)
		_color operator[]( _u32 index ){
			return this->colors[index];
		}
		
		//! Delete all colors from the Palette
		void clear(){ this->colors.clear(); }
		
		//! Add a color to the palette
		void addColor( _color col , bool noDuplicates = true );
		
		//! Add a bunch of colors to the palette using an array of colors
		void addColors( _color* colors , _u32 numColors , bool noDuplicates = true ){
			while( numColors-- > 0 )
				this->addColor( *colors++ , noDuplicates );
		}
		void addColors( _pixel* colors , _u32 numColors , bool noDuplicates = true ){
			while( numColors-- > 0 )
				this->addColor( *colors++ , noDuplicates );
		}
};

#endif