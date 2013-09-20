#ifndef _WIN_G_SELECT_
#define _WIN_G_SELECT_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_gadget/gadget.contextmenu.h"
#include "_gadget/gadget.scrollbar.button.h"

class _select : public _gadget
{
	private:
	
		_contextMenu*	contextMenu;
		_scrollButton*	arrow;
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn keyHandler( _event event );
		static _callbackReturn resizeHandler( _event event );
		_callbackReturn editHandler( _event event );
		_callbackReturn toggleContextMenuHandler( _event event );
		_callbackReturn refreshButtonHandler( _event event );
		
		// Toggles visibility of the contextMenu
		void toggleContextMenu();
		
		// Updates both position and appearence
		void updateArrowButton();
		
	public:
	
		//! To receive/set the value
		string	getStrValue(){ return this->contextMenu->getStrValue(); }
		_s32	getIntValue(){ return this->contextMenu->getIntValue(); }
		
		//! Set index of selected value
		void	setIntValue( _s32 value ){ this->contextMenu->setIntValue( value ); }
		
		//! Get the list of values
		_contextMenuList getList(){ return this->contextMenu->getList(); }
		
		//! Set the list
		void setList( const _contextMenuList& lst ){
			this->contextMenu->setList( lst );
		}
		
		//! Ctor
		_select( _length width , _u8 height , _coord x , _coord y , const _contextMenuList& lst = _contextMenuList( { { 0 , "" } } /* Empty List */ ) , _s32 initialValue = -1 , _style&& style = _style() );
		
		//! Dtor
		~_select(){ delete this->contextMenu; }
};
#endif