// Check if already included
#ifndef _WIN_G_IMAGE_
#define _WIN_G_IMAGE_

#include "_type/type.gadget.h"
#include "_type/type.bitmap.h"

class _imagegadget : public _gadget {
	
	private:
	
		_bitmap img;
		
		static _callbackReturn refreshHandler( _event event );
		
	public:
		
		noinline void setImage( const _bitmap& img )
		{
			this->img = img;
			this->setHeight( img.getHeight() );
			this->setWidth( img.getWidth() );
			this->bubbleRefresh( true );
		}
		
		const _bitmap& getImage(){ return this->img; }
		
		_bitmap& getModifyableImage(){ return this->img; }
		
		_imagegadget( _coord x , _coord y , const _bitmap& bmp , _style&& style = _style() | _styleAttr::notClickable );
};

#endif