#include "_type/type.imagefile.h"
#include "_type/type.system.h"

#include "_library/library.image.png.decoder.h"
#include "_library/library.image.png.encoder.h"
#include "_library/library.image.jpeg.decoder.h"
#include "_library/library.image.jpeg.encoder.h"
#include "_library/library.image.bmp.decoder.h"
#include "_library/library.image.bmp.encoder.h"
#include "_library/library.image.ico.h"
#include "_library/library.image.gif.decoder.h"
#include "_library/library.image.gif.encoder.h"

_bitmap _imageFile::readBitmap( _optValue<_u32> page )
{
	// Check if not-existing
	if( !this->isExisting() )
		return _bitmap();
	
	if( this->getMimeType() == _mime::image_png )
	{
		if( this->bufferedImage )
			return *bufferedImage;
		
		_bitmap result;
		
		YsRawPngDecoder* pngDecoder = new YsRawPngDecoder;
		
		pngDecoder->Decode( this->filename.c_str() );
		
		if( pngDecoder->wid > 0 && pngDecoder->hei > 0 && pngDecoder->rgba != NULL )
		{
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
		
		// Delete the decoder
		delete pngDecoder;
		
		if( this->buffer ){
			this->bufferedImage = new _bitmap( move(result) );
			return *this->bufferedImage;
		}
		return move(result);
	}
	else if( this->getMimeType() == _mime::image_jpeg )
	{
		if( this->bufferedImage )
			return *bufferedImage;
		
		_bitmap result;
		
		_u32 size = this->getSize();
		_byte* data = new _byte[size];
		this->read( data , size );
		
		Jpeg::Decoder* jpgDecoder = new Jpeg::Decoder( data , size );
		
		if ( jpgDecoder->GetResult() == Jpeg::Decoder::OK )
		{
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
		
		if( this->buffer ){
			this->bufferedImage = new _bitmap( move(result) );
			return *this->bufferedImage;
		}
		return move(result);
	}
	else if( this->getMimeType() == _mime::image_bmp )
	{
		if( this->bufferedImage )
			return *bufferedImage;
		
		_bitmap result;
		
		_u32 size = this->getSize();
		_byte* data = new _byte[size];
		this->read( data , size );
		
		_u32 width;
		_u32 height;
		
		_pixelArray pixeldata = GenericBMPDecoder::decode( data , size , width , height );
		
		if ( pixeldata != nullptr ){
			result = _bitmap( width , height );
			memCpy( result.getBitmap() , pixeldata , width * height );
			delete[] pixeldata;
		}
		
		delete[] data;
		
		if( this->buffer ){
			this->bufferedImage = new _bitmap( move(result) );
			return *this->bufferedImage;
		}
		return move(result);
	}
	else if( this->getMimeType() == _mime::image_ico )
	{
		_byte* data;
		if( this->bufferedData )
			data = this->bufferedData;
		else
		{
			_u32 size = this->getSize();
			data = new _byte[size];
			this->read( data , size );
		}
		
		_u32 width;
		_u32 height;
		
		_bitmap result;
		
		_u8* pixeldata = GenericIcoDecoder::decode( data , width , height , page );
		
		if ( pixeldata != nullptr )
		{
			// Allocate bitmap
			result = _bitmap( width , height );
			
			// Get size of Data (in bytes)(every pixel consists of u8 red, u8 green, u8 blue, u8 alpha)
			_u32		size = width * height;
			_pixelArray	base = result.getBitmap();
			_u8*		source = pixeldata;
			
			do{
				_u8 r = *source++;
				_u8 g = *source++;
				_u8 b = *source++;
				_u8 a = *source++;
				*base++ = RGB255(r,g,b,a);
			}while( --size > 0 );
			
			delete[] pixeldata;
		}
		
		if( !this->buffer )
			delete[] data;
		else
			this->bufferedData = data;
		return result;
	}
	else if( this->getMimeType() == _mime::image_gif )
	{
		gif_animation* gifAnim;
		gif_result statusCode = GIF_OK;
		
		// Allocate Decoder
		if( this->bufferedGif )
			gifAnim = this->bufferedGif;
		else
		{
			// Read file content to buffer
			_u32 size = this->getSize();
			_byte* data = new _byte[size];
			this->read( data , size );
			
			// Allocate Decoder
			gifAnim = new gif_animation;
			
			// Initialize Decoder
			gif_create( gifAnim );
			
			// Partly Decode the gif
			do{
				statusCode = gif_initialise( gifAnim , size , data );
				if (statusCode != GIF_OK && statusCode != GIF_WORKING) {
					_imageFile::outputGifWarning( "gif_initialise" , statusCode);
					break;
				}
			}while( statusCode != GIF_OK );
		}
		
		if( !this->bufferedGifBitmap )
			this->bufferedGifBitmap = new _bitmap( gifAnim->width , gifAnim->height , NO_COLOR );
		
		// Limit Page Number
		if( gifAnim->frame_count <= page )
			page = gifAnim->frame_count -1;
		
		
		// Decode...
		statusCode = gif_decode_frame(gifAnim, page);
		
		// ... and Check if everything went ok
		if (statusCode != GIF_OK)
			_imageFile::outputGifWarning( "gif_decode_frame" , statusCode );
		
		// Get destination bitmap
		_pixelArray dest = this->bufferedGifBitmap->getBitmap();
		
		// Get Source bitmap
		_u8* source = (_u8*)gifAnim->frame_image;
		
		// Copy source bitmap to destination
		_length numPixels = gifAnim->height * gifAnim->width;
		do{
			*dest++ = RGB255( source[0] , source[1] , source[2] , source[3] );
			source += 4;
		}while( --numPixels > 0 );
		
		// Clean temps
		if( this->buffer )
			this->bufferedGif = gifAnim;
		else
		{
			delete[] gifAnim->gif_data;
			delete gifAnim;
		}
		
		return *this->bufferedGifBitmap;
	}
	
	return _bitmap();
}

_u32 _imageFile::getNumPages()
{
	// Check if not-existing
	if( !this->isExisting() )
		return 0;
	
	if( this->getMimeType() == _mime::image_ico )
	{
		_byte* data;
		if( this->bufferedData )
			data = this->bufferedData;
		else{
			_u32 size = this->getSize();
			data = new _byte[size];
			this->read( data , size );
		}
		_u32 pages = GenericIcoDecoder::getNumPages( (const unsigned char*) this->bufferedData );
		
		if( this->buffer )
			this->bufferedData = data;
		else
			delete[] data;
		
		return pages;
	}
	else if( this->getMimeType() == _mime::image_gif )
	{
		gif_animation* gifAnim;
		gif_result statusCode = GIF_OK;
		
		// Allocate Decoder
		if( this->bufferedGif )
			gifAnim = this->bufferedGif;
		else
		{
			// Read file content to buffer
			_u32 size = this->getSize();
			_byte* data = new _byte[size];
			this->read( data , size );
			
			// Allocate Decoder
			gifAnim = new gif_animation;
			
			// Initialize Decoder
			gif_create( gifAnim );
			
			// Partly Decode the gif
			do{
				statusCode = gif_initialise( gifAnim , size , data );
				if (statusCode != GIF_OK && statusCode != GIF_WORKING) {
					_imageFile::outputGifWarning( "gif_initialise" , statusCode);
					break;
				}
			}while( statusCode != GIF_OK );
		}
		
		_u32 pageCount = gifAnim->frame_count;
		
		// Clean temps
		if( this->buffer )
			this->bufferedGif = gifAnim;
		else
		{
			delete[] gifAnim->gif_data;
			delete gifAnim;
		}
		
		return pageCount;
	}
	
	return 1;
}

void _imageFile::outputGifWarning( _literal context , int statusCode )
{
	_literal statusCodeStr = nullptr;
	
	switch(statusCode)
	{
		case GIF_INSUFFICIENT_FRAME_DATA:
			statusCodeStr = "insufficient Frame Data";
			break;
		case GIF_FRAME_DATA_ERROR:
			statusCodeStr = "frame Data Error";
			break;
		case GIF_INSUFFICIENT_DATA:
			statusCodeStr = "insufficient Data";
			break;
		case GIF_DATA_ERROR:
			statusCodeStr = "data Error";
			break;
		case GIF_INSUFFICIENT_MEMORY:
			statusCodeStr = "insufficient Memory";
			break;
		default:
			_system::debug( "Error in GIF-Decoder (%s): Unknow Status-Code %i" , context , statusCode );
			return;
	}
	_system::debug( "Error in GIF-Decoder (in %s): %s" , context , statusCodeStr );
}

_u32 _imageFile::getPageDelay( _u32 page )
{
	// Check if not-existing
	if( !this->isExisting() )
		return 0;
	
	if( this->getMimeType() == _mime::image_gif )
	{
		gif_animation* gifAnim;
		gif_result statusCode = GIF_OK;
		
		// Allocate Decoder
		if( this->bufferedGif )
			gifAnim = this->bufferedGif;
		else
		{
			// Read file content to buffer
			_u32 size = this->getSize();
			_byte* data = new _byte[size];
			this->read( data , size );
			
			// Allocate Decoder
			gifAnim = new gif_animation;
			
			// Initialize Decoder
			gif_create( gifAnim );
			
			// Partly Decode the gif
			do{
				statusCode = gif_initialise( gifAnim , size , data );
				if (statusCode != GIF_OK && statusCode != GIF_WORKING) {
					_imageFile::outputGifWarning( "gif_initialise" , statusCode);
					break;
				}
			}while( statusCode != GIF_OK );
		}
		
		// Limit Page Number
		if( gifAnim->frame_count <= page )
			page = gifAnim->frame_count - 1;
		
		_u32 pageDelay = gifAnim->frames[page].frame_delay;
		
		// Clean temps
		if( this->buffer )
			this->bufferedGif = gifAnim;
		else
		{
			delete[] gifAnim->gif_data;
			delete gifAnim;
		}
		
		return pageDelay;
	}
	return 0;
}

_imageFile::~_imageFile()
{
	if( !this->buffer )
		return;
	
	if( this->getMimeType() == _mime::image_ico )
		delete[] this->bufferedData;
	if( this->getMimeType() == _mime::image_gif )
	{
		// Delete buffer of file content
		delete[] bufferedGif->gif_data;
		
		// Finalize gif
		gif_finalise( this->bufferedGif );
		
		// Delet gif decoder
		delete this->bufferedGif;
		
		// Delete the buffered image
		if( this->bufferedGifBitmap )
			delete this->bufferedGifBitmap;
	}
	else
		delete this->bufferedImage;
}

bool _imageFile::writeBitmap( const _bitmap& source , _optValue<_imageFileCompression> compression )
{
	if( !this->isExisting() || !source.isValid() )
		return false;
	
	bool result = false;
	
	switch( this->getMimeType() )
	{
		case _mime::image_bmp:
			GenericBMPEncoder::encode( this->getFileName().c_str() , source.getWidth() , source.getHeight() , source.getBitmap() );
			break;
		case _mime::image_png:
		{
			YsRawPngEncoder* encoder = new YsRawPngEncoder();
			if( compression.isValid() && (_imageFileCompression)compression == _imageFileCompression::none )
				encoder->SetDontCompress(1);
			_u32 numberPixels = source.getWidth()*source.getHeight();
			_u8* buffer = new _u8[numberPixels*4];
			_u8* tempBuffer = buffer;
			_pixelArray data = source.getBitmap();
			do{
				tempBuffer[0] = RGB_GETR(*data)<<3;
				tempBuffer[1] = RGB_GETG(*data)<<3;
				tempBuffer[2] = RGB_GETB(*data)<<3;
				tempBuffer[3] = RGB_GETA(*data++)<<7; // Expand 1bit Alpha to 8bits
				tempBuffer+=4;
			}while( --numberPixels > 0 );
			result = encoder->EncodeToFile( this->getFileName().c_str() , source.getWidth() , source.getHeight() , 8 /*8bit per Channel*/, 6 /*True-Color with Alpha*/ , buffer );
			delete[] buffer;
			delete encoder;
			break;
		}
		case _mime::image_jpeg:
		{
			_u32 numberPixels = source.getWidth()*source.getHeight();
			_u8* buffer = new _u8[numberPixels*3];
			_u8* tempBuffer = buffer;
			_pixelArray data = source.getBitmap();
			do{
				tempBuffer[0] = RGB_GETR(*data)<<3;
				tempBuffer[1] = RGB_GETG(*data)<<3;
				tempBuffer[2] = RGB_GETB(*data++)<<3;
				tempBuffer+=3;
			}while( --numberPixels > 0 );
			jpge::params compressionParam;
			compressionParam.m_quality = compression.isValid() ? (int)(_imageFileCompression)compression : 75; // Set Quality
			result = jpge::compress_image_to_jpeg_file( this->getFileName().c_str() , source.getWidth() , source.getHeight() , 3 , buffer , compressionParam );
		}
		default:
			break;
	}
	
	return result;
}