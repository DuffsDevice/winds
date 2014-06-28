#ifndef _WIN_G_POPUP_
#define _WIN_G_POPUP_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_type/type.direction.h"

class _popup : public _gadget
{
	private:
		
		static _callbackReturn blurHandler( _event );
		
		_direction	dir; // Direction to which it opens
		_gadget*	owner; // The gadget that might receive focus when the popup closes
		bool		opened; // Flag that indicates whether the popup is currently open
		_2s8		offset; // The (X|Y)-Number of pixels , that the popup should be moved in the opposite direction it is opened to
		
	public:
		
		//! Shows the popup at the specified position on the screen
		void show( _coord x , _coord y , bool rightDownOnly = true );
		void show( _pos pos , bool rightDownOnly = true ){
			show( pos.first , pos.second , rightDownOnly );
		}
		void show( _rect object , bool rightDownOnly = true );
		
		//! Shows the popup centered at the specified position
		void showCentered( _coord x , _coord y );
		void showCentered( _pos pos ){
			show( pos.first , pos.second );
		}
		
		//! Hides the popup
		void shelve( bool focusOwner = false );
		
		//! Toggle between hidden and shown
		void toggle( _coord x , _coord y , bool rightDownOnly = true );
		void toggle( _pos pos , bool rightDownOnly = true ){
			toggle( pos.first , pos.second , rightDownOnly );
		}
		void toggle( _rect object , bool rightDownOnly = true );
		
		//! Check if the popup is opened on the screen
		bool isOpened(){ return this->opened; }
		
		//! Specifies the gadget that receives focus after closing the contextMenu
		void setOwner( _gadget* owner ){ this->owner = owner; }
		
		//! ... and a getter for that one, too
		_gadget* getOwner(){ return this->owner; }
		
		//! Get The direction the popup is opened to
		_direction getDirection() const { return dir; }
		
		//! Setter for Attribute @line 16
		void setOffset( _2s8 offset ){ this->offset = offset; }
		
		//! Getter for the Attribute defined @line 16
		_2s8 getOffset() const { return this->offset; }
		
		//! Ctor
		_popup( _optValue<_length> width = ignore , _optValue<_length> height = ignore , _gadget* owner = nullptr , _style&& style = _style() );
		
};

#endif