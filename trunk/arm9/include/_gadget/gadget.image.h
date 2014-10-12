// Check if already included
#ifndef _WIN_G_IMAGE_
#define _WIN_G_IMAGE_

#include <_type/type.gadget.h>
#include <_type/type.bitmap.h>

class _imageGadget : public _gadget {
	
	private:
		
		static _callbackReturn refreshHandler( _event );
		
	public:
		
		//! set the image the easy way
		void setImage( _constBitmap& img ){
			this->setBitmap( img );
		}
		
		void setImage( _bitmap&& img ){
			this->setBitmap( move(img) );
		}
		
		//! Get the bitmap
		_constBitmap& getImage() const { return this->getBitmap(); }
		
		//! Ctor
		_imageGadget( _optValue<_coord> x , _optValue<_coord> y , _bitmap bmp , _optValue<_length> width = ignore , _optValue<_length> height = ignore , _style&& style = _style() );
};

#endif