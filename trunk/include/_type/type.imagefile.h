#ifndef _WIN_F_IMAGEFILE_
#define _WIN_F_IMAGEFILE_

#include "_type/type.h"
#include "_type/type.bitmap.h"
#include "_type/type.direntry.h"

class _imagefile : public _bitmap , private _direntry 
{
	public:
		
		//! Ctor
		_imagefile( string fn );
		
		//! Dtor
		~_imagefile();
		
};
#endif
