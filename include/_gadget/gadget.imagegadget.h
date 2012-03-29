// Check if already included
#ifndef _WIN_G_IMAGE_
#define _WIN_G_IMAGE_

#include "_type/type.gadget.h"
#include "_type/type.bitmap.h"

class _imagegadget : public _gadget {
	
	private:
	
		const _bitmap* img;
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		
	public:
		
		void setImage( const _bitmap* img ){ this->img = img; this->setWidth( img->getWidth() ); this->setHeight( img->getHeight() ); this->handleEvent( refresh ); }
		
		const _bitmap* getImage(){ return this->img; }
		
		_imagegadget( _coord x , _coord y , const _bitmap* bmp , _gadgetStyle style = _defaultStyle_);
};

#endif