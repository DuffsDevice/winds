#ifndef _WIN_G_POPUP_
#define _WIN_G_POPUP_

#include "_type/type.h"
#include "_type/type.gadget.h"

class _popup : public _gadget
{
	
	private:
		
		static _callbackReturn blurHandler( _event );
		
		_direction	dir; // Direction to which it opens
		_gadget*	owner; // The gadget that might receive focus when the popup closes
		bool		opened; // Flag that indicates whether the popup is currently open
		
	public:
		
		//! Shows the popup at the specified position on the screen
		void show( _coord x , _coord y );
		void show( _2s32 pos ){
			show( pos.first , pos.second );
		}
		
		//! Shows the popup centered at the specified position
		void showCentered( _coord x , _coord y );
		void showCentered( _2s32 pos ){
			show( pos.first , pos.second );
		}
		
		//! Hides the popup
		void shelve( bool focusOwner = false );
		
		//! Toggle between hidden and shown
		void toggle( _coord x , _coord y );
		void toggle( _2s32 pos ){
			toggle( pos.first , pos.second );
		}
		
		//! Check if the popup is opened on the screen
		bool isOpened(){ return this->opened; }
		
		//! Specifies the gadget that receives focus after closing the contextMenu
		void setOwner( _gadget* owner ){ this->owner = owner; }
		
		//! ... and a getter for that one, too
		_gadget* getOwner(){ return this->owner; }
		
		//! Ctor
		_popup( _optValue<_length> width = ignore , _optValue<_length> height = ignore , _gadget* owner = nullptr , _style&& style = _style() );
		
};

#endif