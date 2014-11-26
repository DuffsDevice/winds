#include <_type/type.direntry.rom.h>
#include <_type/type.bitmap.transform.h>

struct _ndsExecuteableHeader
{
	_u32	id;
	_char	gameId[4];
	_byte	padding[4];
	_char	hash[4];
	_char	zero;
	
	//! Ctor
	_ndsExecuteableHeader() : zero(0) {}
	
}PACKED;

#define headerCharValid( ch ) ( !( ch <= '/' && ch != '#' ) && !( ':' <= ch && ch <= '@' ) && !( '[' <= ch && ch <= '_' ) && !( '{' <= ch ) )

_ndsExecuteable::_ndsExecuteable( string filename ) :
	_direntry( filename )
	, homeBrew( false )
{
	_ndsExecuteableHeader header;
	bool success = read( &header , 16 );
	
	if( !success )
		return;
	
	if( !strcmp( "####" , header.hash ) )
		this->homeBrew = true;
	else if( !strcmp( "PASS" , header.hash ) )
		this->homeBrew = true;
	else if( header.hash[0] == 0x3d && header.hash[1] == 0x84 && header.hash[2] == 0x82 && header.hash[3] == 0x0a )
		this->homeBrew = true;
	
	//! From Moonshell source: NDSRomIcon.cpp
	
	const _u32	binSize = 0x840;
	_byte*		binData = new _byte[binSize];
	
	if( header.id == 0x4e4f4349 ) // Header "ICON"
	{
		for( _u8 i = 0; i < 4 ; i++ ){
			if( !headerCharValid( header.gameId[i] ) ){
				delete[] binData;
				return;
			}
		}
		success = success && read( binData , binSize , 0x08 );
	}
	else // Normal NDSROM
	{
		for( _u8 i = 0; i < 4 ; i++ ){
			if( !headerCharValid( header.hash[i] ) ){
				delete[] binData;
				return;
			}
		}
		_u32 binOffset;
		success = success && read( &binOffset , 4 , 0x68 );
		success = success && read( binData , binSize , binOffset );
	}
	
	if( !success || binData[0] != 0x01 || binData[1] != 0x00 ){
		delete[] binData;
		return;
	}
	
	// Reading the image data!
	_pixelArray paletteData = _pixelArray( binData + 0x220 );
	_u32*		pixelData = (_u32*) ( binData + 0x020 );
	
	// Read the palette
	_color		palette[16];
	palette[0] = _color::black;
	
	for( _u8 i = 1 ; i < 16 ; i++ )
		palette[i] = _color( paletteData[i] ).setAlpha( true );
	
	this->image = _bitmap( 32 , 32 );
	
	// Read the pixel data
	for( _u8 y = 0 ; y < 4 ; y++ )
	{
		for( _u8  x = 0 ; x < 4 ; x++ )
		{
			_u32 destX = x * 8;
			_u32 destY = y * 8;
			
			for( _u8 y0 = 0 ; y0 < 8 ; y0++ )
			{
				_pixelArray dest = &this->image( destX , destY + y0 );
				_u32 data = *pixelData++;
				
				for( _u8 x0 = 0 ; x0 < 8 ; x0++ )
				{
					dest[x0] = palette[ data & 0x0f ];
					data >>= 4;
				}
			}
		}
	}
	
	this->fileImage = _bitmapTransform( this->image , (_length)10 , (_length)10 );
	
	// Delete Buffer
	delete[] binData;
}