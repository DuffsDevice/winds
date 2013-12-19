#ifndef _WIN_G_CONTEXTMENUENTRY_
#define _WIN_G_CONTEXTMENUENTRY_

#include "_type/type.gadget.h"
#include "_type/type.gadget.helpers.h"

class _contextMenuEntry : public _gadget
{
	private:
		
		// Text to display
		string	text;
		
		// Index in the context menu
		_int	id;
		
		// Event Handlers
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn updateHandler( _event );
		static _callbackReturn mouseHandler( _event );
	
	public:
	
		//! Get text
		string getStrValue(){ return this->text; }
		
		//! Get id
		_int getIntValue(){ return this->id; }
		
		//! Ctor
		_contextMenuEntry( _optValue<_length> width , _int index , string value , _style&& style = _style() );
};

#endif