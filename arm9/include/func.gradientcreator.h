#ifndef _WIN_F_GRID_CREATOR_
#define _WIN_F_GRID_CREATOR_

#include "_type/type.h"
#include <nds/arm9/math.h>

static unused _uniquePtr<_pixel[]> computeGradient( _color fromColor , _color toColor , _length gradientLength , _length offset = 0 , _u32* numConsecutivePixels = nullptr )
{
	//! Gradient Computations
		
		_pixelArray gradTable = new _pixel[gradientLength];
		_pixelArray temp = gradTable;
		_pixelArray end = gradTable + gradientLength;
		
		// Difference between color values
		int dR = toColor.getR() - fromColor.getR();
		int dG = toColor.getG() - fromColor.getG();
		int dB = toColor.getB() - fromColor.getB();
		
		// Scale them to 2048
		dR <<= 11; dG <<= 11; dB <<= 11;
		
		int trigR = div32( dR , gradientLength );
		int trigG = div32( dG , gradientLength );
		int trigB = div32( dB , gradientLength );
		
		// Defines
		int curR = fromColor.getR() << 11;
		int curG = fromColor.getG() << 11;
		int curB = fromColor.getB() << 11;
		
		if( offset > 0 ){
			curR += trigR*offset; curG += trigG*offset; curB += trigB*offset;
		}
		
		// Fill the table
		for( ; temp != end ; curR += trigR, curG += trigG, curB += trigB )
			//*temp++ = _color::fromRGB( curR >> 11 , curG >> 11 , curB >> 11 );
			*temp++ = ( ( curR >> 11 ) & ( 31 << 0  ) ) // Adjust and bitwise AND width 31
					| ( ( curG >> 6  ) & ( 31 << 5  ) )
					| ( ( curB >> 1  ) & ( 31 << 10 ) )
					| ( ( 1 << 15 ) );
			// Faster because of less bitshifts
		
	// Number of pixels above each other having the same color
	if( numConsecutivePixels )
		*numConsecutivePixels = div32( 1 << 12 , abs( trigR ) + abs( trigG ) + abs( trigB ) );
	
	return move(gradTable);
}

#endif