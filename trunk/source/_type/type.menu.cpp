#include "_type/type.menu.h"

void _menu::callHandler( _int listIndex , _int entryIndex )
{
	// Sort list!
	if( !this->sorted )
	{
		std::sort( this->handlers.begin() , this->handlers.end()
			, []( const HandlerTuple& val1 , const HandlerTuple& val2 )->bool{ return std::get<0>(val1).type < std::get<0>(val2).type; }
		);
		this->sorted = true;
	}
	
	for( auto entry : this->handlers )
	{
		// Check if rule evaluates to true
		if( std::get<0>(entry)( listIndex , entryIndex ) == true )
		{
			// Call handler
			(*std::get<1>(entry))( listIndex , entryIndex );
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