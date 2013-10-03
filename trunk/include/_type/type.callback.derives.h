// Check if already included
#ifndef _WIN_T_CALLBACK_DERIVES_
#define _WIN_T_CALLBACK_DERIVES_

#include <type_traits>
#include "_type/type.callback.h"

//! For the Lua-Part of the callbacks
#include "_lua/lua.funcs.h"

class unknownClass;
template<typename T> class _staticCallback{ };

template<typename T,typename... Parameters>
class _staticCallback<T(Parameters...)> : public _callback<T(Parameters...)>
{
	private:
		
		T 			(*func)(Parameters...);
		
		// Internal
		_s8 equals( const _callback<T(Parameters...)>& param ) const {
			return ((_staticCallback<T(Parameters...)>&)param).func == this->func;
		}
		
	public:
		
		T operator()( Parameters... args ) const {
			if( this->func )
				return this->func( forward<Parameters>(args)... );
			return T();
		}
		
		//! Ctor
		_staticCallback( T (*func)(Parameters...) ) :
			_callback<T(Parameters...)>( _callbackType::static_func )
			, func( func )
		{}
};

template<typename T> class _classCallback{ };

template<typename T, typename... Parameters>
class _classCallback<T(Parameters...)> : public _callback<T(Parameters...)>
{
	private:
		
		unknownClass* 	instance;
		
		T				(unknownClass::*func)(Parameters...);
		
		// Internal
		_s8 equals( const _callback<T(Parameters...)>& param ) const {
			return ((_classCallback<T(Parameters...)>&)param).func == this->func;
		}
		
	public:
		
		T operator()( Parameters... args ) const {
			if( this->func && this->instance )
				return (this->instance->*func)( forward<Parameters>(args)... );
			return T();
		}
		
		//! Ctor
		template<class C>
		_classCallback( C* ptr , T (C::*func)(Parameters...) ) :
			_callback<T(Parameters...)>( _callbackType::class_func )
			, instance( reinterpret_cast<unknownClass*>(ptr) )
			, func( reinterpret_cast<T (unknownClass::*)(Parameters...)>(func) )
		{}
};

template<typename T> class _inlineCallback{ };

template<typename T, typename... Parameters>
class _inlineCallback<T(Parameters...)> : public _callback<T(Parameters...)>
{
	private:
		
		function<T(Parameters...)>					func;
		
		// Internal
		_s8 equals( const _callback<T(Parameters...)>& param ) const {
			return true;
		}
		
	public:
		
		T operator()(Parameters... args) const {
			return func( forward<Parameters>(args)... );
		}
		
		//! Ctor
		_inlineCallback( function<T(Parameters...)> func ) :
			_callback<T(Parameters...)>( _callbackType::inline_func )
			, func( func )
		{}
};

template<typename T> class _luaCallback{ };

template<typename T,typename... Parameters>
class _luaCallback<T(Parameters...)> : public _callback<T(Parameters...)>
{
	private:
		
		lua_State* 		state;
		int 			index;
		
		// Internal
		_s8 equals( const _callback<T(Parameters...)>& c ) const 
		{
			const _luaCallback<T(Parameters...)>& cb = (_luaCallback&) c;
			
			if( cb.state != this->state )								return 0;
			else if( cb.index == LUA_NOREF || this->index == LUA_NOREF )return -1;
			else if( cb.index == this->index )							return 1;
			
			// Push both indices onto stack and compare!
			lua_rawgeti( this->state , LUA_REGISTRYINDEX, index );
			lua_rawgeti( this->state , LUA_REGISTRYINDEX, cb.index );
			
			if ( lua_rawequal(this->state, -1, -2) ){ // Are they equal? 
				lua_pop(this->state, 2); // Remove both values
				return 1;
			}
			
			lua_pop(this->state, 2); // Remove both values
			return -1;
		}
		
		friend class _progLua;
		
	public:
		
		//! Move ctor needed!
		_luaCallback( _luaCallback&& cb ) :
			_callback<T(Parameters...)>( _callbackType::lua_func )
			, state( cb.state )
			, index( cb.index )
		{
			// invalidate the passed _luaCallback
			cb.state = 0;
		}
		
		//! Ctor
		_luaCallback( lua_State* state , int narg ) :
			_callback<T(Parameters...)>( _callbackType::lua_func )
			, state( state )
			, index( _luafunc::checkfunction( state , narg ) )
		{}
		
		//! Ctor
		_luaCallback( lua_State* state ) :
			_callback<T(Parameters...)>( _callbackType::lua_func )
			, state( state )
			, index( LUA_NOREF )
		{}
		
		//! Dtor
		~_luaCallback()
		{
			if( state )
				luaL_unref( state , LUA_REGISTRYINDEX , index ); // Unreference the handler
		}
		
		T operator()(Parameters... params) const
		{
			//! No state registered?
			if( !state || index == LUA_NOREF )
				return T();
			
			//! Put the Lua-Handler-Function on top of the Stack
			lua_rawgeti( state , LUA_REGISTRYINDEX, index );
			
			//! Call it with params
			_luafunc::push( state , forward<Parameters>(params)... );
			
			if( lua_pcall( state , sizeof...(Parameters) , 1 , 0 ) ){
				_luafunc::errorHandler( state , lua_tostring( state , -1 ) );
			}
			
			if( std::is_same<T,void>::value || lua_isnil( state , -1 ) )
				return T();
			//! Forward the Value returned by the Handler
			return _luafunc::check<T>( state , -1 );
		}
};

template<typename T> class _dummyCallback{ }; // A Callback like _gadgetHelpers::sizeParent

template<typename T,typename... Parameters>
class _dummyCallback<T(Parameters...)> : public _callback<T(Parameters...)>
{
	private:
		
		T (_dummyCallback::*func)(Parameters...) const;
		
		// Internal
		_callbackType getType() const { return _callbackType::dummy_func; }
		
		// Internal
		_s8 equals( const _callback<T(Parameters...)>& param ) const {
			return ((_dummyCallback<T(Parameters...)>&)param).func == this->func;
		}
		
	public:
		
		T operator()( Parameters... args ) const {
			if( this->func )
				return (this->*func)( forward<Parameters>(args)... );
			return T();
		}
		
		//! Ctor		
		template<class C>
		_dummyCallback( T (C::*executor)(Parameters...)const ) :
			_callback<T(Parameters...)>( _callbackType::dummy_func )
			, func( reinterpret_cast<T(_dummyCallback::*)(Parameters...)const>(executor) )
		{}
};

//
// Named Ctors
//
template<typename T,typename... Parameters>
constexpr _staticCallback<T(Parameters...)> make_callback( T (*func)(Parameters...) ){
	return _staticCallback<T(Parameters...)>( func );
}
template<typename T, class C, typename... Parameters>
constexpr _classCallback<T(Parameters...)> make_callback( C* obj , T (C::*func)(Parameters...) ){
	return _classCallback<T(Parameters...)>( obj , func );
}
template<typename T>
constexpr _inlineCallback<T> make_inline_callback( function<T> func ){
	return _inlineCallback<T>( func );
}

#endif