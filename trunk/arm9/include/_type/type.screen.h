#ifndef _WIN_T_SCREEN_
#define _WIN_T_SCREEN_

//! NDS-Headers
#include <nds/ndstypes.h>
#include <nds/arm9/input.h>

//! Types
#include "_type/type.h"
#include "_type/type.bitmap.h"

class _screen{
	
	private:
		
		//! Background-id returned by bgInit or bgInitSub
		int bgId;
		
	public:
	
		//! Scroll the background ( < 0 will be right/bottom )
		void scrollX( _coord x );
		void scrollY( _coord y );
		
		//! Scale the background ( 0.5 becomes 2:1 = larger )
		void scaleX( float x );
		void scaleY( float y );
		void scale( float rat );
		
		//! Get current Scroll
		_coord getScrollX() const ;
		_coord getScrollY() const ;
		
		//! Get current Scale
		float getScaleX() const ;
		float getScaleY() const ;
		
		//! get the base Memory
		_pixelArray	getMemoryPtr() const ;
		
		//! Get the id returned by bgInit or bgInitSub
		int getBgId() const ;
		
		//! Ctor
		_screen( int bgId );
		
		//! Dtor
		virtual ~_screen() = default;
};

#endif