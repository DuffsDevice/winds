#ifndef _WIN_T_MENU_
#define _WIN_T_MENU_

#include "_type/type.h"
#include "_type/type.assocvector.h"
#include "_type/type.callback.h"
#include "_type/type.menu.rule.h"
#include <memory>

// Specifies all the data that is stored for each entry
struct _menuEntry
{
	string	text;
	_u16	linkedList;
	
	//! Default Ctor
	_menuEntry( string text = "" , _u16 linkedList = 0 ) : text( move(text) ) , linkedList( linkedList ) {}
	
	//! Ctor
	_menuEntry( _literal text , _u16 linkedList = 0 ) : text( text ) , linkedList( linkedList ) {}
	
	//! Ctor from tuple
	_menuEntry( _tuple<string,_u16> tuple ) : _menuEntry( move(get<0>(tuple)) , get<1>(tuple) ) {}
	
	//! Copy & Move Ctor
	_menuEntry( _menuEntry&& menu ) : text( move(menu.text) ) , linkedList( menu.linkedList ) {}
	_menuEntry( const _menuEntry& menu ) : text( menu.text ) , linkedList( menu.linkedList ) {}
	
	//! Copy & Move Assignment Operators
	_menuEntry& operator=( _menuEntry&& menu ){
		text		= move(menu.text);
		linkedList	= menu.linkedList;
		return *this;
	}
	_menuEntry& operator=( const _menuEntry& menu ){
		text		= menu.text;
		linkedList	= menu.linkedList;
		return *this;
	}
	
	//! Cast to tuple
	operator _tuple<string,_u16>(){ return make_tuple( text , linkedList ); }
};

using _menuEntryList		= _assocVector<_u16,_menuEntry>;		// Specifies a list of entries, sorted by their index
using _rawMenuEntryLists	= _assocVector<_u16,_menuEntryList>;	// Specifies a collection of lists. The list at index [0] is the starting point of the menu

// The type that a handler must have to receive selections of individual menu entries
using _menuHandler			= void( _u16 listIndex , _u16 listEntryIndex );
class _menu : private _rawMenuEntryLists
{
	private:
		
		// Internal Helper types
		using PtrType		= std::shared_ptr<_callback<_menuHandler>>;
		using HandlerPair	= _pair<_menuHandlerRule,PtrType>;
		
		mutable _vector<HandlerPair>	handlers;
		mutable _bool					sorted;
		
	public:
		
		//! Ctor
		_menu() : sorted( true )
		{}
		
		//! Ctor with main List
		_menu( _menuEntryList mainList ) : 
			_rawMenuEntryLists( { { 0 , mainList } } )
			, sorted( true )
		{}
		
		//! Add a handler for the menu applying for all entries for which the passed 'rule' returns true
		void addHandler( _menuHandlerRule rule , _paramAlloc<_callback<_menuHandler>> handler ){
			this->handlers.push_back(
				make_pair(
					rule
					, PtrType( handler.get() )
				)
			);
			this->sorted = false;
		}
		
		//! Supply the list of index 'index' to the menu
		void setList( _menuEntryList list ){
			(*this)[0] = move(list);
		}
		void setList( _u16 listIndex , _menuEntryList list ){
			(*this)[listIndex] = move(list);
		}
		
		//! Get a reference to a specific list of index 'index'
		const _menuEntryList&	getList( _u16 listIndex = 0 ) const ;
		_menuEntryList&			getList( _u16 listIndex = 0 );
		
		//! Clear all lists
		void clearMenu(){
			_rawMenuEntryLists::clear();
		}
		
		//! Clear all registered handlers
		void clearHandlers(){
			this->handlers.clear();
		}
		
		//! Call the handler that applies to the passed list number and entry number
		void callHandler( _u16 listIndex , _u16 entryIndex ) const ;
		
		//! String the represents a dividing line
		static _literal divider;
};

#endif