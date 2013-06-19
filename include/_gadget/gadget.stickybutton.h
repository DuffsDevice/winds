// Check if already included
#ifndef _WIN_G_STICKYBUTTON_
#define _WIN_G_STICKYBUTTON_

#include "_type/type.gadget.h"
#include "_type/type.singlevaluegroup.h"
#include "_gadget/gadget.button.h"

class _stickybutton : public _button
{
	private:
		
		_singleValueGroup<_stickybutton>* group;
		
		friend class _singleValueGroup<_stickybutton>;
		
		static _callbackReturn mouseHandler( _event );
		
	public:
		
		//! Set, whether the button is stuck down
		void setStuckDown( bool stuck ){ if( this->pressed == stuck ) return; this->pressed = stuck; this->bubbleRefresh( true ); }
		void setIntValue( bool value ){ setStuckDown( value ); }
		
		//! Check whether the button is stuck down
		_s32 isStuckDown(){ return this->pressed; }
		_s32 getIntValue(){ return isStuckDown(); }
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_stickybutton( _length width , _length height , _coord x , _coord y , string title = "" , _style&& style = _style() );
		
		//! Constructor with coordinates, title and optional: Style
		//! The Width will be computed by the font
		_stickybutton( _coord x , _coord y , string text = "" , _style&& style = _style() );
};

#endif