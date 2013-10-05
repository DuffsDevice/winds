#ifndef _WIN_G_CONTEXTMENU_
#define _WIN_G_CONTEXTMENU_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_gadget/gadget.popup.h"
#include "_gadget/gadget.contextmenu.entry.h"

typedef _map<_s32,string> _contextMenuList;

class _contextMenu : public _popup
{
	private:
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn keyHandler( _event );
		static _callbackReturn openHandler( _event );
		static _callbackReturn closeHandler( _event );
		
		// Temporary variable to be read after the contextMenu was closed
		_contextMenuEntry*	selectedEntry;
		_contextMenuEntry*	activeEntry;
		bool				preserveValue;
		
		// For _contextMenuEntry's to call when they are clicked
		void selectEntry( _contextMenuEntry* gadget );
		void highlightEntry( _contextMenuEntry* gadget );
		
		friend class _contextMenuEntry;
		
		// Generates all the _contextMenuEntries and removes the old ones before
		void generateChildren( const _contextMenuList& list );
		
	public:
	
		//! Get the index of the selected Value
		_s32 getIntValue(){ return this->selectedEntry ? this->selectedEntry->getIntValue() : -1; }
		
		//! Get the text of the selected Value
		string getStrValue(){ return this->selectedEntry ? this->selectedEntry->getStrValue() : ""; }
		
		//! Set Value
		void setIntValue( _s32 id );
		
		//! Set List
		void setList( const _contextMenuList& list ){
			this->generateChildren( list );
		}
		
		//! Get List
		_contextMenuList getList();
		
		//! Ctor
		_contextMenu( const _contextMenuList& list , _gadget* owner , _optValue<_length> width = ignore , bool preserveValue = false , _s32 initialValue = -1 , _style&& style = _style() );
		
		//! Dtor
		~_contextMenu();
};

#endif