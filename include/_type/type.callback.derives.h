// Check if already included
#ifndef _WIN_T_CALLBACK_DERIVES_
#define _WIN_T_CALLBACK_DERIVES_

#include "_type/type.callback.h"

//! For the Lua-Part of the callbacks
#include "_lua/lua.h"
#include "_lua/lauxlib.h"

class unknownClass;

extern void lua_callFunc( lua_State* L , int handler );

class _staticCallback : public _callback
{
	private:
		
		void (*func)();
		
		_callbackType getType() const { return _callbackType::static_func; }
		
		bool equals( const _callback& param ) const 
		{
			return ((_staticCallback&)param).func == this->func;
		}
		
	public:
		
		void operator()() const {
			if( this->func )
				this->func();
		}
		
		_staticCallback( void (*func)() ) :
			func( func )
		{}

};


class _classCallback : public _callback
{
	private:
		
		unknownClass* instance;
		
		void (unknownClass::*func)();
		
		_callbackType getType() const { return _callbackType::class_method; }
		
		bool equals( const _callback& param ) const 
		{
			return ((_classCallback&)param).func == this->func && ((_classCallback&)param).instance == this->instance;
		}
		
	public:
		
		void operator()() const {
			if( this->func && this->instance )
				(instance->*func)();
		}
		
		_classCallback( unknownClass* ptr , void (unknownClass::*func)() ) :
			instance( ptr )
			, func( func )
		{}

};


class _luaCallback : public _callback
{
	private:
		
		lua_State* 	state;
		int 		index;
		
		_callbackType getType() const { return _callbackType::lua_func; }
		
		bool equals( const _callback& param ) const 
		{
			return ((_luaCallback&)param).index == this->index && ((_luaCallback&)param).state == this->state;
		}
		
	public:
		
		void operator()() const {
			lua_callFunc( state , index );
		}
		
		_luaCallback( lua_State* state , int index ) : 
			state( state )
			, index( index )
		{}

};

#endif