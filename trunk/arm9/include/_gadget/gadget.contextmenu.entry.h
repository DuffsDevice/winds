#ifndef _WIN_G_CONTEXTMENUENTRY_
#define _WIN_G_CONTEXTMENUENTRY_

#include <_type/type.gadget.h>
#include <_type/type.gadget.helpers.h>

class _contextMenuEntry : public _gadget
{
	private:
		
		// Text to display
		_menuEntry	entry;
		
		// Index in the context menu
		_u16		index;
		
		// Event Handlers
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn updateHandler( _event );
		static _callbackReturn mouseHandler( _event );
	
	public:
	
		//! Get text
		string getStrValue() const { return this->entry.text; }
		
		//! Get id
		_u16 getIntValue() const { return this->index; }
		
		//! Check if the entry has a sub menu
		bool hasSubMenu(){ return this->entry.linkedList; }
		_u16 getSubMenu(){ return this->entry.linkedList; }
		
		//! Ctor
		_contextMenuEntry( _optValue<_length> width , _u16 index , _menuEntry value , _style&& style = _style() );
};

#endif