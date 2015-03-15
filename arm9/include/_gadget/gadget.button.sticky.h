// Check if already included
#ifndef _WIN_G_STICKYBUTTON_
#define _WIN_G_STICKYBUTTON_

#include <_type/type.gadget.h>
#include <_type/type.singlevaluegroup.h>
#include <_gadget/gadget.button.h>

class _stickyButton : public _button
{
	private:
		
		_singleValueGroup<_stickyButton>* group;
		
		friend class _singleValueGroup<_stickyButton>;
		
		static _callbackReturn mouseHandler( _event );
		
	public:
		
		//! Set, whether the button is stuck down
		void setStuckDown( bool stuck ){ if( this->pressed == stuck ) return; this->pressed = stuck; this->redraw(); }
		void setIntValue( bool value ){ setStuckDown( value ); }
		
		//! Check whether the button is stuck down
		_s32 isStuckDown(){ return this->pressed; }
		_s32 getIntValue(){ return isStuckDown(); }
		
		//! Constructor with dimensions, coordinates, title and optional: Style
		_stickyButton( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , wstring title = "" , _style&& style = _style() );
};

#endif