#include "_type/type.menu.h"

void _menu::callHandler( _int listIndex , _int entryIndex )
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


const _menuEntryList& _menu::getListInternal( _int index ) const {
	auto it = this->find( index );
	if( it == this->end() ){
		static auto ret = _menuEntryList();
		return ret;
	}
	return it->second;
}