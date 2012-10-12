#include "_type/type.imagefile.h"
#include <functional>

_imagefile::_imagefile( string fn ) :
	_bitmap( nullptr , 0 , 0 )
	, _direntry( fn )
	, pngDecoder( nullptr )
{
	// Doesn't make sence to 
	if( !this->isExisting() )
		return;
	
	if( this->getMimeType() == _mime::image_png )
	{
		printf("PNG-File!\n");
		pngDecoder = new YsRawPngDecoder;
		
		pngDecoder->Decode( this->filename.c_str() );
		
		if( pngDecoder->wid > 0 && pngDecoder->hei > 0 && pngDecoder->rgba != NULL )
		{
			printf("Success: %d , %d\n",pngDecoder->wid,pngDecoder->hei);
			this->width = pngDecoder->wid;
			this->height = pngDecoder->hei;
			
			// Get size of Data (in bytes)(every pixel consists of u8 red, u8 green, u8 blue, u8 alpha)
			_u32 size = pngDecoder->wid * pngDecoder->hei * 4;
			
			this->bmp = new _pixel[ size >> 2 ];
			
			for( _u32 i = 0, s = 0 ; i < size ; ++s, i+=4 )
				this->bmp[s] = RGBA( pngDecoder->rgba[i] >> 3 , pngDecoder->rgba[i+1] >> 3 , pngDecoder->rgba[i+2] >> 3 , pngDecoder->rgba[i+3] >> 7 );
		}
		delete this->pngDecoder;
	}
	else if( this->getMimeType() == _mime::image_jpeg )
	{
		printf("JPG-File!\n");
		_u32 size = this->getSize();
		_u8* data = new _u8[size];
		this->read( data , size );
		this->jpgDecoder = new Jpeg::Decoder( data , size );
		
		if ( this->jpgDecoder->GetResult() == Jpeg::Decoder::OK )
		{
			//printf("Success: %d , %d\n",this->jpgDecoder->GetWidth(),this->jpgDecoder->GetHeight());
			this->bmp = new _pixel[ this->jpgDecoder->GetWidth() * this->jpgDecoder->GetHeight() ];
			this->width = this->jpgDecoder->GetWidth();
			this->height = this->jpgDecoder->GetHeight();
			
			if( this->jpgDecoder->IsColor() )
			{
				_u32 size = this->jpgDecoder->GetImageSize();
				_u8* rgb = this->jpgDecoder->GetImage();
				
				for( _u32 i = 0, s = 0 ; i < size ; ++s, i+=3 )
					this->bmp[s] = RGB( rgb[i] >> 3 , rgb[i+1] >> 3 , rgb[i+2] >> 3 );
			}
			else
			{
				_u32 size = this->jpgDecoder->GetImageSize();
				_u8* rgb = this->jpgDecoder->GetImage();
				
				for( _u32 s = 0 ; s < size ; ++s )
					this->bmp[s] = RGB( rgb[s] >> 3 , rgb[s] >> 3 , rgb[s] >> 3 );
			}
		}
		delete data;
		delete this->jpgDecoder;
	}
}

_imagefile::~_imagefile()
{
	if( this->bmp )
		delete this->bmp;
}
