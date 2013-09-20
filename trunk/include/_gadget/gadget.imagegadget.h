// Check if already included
#ifndef _WIN_G_IMAGE_
#define _WIN_G_IMAGE_

#include "_type/type.gadget.h"
#include "_type/type.bitmap.h"

class _imagegadget : public _gadget {
	
	private:
		
		static _callbackReturn refreshHandler( _event event );
		
	public:
		
		//! set the image the easy way
		void setImage( const _bitmap& img ){
			this->setBitmap( img );
		}
		
		//! Get the bitmap
		const _bitmap& getImage() const { return this->getBitmap(); }
		
		//! Ctor
		_imagegadget( _coord x , _coord y , _bitmap&& bmp , _style&& style = _style() | _styleAttr::notClickable );
		_imagegadget( _coord x , _coord y , const _bitmap& bmp , _style&& style = _style() | _styleAttr::notClickable ) :
			_imagegadget( x , y , _bitmap( bmp ) , (_style&&) style )
		{}
};

#endif