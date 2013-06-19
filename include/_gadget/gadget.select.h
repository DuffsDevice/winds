#ifndef _WIN_G_SELECT_
#define _WIN_G_SELECT_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_gadget/gadget.select.item.h"
#include "_gadget/gadget.scrollArea.h"
#include "_gadget/gadget.contextMenu.h"

class _select : public _scrollArea {
	
	private:
		
		friend class _selectItem;
		
		_contextMenuEntryList 	entries;
		_s32					selected;
		
		void refreshChildren();
		
		void setSelected( _s32 val );
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn keyHandler( _event event );
		
	public:
	
		// To receive/set the value
		string	getStrValue(){ return this->entries[this->selected]; }
		_s32	getIntValue(){ return this->selected; }
		
		void	setIntValue( int value ){ this->setSelected( value ); }
		
		void	refreshList();
		
		// To manipulate the list
		_contextMenuEntryList&	getList(){ return this->entries; }
		void	setList( const _contextMenuEntryList& lst ){
			this->entries = lst;
			this->refreshList();
		}
		
		_select( _length w , _u8 h , _coord x , _coord y , _contextMenuEntryList lst = _contextMenuEntryList( { { 0 , "" } } /* Empty List */ ) , _style&& style = _style() );
		
		~_select();
};
#endif