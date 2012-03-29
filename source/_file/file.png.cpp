#include "_file/file.png.h"

_png::_png( string fn ) :
	_file( fn )
	, _bitmap( nullptr , 0 , 0 )
{ }

void _png::decode()
{
	decoder.Decode( this->filename.c_str() );
	if( this->bmp != nullptr )
		delete[] this->bmp;
	this->height = 0;
	this->width = 0;
	if( decoder.wid > 0 && decoder.hei > 0 && decoder.rgba != NULL )
	{
		// Get size of Data (in bytes)(every pixel consists of u8 red, u8 green, u8 blue, u8 alpha)
		_u32 size = decoder.wid * decoder.hei * 4;
		
		this->bmp = new _pixel[ size >> 2 ];
		
		for( _u32 i = 0, s = 0 ; i < size ; ++s ){
			this->bmp[s] = RGBA( decoder.rgba[i] >> 3 , decoder.rgba[i+1] >> 3 , decoder.rgba[i+2] >> 3 , decoder.rgba[i+3] >> 7 );
			i+=4;
		}
		this->width = decoder.wid;
		this->height = decoder.hei;
	}
}
	
_pixelArray _png::getBitmap(){
	if( this->bmp == nullptr )
		this->decode();
	if( this->bmp != nullptr )
		return this->bmp;
	return nullptr;
}

_pixelArray _png::getBitmap( _length y ){
	if( this->bmp == nullptr )
		this->decode();
	if( this->bmp != nullptr )
		return &this->bmp[ y * this->width ];
	return nullptr;
}

_pixelArray _png::getBitmap( _length x , _length y ){
	if( this->bmp == nullptr )
		this->decode();
	if( this->bmp != nullptr )
		return &this->bmp[ y * this->width + x ];
	return nullptr;
}