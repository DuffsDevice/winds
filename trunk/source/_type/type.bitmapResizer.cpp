#include "_type/type.bitmapResizer.h"
#include "func.memory.h"

void _bitmapResizer::compute(){
	this->scaleRect( *this , *this->src , this->srcArea );
}

void _bitmapResizer::setSrcRect( _rect area ){
	this->srcArea = area;
}

_bitmapResizer::_bitmapResizer( _length width , _length height , const _bitmap& src , _rect srcArea , bool doNotCompute ) :
	_bitmap( width , height )
	, src( &src )
	, srcArea( srcArea )
{
	if( !doNotCompute )
		this->compute();
}

// Fast Bitmap Resizing using the
// Coarse scaling by Bresenham algorithm
void _bitmapResizer::scaleRect( _bitmap& dest , const _bitmap& src, _rect srcArea )
{
	_length realDestWidth = dest.getWidth();
	_length realSrcWidth = src.getWidth();
	if( !srcArea.isValid() )
		srcArea = _rect( 0 , 0 , realSrcWidth , src.getHeight() );
	_length srcWidth = srcArea.width;
	_length srcHeight = srcArea.height;
	_length destWidth = dest.getWidth();
	_length destHeight = dest.getHeight();
	
	_pixelArray prevSrcData = nullptr;
	_pixelArray srcData = src.getBitmap();
	_pixelArray destData = dest.getBitmap();
	
	for( _length y = destHeight , intPart = ( srcHeight / destHeight ) * realSrcWidth , fractPart = srcHeight % destHeight , trigY = 0 ; y ; y-- )
	{
		if ( srcData == prevSrcData )
			memCpy( destData , destData - realDestWidth , destWidth * sizeof( *destData ) );
		else
		{
			// Copy The Line
			_pixelArray tempSrc = srcData;
			_pixelArray tempDest = destData;
			for( _length x = destWidth , intPartX = (srcWidth / destWidth) , fractPartX = (srcWidth % destWidth) , trigX = 0 ; x ; x-- )
			{
				// Copy Pixel
				*tempDest++ = *tempSrc;
				
				// Increase
				tempSrc += intPartX;
				trigX += fractPartX;
				
				// Check overflow
				if ( trigX >= destWidth )
				{
					trigX -= destWidth;
					tempSrc++;
				} /* if */
				
			} /* for */
			
			prevSrcData = srcData;
		} /* if */
		
		// Increase
		destData += realDestWidth;
		srcData += intPart;
		trigY += fractPart;
		
		if ( trigY >= destHeight)
		{
			trigY -= destHeight;
			srcData += realSrcWidth;
		} /* if */
	} /* for */
}