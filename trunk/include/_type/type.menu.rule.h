#ifndef _WIN_T_MENURULE_
#define _WIN_T_MENURULE_

#include "_type/type.h"

enum class _menuHandlerRuleType : _u8
{
	listRange,
	wholeList,
	wholeMenu
};

class _menuHandlerRule
{
	private:
	
		_int listIndex;
		_int startIndex;
		_int endIndex;
		_menuHandlerRuleType type;
		friend class _menu;
		
	public:
	
		//! Ctor for a rule that applies for a range of entryies in a list
		_menuHandlerRule( _int listIndex , _int sI , _int eI ) : listIndex( listIndex ) , startIndex( sI ) , endIndex( eI ) , type( _menuHandlerRuleType::listRange )
		{}
		//! Ctor for a rule that applies for the entry with index 'index' in the list 'listIndex'
		_menuHandlerRule( _int listIndex , _int index ) : listIndex( listIndex ) , startIndex( index ) , endIndex( index ) , type( _menuHandlerRuleType::listRange )
		{}
		//! Ctor for a rule that applies for a whole list
		_menuHandlerRule( _int listIndex ) : listIndex( listIndex ) , type( _menuHandlerRuleType::wholeList )
		{}
		//! Ctor for a rule that applies for all lists of a menu
		_menuHandlerRule() : type( _menuHandlerRuleType::wholeMenu )
		{}
		
		//! Copy and move Ctors
		_menuHandlerRule( const _menuHandlerRule& ) = default;
		_menuHandlerRule( _menuHandlerRule&& ) = default;
		
		//! Assign and move assign operators
		_menuHandlerRule& operator=( const _menuHandlerRule& ) = default;
		_menuHandlerRule& operator=( _menuHandlerRule&& ) = default;
		
		/**
		 * Method that one can call to check a specific entry
		 * in the list with index 'listIndex' against this rule
		 */
		bool operator()( _int listIndex , _int index ) const ;
} PACKED ;

#endif