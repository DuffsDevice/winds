// Check if already included
#ifndef _WIN_T_CALLBACK_
#define _WIN_T_CALLBACK_

#include "_type/type.predefines.h"

enum class _callbackType : _u8 {
	none,
	static_func,
	dummy_func,
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
	private:
		
		_callbackType type;
		
	protected:
		
		virtual _s8 equals( const _callback& param ) const = 0; // 0 = false ; 1 = true ; -1 = semi-true
		
	public:
		
		typedef T return_type;
		
		virtual T operator()(Parameters...) const = 0;
		
		//! Check if two callbacks are the same
		_s8 operator==( const _callback<T(Parameters...)>& param ) const 
		{
			if( param.type != this->type )
				return 0;
			return this->equals( param );
		}
		
		//! Constructor
		_callback( _callbackType type ) :
			type( type )
		{}
		
		//! Virtual Destructor
		virtual ~_callback(){}
};

typedef _callback<void()> _voidCallback;
typedef _callback<void(int)> _intSetCallback;
typedef _callback<int(void)> _intGetCallback;

#endif