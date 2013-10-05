#ifndef _WIN_G_POPUP_
#define _WIN_G_POPUP_

#include "_type/type.h"
#include "_type/type.gadget.h"

class _popup : public _gadget
{
	
	private:
		
		static _callbackReturn blurHandler( _event );
		
		_align		xDir; // can either be left or right
		_valign		yDir; // Can either bo top or bottom
		_gadget*	owner; // The gadget that might receive focus when the popup closes
		bool		opened; // Flag that indicates whether the popup is currently open
		
	public:
		
		//! Shows the popup at the specified position on the screen
		void show( _coord x , _coord y );
		
		//! Hides the popup
		void shelve( bool focusOwner = false );
		
		//! Toggle between hidden and shown
		void toggle( _coord x , _coord y );
		
		//! Check if the popup is opened on the screen
		bool isOpened(){ return this->opened; }
		
		//! Specifies the gadget that receives focus after closing the contextMenu
		void setOwner( _gadget* owner ){ this->owner = owner; }
		
		//! ... and a getter for that one, too
		_gadget* getOwner(){ return this->owner; }
		
		// Ctor
		_popup( _optValue<_length> width = ignore , _optValue<_length> height = ignore , _gadget* owner = nullptr , _style&& style = _style() );
		
};

#endif