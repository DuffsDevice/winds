// Check if already included
#ifndef _WIN_T_CALLBACK_
#define _WIN_T_CALLBACK_

#include "type.h"

enum class _callbackType : _u8 {
	none,
	static_func,
	class_method,
	lua_func
};

class _callback
{
	protected:
		
		virtual _callbackType getType() const = 0;
		
		virtual bool equals( const _callback& param ) const = 0;
		
	public:
		
		virtual void operator()() const = 0;
		
		bool operator==( const _callback& param ) const 
		{
			return param.getType() == this->getType() && this->equals( param );
		}
		
		virtual ~_callback(){}

};

#include "_type/type.callback.derives.h"

#endif