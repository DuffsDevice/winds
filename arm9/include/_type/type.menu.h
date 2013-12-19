#ifndef _WIN_T_MENU_
#define _WIN_T_MENU_

#include "_type/type.h"
#include "_type/type.assocvector.h"
#include "_type/type.callback.h"
#include "_type/type.menu.rule.h"

using _menuEntry = _pair<_int,string>;
using _menuEntryList = _assocVector<_int,string>;
using _rawMenuEntryLists = _assocVector<_int,_menuEntryList>;
using _menuHandler = void( _int listIndex , _int listEntryIndex );
using HandlerTuple = _tuple<_menuHandlerRule,_callback<_menuHandler>*>;

class _menu : private _rawMenuEntryLists
{
	private:
		
		_vector<HandlerTuple> handlers;
		_bool sorted;
		
		// Internal getter for one of the stored lists
		const _menuEntryList& getListInternal( _int index ) const ;
		_menuEntryList& getListInternal( _int index ){
			return const_cast<_menuEntryList&>(((const _menu*)this)->getList( index ));
		}
		// Internal setter
		void setListInternal( _int index , _menuEntryList list ){
			(*this)[index] = move(list);
		}
		
		// Returns a from zero different number that represents 'index'
		constexpr static inline _int transformIndex( _int index ){
			return index >= 0 ? index + 1 : index;
		}
		
	public:
		
		//! Ctor
		_menu() : sorted( true )
		{}
		
		//! Add a handler for the menu applying for all entries for which the passed 'rule' returns true
		void addMenuHandler( _menuHandlerRule rule , _paramAlloc<_callback<_menuHandler>> handler ){
			this->handlers.push_back(
				make_tuple(
					rule
					, handler.get()
				)
			);
			this->sorted = false;
		}
		
		//! Set entry point of the menu; the list that makes the first appearence
		void setMainList( _menuEntryList list ){
			setListInternal( 0 , move(list) );
		}
		
		//! Get the entry point list of the menu
		const _menuEntryList& getMainList() const {
			return getListInternal( 0 );
		}
		_menuEntryList& getMainList(){
			return getListInternal( 0 );
		}
		
		//! Supply the list of index 'index' to the menu
		void setList( _int index , _menuEntryList list ){
			setListInternal( index , move(list) );
		}
		
		//! Get a reference to a specific list of index 'index'
		const _menuEntryList& getList( _int index ) const {
			return getListInternal( index );
		}
		_menuEntryList& getList( _int index ){
			return getListInternal( index );
		}
		
		//! Clear all lists
		void clear(){
			_rawMenuEntryLists::clear();
		}
		
		//! Clear all registered handlers
		void clearHandlers(){
			for( auto entry : this->handlers )
				delete std::get<1>(entry);
			this->handlers.clear();
		}
		
		~_menu(){
			clearHandlers();
		}
		
		//! Call the handler that applies to the passed list number and entry number
		void callHandler( _int listIndex ,_int entryIndex );
		
		//static _int getLinkFromText( const string& entry ){
		//	const char* str = entry.c_str();
		//	const char* strEnd = str + entry.length();
		//	const char* strCur = strEnd;
		//	while( strCur < strEnd && *strCur != "$" );
		//	if( strCur != strEnd )
		//		return string2int( strCur );
		//	return -1;
		//}
};

#endif