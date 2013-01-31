// Check if already included
#ifndef _WIN_T_CALLBACK_
#define _WIN_T_CALLBACK_

#include "_type/type.h"
#include "_type/type.event.h"

enum class _callbackClassType : _u8 {
	none,
	static_func,
	inline_func,
	class_func,
	lua_func
};

enum class _callbackType : _u8 {
	voidFunc,
	intFunc,
	eventFunc
};

struct _callbackData
{
	_tempTime	startTime;
	_tempTime	duration;
	bool 		repeating;
};

class _callback
{
	protected:
		
		virtual _callbackClassType getType() const = 0;
		
		virtual _u8 equals( const _callback& param ) const = 0; // 0 = false ; 1 = true ; -1 = semi-true
		
	public:
		
		virtual void operator()() const = 0;
		virtual int operator()( int ) const = 0;
		virtual _callbackReturn operator()( _event ) const = 0;
		
		_u8 operator==( const _callback& param ) const 
		{
			if( param.getType() != this->getType() )
				return 0;
			return this->equals( param );
		}
		
		virtual ~_callback(){}

};

#include "_type/type.callback.derives.h"

#endif