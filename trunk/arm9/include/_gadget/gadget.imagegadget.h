// Check if already included
#ifndef _WIN_G_IMAGE_
#define _WIN_G_IMAGE_

#include "_type/type.gadget.h"
#include "_type/type.bitmap.h"

class _imagegadget : public _gadget {
	
	private:
		
		static _callbackReturn refreshHandler( _event );
		
	public:
		
		//! set the image the easy way
		void setImage( _constbitmap& img ){
			this->setBitmap( img );
		}
		
		void setImage( _bitmap&& img ){
			this->setBitmap( move(img) );
		}
		
		//! Get the bitmap
		_constbitmap& getImage() const { return this->getBitmap(); }
		
		//! Ctor
		_imagegadget( _optValue<_coord> x , _optValue<_coord> y , _bitmap&& bmp , _style&& style = _style() | _styleAttr::notClickable );
		_imagegadget( _optValue<_coord> x , _optValue<_coord> y , _constbitmap& bmp , _style&& style = _style() | _styleAttr::notClickable ) :
			_imagegadget( x , y , _bitmap( bmp ) , move(style) )
		{}
};

#endif