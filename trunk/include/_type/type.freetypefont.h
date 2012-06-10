#ifndef _WIN_T_FREETYPEFONT_
#define _WIN_T_FREETYPEFONT_

#include "_type/type.font.h"
#include "type.bitmap.h"

#include "_freetype/ft2build.h"
#include FT_FREETYPE_H
#include FT_CACHE_H 
#include FT_BITMAP_H

#include "_freetype/freetypefacemanager.h"
#include "_freetype/freetypecache.h"

class _freetypefont : public _font
{
	private:
		
		const FaceID*			faceID;
		_u32 					charMapIndex;
		FTC_ScalerRec 			scaler;
		FTC_SBit 				sbit;
		
		void getSbit( const _char codepoint ) const;
		
	public:
		
		_freetypefont( string path , const u8 width , const u8 height );
		
		_u16 drawCharacter( _bitmap* dest , _coord x , _coord y , _char character , _pixel color , _rect clip ) const ;
		
		inline _u16 getCharacterWidth( const _char codepoint ) const ;
		
		inline _u16 isMonospace() const ;
		
		_u16 getHeight() const ;
		
		_u16 getMainHeight() const ;
		
		bool valid() const ;
		
		inline bool isCharSupported(const _char codepoint) const ;
	
};

#endif