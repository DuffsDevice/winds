#ifndef _WIN_G_SELECTITEM_
#define _WIN_G_SELECTITEM_

#include "_type/type.h"
#include "_type/type.gadget.h"

class _selectItem : public _gadget {
	
	private:
	
		string	strVal;
		_s32	intVal;
		bool	active;
		
		// Event - Handler
		static _callbackReturn	refreshHandler( _event event );
		static _callbackReturn	mouseHandler( _event event );
	
	public:
	
		// To receive/set the value
		string	getStrValue(){ return this->strVal; }
		_s32	getIntValue(){ return this->intVal; }
		
		void	setIntValue( int value ){ this->intVal = value; }
		void	setStrValue( string value )
		{
			if( this->strVal != value ){
				this->strVal = value;
				this->bubbleRefresh( true );
			}
		}
		
		void setActive( bool active )
		{
			if( active != this->active )
			{
				this->active = active;
				bubbleRefresh( true );
			}
		}
		
		//! Default Constructor
		_selectItem( _s32 nth , _length width , string str , _s32 value , _style&& style = _style() );
};
#endif