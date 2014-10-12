#ifndef _WIN_G_CONTEXTMENU_
#define _WIN_G_CONTEXTMENU_

#include <_type/type.h>
#include <_type/type.gadget.h>
#include <_type/type.menu.h>
#include <_gadget/gadget.popup.h>
#include <_gadget/gadget.contextmenu.entry.h>

class _contextMenu : public _popup
{
	private:
		
		static _callbackReturn	refreshHandler( _event );
		static _callbackReturn	keyHandler( _event );
		static _callbackReturn	openHandler( _event );
		static _callbackReturn	closeHandler( _event );
		static _callbackReturn	updateHandler( _event );
		static _callbackReturn	blurHandler( _event );
		_callbackReturn			subMenuCloseHandler( _event );
		
		// Temporary variable to be read after the contextMenu was closed
		_bool						preserveValue;
		_bool						wasMenuAllocated;
		_u16						mainList;
		_menu*						menu;
		_s16						currentValue;
		_uniquePtr<_contextMenu>	curOpenedSubMenu;
		
		friend class _contextMenuEntry;
		
		// Chooses the currently selected entry and if its valid, it closes the contextMenu
		void submit();
		
		//! Destructs the possibly allocated menu object
		void destruct(){
			if( this->wasMenuAllocated ){
				delete this->menu;
				this->wasMenuAllocated = false;
				this->curOpenedSubMenu = nullptr;
			}
		}
		
		//! Get _contextMenuEntry that has the supplied intValue
		_contextMenuEntry* getEntryFromValue( _u16 value ) const ;
		
	public:
	
		//! Get the index of the selected menu entry in the main list
		_int			getIntValue() const { return this->currentValue; }
		
		//! Get List index
		_u16			getListIndex() const { return this->mainList; }
		
		//! Set index of used list in _menu
		void			setListIndex( _u16 listIndex ){
			if( listIndex == this->mainList )
				return;
			this->mainList = listIndex;
			this->update();
		}
		
		//! Get the used list for the context menu
		const _menuEntryList&	getList() const { return this->menu->getList( this->mainList ); }
		_menuEntryList&			getList(){ return this->menu->getList( this->mainList ); }
		
		//! Apply a new list over the for this menu currently active list
		void			setList( _menuEntryList lst ){
			this->menu->setList( this->mainList , move(lst) );
		}
		
		//! Get the text of the selected Value
		string			getStrValue() const {
			_contextMenuEntry* entry = this->getEntryFromValue( this->currentValue );
			return entry ? entry->getStrValue() : "";
		}
		
		//! Select entry in list
		void			setIntValue( _u16 entryIndex );
		
		//! Set List
		void			setMenu( _paramAlloc<_menu> menu ){
			this->destruct();
			this->menu = menu;
			this->wasMenuAllocated = true;
			this->update();
		}
		void			setMenu( _menu* menu ){
			this->destruct();
			this->menu = menu;
			this->update();
		}
		
		//! Get underlying _menu object
		const _menu&	getMenu() const { return *this->menu; }
		_menu&			getMenu(){ return *this->menu; }
		
		//! Ctor
		_contextMenu( _optValue<_length> width , _paramAlloc<_menu> content , _gadget* owner = nullptr , bool preserveValue = false , _int initialValue = -1 , _u16 mainList = 0 , _style&& style = _style() ) :
			_contextMenu( width , content.get() , owner , preserveValue , initialValue , mainList , move(style) )
		{ this->wasMenuAllocated = true; }
		_contextMenu( _optValue<_length> width , _menu* content , _gadget* owner = nullptr , bool preserveValue = false , _int initialValue = -1 , _u16 mainList = 0 , _style&& style = _style() );
		
		//! Dtor
		virtual ~_contextMenu();
};

#endif