#include "_file/direntry.png.h"

_png::_png( string fn ) :
	_bitmap( nullptr , 0 , 0 )
	, _direntry( fn )
{
	int result = decoder.Decode( this->filename.c_str() );
	
	if( decoder.wid > 0 && decoder.hei > 0 && decoder.rgba != NULL )
	{
		this->width = decoder.wid;
		this->height = decoder.hei;
		
		// Get size of Data (in bytes)(every pixel consists of u8 red, u8 green, u8 blue, u8 alpha)
		_u32 size = decoder.wid * decoder.hei * 4;
		
		this->bmp = new _pixel[ size >> 2 ];
		
		for( _u32 i = 0, s = 0 ; i < size ; ++s, i+=4 )
			this->bmp[s] = RGBA( decoder.rgba[i] >> 3 , decoder.rgba[i+1] >> 3 , decoder.rgba[i+2] >> 3 , decoder.rgba[i+3] >> 7 );
	}
}

_png::~_png()
{
	if( this->bmp )
		delete this->bmp;
}
