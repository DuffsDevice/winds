#include "_type/type.bitmap.transform.h"
#include "_controller/controller.memory.h"

void _bitmapTransform::compute(){
	this->scaleRect( *this , *this->src , this->srcArea , this->flipHorizontal , this->flipVertical );
}

void _bitmapTransform::setSrcRect( _rect area ){
	this->srcArea = area;
}

_bitmapTransform::_bitmapTransform( _constBitmap& src , _length width , _length height , _rect srcArea , bool doNotCompute , bool flipH , bool flipV ) :
	_bitmap( width , height )
	, src( &src )
	, srcArea( srcArea )
	, flipHorizontal( flipH )
	, flipVertical( flipV )
{
	if( !doNotCompute )
		this->compute();
}

// Fast Bitmap Resizing using the
// Coarse scaling by Bresenham algorithm
void _bitmapTransform::scaleRect( _bitmap& dest , _constBitmap& src, _rect srcArea , bool flipH , bool flipV )
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
	_pixelArray destData = dest.getBitmap() + ( flipV ? destWidth * ( destHeight - 1 ) : 0 );
	
	_s8 deltaX = flipH ? -1 : 1;
	_s8 deltaY = flipV ? -1 : 1;
	
	for( _length y = destHeight , intPart = ( srcHeight / destHeight ) * realSrcWidth , fractPart = srcHeight % destHeight , trigY = 0 ; y ; y-- )
	{
		if( srcData == prevSrcData )
			memcpy16( destData , destData - realDestWidth , destWidth * sizeof( *destData ) );
		else
		{
			// Copy The Line
			_pixelArray tempSrc = srcData;
			_pixelArray tempDest = destData + ( flipH ? destWidth - 1 : 0 );
			for( _length x = destWidth , intPartX = (srcWidth / destWidth) , fractPartX = (srcWidth % destWidth) , trigX = 0 ; x ; x-- )
			{
				// Copy Pixel
				*tempDest = *tempSrc;
				tempDest += deltaX;
				
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
		destData += realDestWidth * deltaY;
		srcData += intPart;
		trigY += fractPart;
		
		if ( trigY >= destHeight)
		{
			trigY -= destHeight;
			srcData += realSrcWidth;
		} /* if */
	} /* for */
}