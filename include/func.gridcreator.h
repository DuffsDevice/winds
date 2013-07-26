#ifndef _WIN_FUNC_GRID_CREATOR_
#define _WIN_FUNC_GRID_CREATOR_

#include "_type/type.h"
#include <nds/arm9/math.h>

static unused _vector<int> computeGrid( int wholeLength , int numFields )
{
	// Scale to 2048
	int wholeLengthScaled = wholeLength << 13;
	
	// Temp...
	int curScaledLength = 0;
	int oldLength = 0;
	vector<int> out = vector<int>();
	
	// Compute scaled quotient
	int trig = div32( wholeLengthScaled , numFields );
	
	while( --numFields > 0 )
	{
		curScaledLength += trig;
		out.push_back( ( curScaledLength >> 13 ) - oldLength );
		oldLength = curScaledLength >> 13;
	}
	
	out.push_back( wholeLength - oldLength );
	
	return out;
}

#endif