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
		
		//! Get the bitmap
		_constbitmap& getImage() const { return this->getBitmap(); }
		
		//! Ctor
		_imagegadget( _coord x , _coord y , _bitmap&& bmp , _style&& style = _style() | _styleAttr::notClickable );
		_imagegadget( _coord x , _coord y , _constbitmap& bmp , _style&& style = _style() | _styleAttr::notClickable ) :
			_imagegadget( x , y , _bitmap( bmp ) , (_style&&) style )
		{}
};

#endif