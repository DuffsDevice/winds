#ifndef _WIN_G_SELECT_
#define _WIN_G_SELECT_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_gadget/gadget.label.h"
#include "_type/type.system.h"
#include "_gadget/gadget.contextMenu.h"

class _select;

class _selectPopDown : public _contextMenu{
	
	private:
		
		friend class _select;
		_contextMenuEntryList* 	entries;
		
		// Event-Handler
		static _gadgetEventReturnType	refreshHandler( _gadgetEvent event );
		static _gadgetEventReturnType	clickHandler( _gadgetEvent event );
		
		void refreshChildren();
		
	public:
		
		void toggle();
		
		_selectPopDown( _gadget* owner , _contextMenuEntryList* lst );
};

class _select : public _gadget, public _interface_input {
	
	private:
	
		friend class _selectPopDown;
		
		_selectPopDown*			popDownList;
		
		_contextMenuEntryList	entries;
		_label*					label;
		_s32					selected;
		
		// Event - Handler
		static _gadgetEventReturnType	refreshHandler( _gadgetEvent event );
		static _gadgetEventReturnType	mouseHandler( _gadgetEvent event );
		static _gadgetEventReturnType	dialogCloseHandler( _gadgetEvent event );
	
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
		
		//! Default Constructor
		_select( _coord x , _coord y , _length w , _contextMenuEntryList lst = _contextMenuEntryList( { { -1 , "----" } } /* Empty List */ ) , _gadgetStyle style = _defaultStyle_ );
		
		//! Destrucor
		~_select();
};
#endif