#include "_type/type.imagefile.h"

#include "_library/library.image.png.h"
#include "_library/library.image.jpeg.h"
#include "_library/library.image.bmp.h"

_bitmap _imageFile::readBitmap()
{
	// Check if not-existing
	if( !this->isExisting() )
		return _bitmap();
	
	_bitmap result;
	
	if( this->getMimeType() == _mime::image_png )
	{
		//printf("PNG-File!\n");
		YsRawPngDecoder* pngDecoder = new YsRawPngDecoder;
		
		pngDecoder->Decode( this->filename.c_str() );
		
		if( pngDecoder->wid > 0 && pngDecoder->hei > 0 && pngDecoder->rgba != NULL )
		{
			//printf("Success: %d , %d\n",pngDecoder->wid,pngDecoder->hei);
			
			// Allocate bitmap
			result = _bitmap( pngDecoder->wid , pngDecoder->hei );
			
			// Get size of Data (in bytes)(every pixel consists of u8 red, u8 green, u8 blue, u8 alpha)
			_u32		size = pngDecoder->wid * pngDecoder->hei;
			_pixelArray	base = result.getBitmap();
			_u8*		source = pngDecoder->rgba;
			
			do{
				_u8 r = *source++;
				_u8 g = *source++;
				_u8 b = *source++;
				_u8 a = *source++;
				*base++ = RGB255(r,g,b,a);
			}while( --size > 0 );
		}
	}
	else if( this->getMimeType() == _mime::image_jpeg )
	{
		//printf("JPG-File!\n");
		_u32 size = this->getSize();
		_byte* data = new _byte[size];
		this->read( data , size );
		
		Jpeg::Decoder* jpgDecoder = new Jpeg::Decoder( data , size );
		
		if ( jpgDecoder->GetResult() == Jpeg::Decoder::OK )
		{
			//printf("Success: %d , %d\n",jpgDecoder->GetWidth(),jpgDecoder->GetHeight());
			result = _bitmap( jpgDecoder->GetWidth() , jpgDecoder->GetHeight() );
			
			if( jpgDecoder->IsColor() )
			{
				_u32 size = jpgDecoder->GetImageSize();
				_u8* rgb = jpgDecoder->GetImage();
				_pixelArray dest = result.getBitmap();
				
				do{
					_u8 r = *rgb++;
					_u8 g = *rgb++;
					_u8 b = *rgb++;
					*dest++ = RGB255(r,g,b);
					size -= 3;
				}while( size > 0 );
			}
			else
			{
				_u32 size = jpgDecoder->GetImageSize();
				_u8* rgb = jpgDecoder->GetImage();
				_pixelArray dest = result.getBitmap();
				
				do{
					*dest++ = BW255( *rgb++ );
				}while( --size > 0 );
			}
		}
		delete[] data;
		delete jpgDecoder;
	}
	else if( this->getMimeType() == _mime::image_bmp )
	{
		//printf("BMP-File!\n");
		_u32 size = this->getSize();
		_u8* data = new _u8[size];
		this->read( data , size );
		
		_u32 width;
		_u32 height;
		
		_pixelArray pixeldata = GenericBMPDecoder::decode( data , width , height );
		
		if ( pixeldata != nullptr ){
			result = _bitmap( width , height );
			memCpy( result.getBitmap() , pixeldata , width * height );
			delete[] pixeldata;
		}
		
		delete[] data;
	}
	
	return move(result);
}