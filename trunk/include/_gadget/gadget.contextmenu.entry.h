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
		_s32	id;
		
		// Event Handlers
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn updateHandler( _event event );
		static _callbackReturn clickHandler( _event event );
		static _callbackReturn mouseHandler( _event event );
	
	public:
	
		//! Get text
		string getStrValue(){ return this->text; }
		
		//! Get id
		_s32 getIntValue(){ return this->id; }
		
		//! Ctor
		_contextMenuEntry( _s32 index , string value , _optValue<_length>&& width = ignore , _style&& style = _style() );
};

#endif