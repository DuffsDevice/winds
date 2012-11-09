#ifndef _WIN_G_SELECT_
#define _WIN_G_SELECT_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_gadget/gadget.select.item.h"
#include "_type/type.system.h"

class _select : public _gadget, public _interface_input {
	
	private:
		
		friend class _selectItem;
		
		_contextMenuEntryList 	entries;
		_s32					selected;
		
		// Event-Handler
		static _gadgetEventReturnType	refreshHandler( _gadgetEvent event );
		static _gadgetEventReturnType	dragHandler( _gadgetEvent event );
		
		void refreshChildren();
		
		void setSelected( _s32 val );
		
	public:
	
		// To receive/set the value
		string	getStrValue();
		_s32	getIntValue();
		
		void	setIntValue( int value );
		
		void	refreshList();
		
		// To manipulate the list
		_contextMenuEntryList&	getList();
		void	setList( _contextMenuEntryList lst );
		void	setList( _contextMenuEntryList& lst );
		
		_select( _length w , _u8 h , _coord x , _coord y , _contextMenuEntryList lst = _contextMenuEntryList( { { -1 , "" } } /* Empty List */ ) , _gadgetStyle style = _defaultStyle_ );
};
#endif