#ifndef _WIN_G_SELECT_
#define _WIN_G_SELECT_

#include <_type/type.h>
#include <_type/type.gadget.h>
#include <_gadget/gadget.contextmenu.h>
#include <_gadget/gadget.scrollbar.button.h>

class _select : public _gadget
{
	private:
	
		_contextMenu*	contextMenu;
		_scrollButton*	arrow;
		
		static _callbackReturn	refreshHandler( _event );
		static _callbackReturn	keyHandler( _event );
		static _callbackReturn	resizeHandler( _event );
		_callbackReturn			editHandler( _event );
		_callbackReturn			toggleContextMenuHandler( _event );
		_callbackReturn			refreshButtonHandler( _event );
		
		// Toggles visibility of the contextMenu
		void	toggleContextMenu();
		
		// Updates both position and appearence
		void	updateArrowButton();
		
		// Returns a menu object with only the main list set
		_menu	getMenuFromList( const _menuEntryList& list ){
			_menu menu;
			menu.setList( list );
			return move(menu);
		}
		
	public:
	
		//! To receive/set the value
		string	getStrValue(){ return this->contextMenu->getStrValue(); }
		_int	getIntValue(){ return this->contextMenu->getIntValue(); }
		
		//! Set index of selected value
		void	setIntValue( _int value ){ this->contextMenu->setIntValue( value ); }
		
		//! Get the list of values
		const _menuEntryList&	getList() const { return this->contextMenu->getMenu().getList(); }
		_menuEntryList&			getList(){ return this->contextMenu->getMenu().getList(); }
		
		//! Set the list
		void	setList( _menuEntryList lst ){
			this->contextMenu->getMenu().setList( move(lst) );
			this->update();
		}
		
		//! Ctor
		_select( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , const _menuEntryList& lst = _menuEntryList( { { 0 , "" } } /* Empty List */ ) , _int initialValue = -1 , _style&& style = _style() );
		
		//! Dtor
		virtual ~_select(){ delete this->contextMenu; }
};
#endif