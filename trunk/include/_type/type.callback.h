// Check if already included
#ifndef _WIN_T_CALLBACK_
#define _WIN_T_CALLBACK_

#include "_type/type.h"
#include "_type/type.event.h"

enum class _callbackClassType : _u8 {
	none,
	static_func,
	fast_event_func,
	inline_func,
	class_func,
	lua_func
};

template<typename T>
class _callback{
};

template<typename T,typename... Parameters>
class _callback<T(Parameters...)>
{
	protected:
		
		virtual _callbackClassType getType() const = 0;
		
		virtual _s8 equals( const _callback& param ) const = 0; // 0 = false ; 1 = true ; -1 = semi-true
		
	public:
		
		virtual T operator()(Parameters...) const = 0;
		
		_s8 operator==( const _callback<T(Parameters...)>& param ) const 
		{
			if( param.getType() != this->getType() )
				return 0;
			return this->equals( param );
		}
		
		virtual ~_callback(){}
};

typedef _callback<void()> _voidCallback;
typedef _callback<void(int)> _intSetCallback;
typedef _callback<int(void)> _intGetCallback;

#endif