#ifndef _WIN_F_IMAGEFILE_
#define _WIN_F_IMAGEFILE_

#include <_type/type.h>
#include <_type/type.bitmap.h>
#include <_type/type.direntry.h>

enum class _imageFileCompression : _u8{
	low = 30,
	medium = 55,
	high = 70,
	veryHigh = 80,
	none = 100
};

extern _fromStr<_imageFileCompression> string2imageFileCompression;

// Predefines
typedef const _direntry _constDirentry;
class gif_animation;

class _imageFile : public _constDirentry
{
	private:
		
		bool buffer;
		union{
			_bitmap*		bufferedImage;
			_byte*			bufferedData;
			gif_animation*	bufferedGif;
		};
		_bitmap*			bufferedGifBitmap;
		
		//! Helper function for errors in the gif decoder
		static void outputGifWarning( _literal context , int code );
		
	public:
		
		//! Ctor
		_imageFile( string filename , bool buffer = false ) :
			_direntry( filename )
			, buffer( buffer )
			, bufferedImage( nullptr )
			, bufferedGifBitmap( nullptr )
		{}
		
		//! Get the underlying bitmap Format
		_bitmap readBitmap( _optValue<_u32> page = ignore );
		
		//! Check if the image file supports pages
		bool supportsPages(){
			_mimeType type = this->getMimeType();
			return type == _mime::image_ico || type == _mime::image_gif;
		}
		
		//! Provided, supportsPages() returns true, this method return the number of pages in this file
		_u32 getNumPages();
		
		//! If the image is a .gif animation this returns the delay of the supplied frame
		_u32 getPageDelay( _u32 page );
		
		//! Write to the image file
		bool writeBitmap( const _bitmap& source , _optValue<_mimeType> mimeType2Write = ignore , _optValue<_imageFileCompression> compression = _imageFileCompression::high );
		
		//! Extension independent determination of the underlying image type
		_mimeType getRealMime();
		
		//! Dtor
		~_imageFile();
};
#endif
