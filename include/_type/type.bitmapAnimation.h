// Check if already included
#ifndef _WIN_T_BITMAPANIMATION_
#define _WIN_T_BITMAPANIMATION_

#include "_type/type.bitmap.h"
#include "_type/type.animation.h"

class _bitmapTransform : public _bitmap{
	
	private:
		
		int		(*transformFuncX)( float x , float offset , float range , float xGesamt );
		int		(*transformFuncY)( float y , float offset , float range , float yGesamt );
		
		_bitmap*	source;
		
	public:
		
		_bitmapTransform( _length width , _length height , _bitmap* source) :
			_bitmap( width , height )
			, source( source )
		{
			this->transformFuncX = _animation::_linear::ease;
			this->transformFuncY = _animation::_linear::ease;
		}
		
		void compute(){
			
			for( _length x = 0 ; x < this->getWidth() ; x++ ){
				for( _length y = 0 ; y < this->getHeight() ; y++ ){ 
					_coord srcX = this->transformFuncX( x , 0 , this->source->getWidth() , this->getWidth() );
					_coord srcY = this->transformFuncY( y , 0 , this->source->getHeight() , this->getHeight() );
					_bitmap::drawPixel( x , y , this->source->getPixel( srcX , srcY ) );
				}
			}
		}
		
};

#endif