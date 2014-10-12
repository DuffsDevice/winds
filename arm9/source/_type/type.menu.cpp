#include <_type/type.menu.h>

void _menu::callHandler( _u16 listIndex , _u16 entryIndex ) const
{
	// Sort list!
	if( !this->sorted )
	{
		std::sort( this->handlers.begin() , this->handlers.end()
			, []( const HandlerPair& val1 , const HandlerPair& val2 )->bool{ return val1.first.type < val2.first.type; }
		);
		this->sorted = true;
	}
	
	for( auto entry : this->handlers )
	{
		// Check if rule evaluates to true
		if( entry.first( listIndex , entryIndex ) == true )
		{
			// Call handler
			(*entry.second)( listIndex , entryIndex );
			return;
		}
	}
}

const _menuEntryList& _menu::getList( _u16 index ) const {
	auto it = this->find( index );
	if( it == this->end() ){
		static auto ret = _menuEntryList();
		return ret;
	}
	return it->second;
}

_menuEntryList& _menu::getList( _u16 index ){
	auto it = this->find( index );
	if( it == this->end() ){
		static auto ret = _menuEntryList();
		return ret;
	}
	return it->second;
}

_literal _menu::divider = "----";