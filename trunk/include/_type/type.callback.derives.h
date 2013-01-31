// Check if already included
#ifndef _WIN_T_CALLBACK_DERIVES_
#define _WIN_T_CALLBACK_DERIVES_

#include "_type/type.callback.h"

//! For the Lua-Part of the callbacks
#include "_lua/lua.hpp"

#include <functional>

class unknownClass;

extern void 			lua_callVoidFn( lua_State* L , int handler );
extern int 				lua_callIntFn( lua_State* L , int handler , int i );
extern _callbackReturn 	lua_callEventFn( lua_State* L , int handler , _event e );
extern int 				lua_checkFunction( lua_State* L , int narg );
extern void 			lua_popFunction( lua_State* L , int index );

class _staticCallback : public _callback
{
	private:
		
		union{
			void 			(*voidFn)();
			int 			(*intFn)( int );
			_callbackReturn (*eventFn)( _event );
		};
		_callbackType		funcType;
		
		// Internal
		_callbackClassType getType() const { return _callbackClassType::static_func; }
		
		// Internal
		_u8 equals( const _callback& param ) const
		{
			return ((_staticCallback&)param).voidFn == this->voidFn;
		}
		
	public:
		
		void operator()() const ;
		int operator()( int i ) const ;
		_callbackReturn operator()( _event e ) const ;
		
		_staticCallback( void (*voidFn)() ) : voidFn( voidFn ) , funcType( _callbackType::voidFunc ) {}
		
		_staticCallback( int (*intFn)( int ) ) : intFn( intFn ) , funcType( _callbackType::intFunc ) {}
		
		_staticCallback( _callbackReturn (*eventFn)( _event ) ) : eventFn( eventFn ) , funcType( _callbackType::eventFunc ) {}

};


class _classCallback : public _callback
{
	private:
		
		unknownClass* instance;
		
		union{
			void 			(unknownClass::*voidFn)();
			int 			(unknownClass::*intFn)( int );
			_callbackReturn (unknownClass::*eventFn)( _event );
		};
		_callbackType		funcType;
		
		// Internal
		_callbackClassType getType() const { return _callbackClassType::class_func; }
		
		// Internal
		_u8 equals( const _callback& param ) const 
		{
			return ((_classCallback&)param).voidFn == this->voidFn;
		}
		
	public:
		
		void operator()() const ;
		int operator()( int i ) const ;
		_callbackReturn operator()( _event e ) const ;
		
		template<typename T>
		_classCallback( T* ptr , void (T::*voidFn)() ) :
			instance( reinterpret_cast<unknownClass*>(ptr) )
			, voidFn( reinterpret_cast<void (unknownClass::*)()>(voidFn) )
			, funcType( _callbackType::voidFunc )
		{}
		
		template<typename T>
		_classCallback( T* ptr , int (T::*intFn)( int ) ) :
			instance( reinterpret_cast<unknownClass*>(ptr) )
			, intFn( reinterpret_cast<int (unknownClass::*)( int )>(intFn) )
			, funcType( _callbackType::intFunc )
		{}
		
		template<typename T>
		_classCallback( T* ptr , _callbackReturn (T::*eventFn)( _event ) ) :
			instance( reinterpret_cast<unknownClass*>(ptr) )
			, eventFn( reinterpret_cast<_callbackReturn (unknownClass::*)( _event )>(eventFn) )
			, funcType( _callbackType::eventFunc )
		{}

};

class _inlineCallback : public _callback
{
	private:
		
		function<void()>					voidFn;
		function<int(int)>					intFn;
		function<_callbackReturn(_event)>	eventFn;
		
		// Internal
		_callbackClassType getType() const { return _callbackClassType::inline_func; }
		
		// Internal
		_u8 equals( const _callback& param ) const
		{
			return true;
		}
		
	public:
		
		void operator()() const ;
		int operator()( int i ) const ;
		_callbackReturn operator()( _event e ) const ;
		
		_inlineCallback( function<void()> voidFn ) :
			voidFn( voidFn )
			, intFn( nullptr )
			, eventFn( nullptr )
		{}
		
		_inlineCallback( function<int(int)> intFn ) :
			voidFn( nullptr )
			, intFn( intFn )
			, eventFn( nullptr )
		{}
		
		_inlineCallback( function<_callbackReturn(_event)> eventFn ) :
			voidFn( nullptr )
			, intFn( nullptr )
			, eventFn( eventFn )
		{}
};


class _luaCallback : public _callback
{
	private:
		
		lua_State* 		state;
		int 			index;
		
		// Internal
		_callbackClassType getType() const { return _callbackClassType::lua_func; }
		
		// Internal
		_u8 equals( const _callback& param ) const ;
		
		friend class _progLua;
		
	public:
		
		void operator()() const ;
		int operator()( int i ) const ;
		_callbackReturn operator()( _event e ) const ;
		
		_luaCallback( lua_State* state , int narg ) : 
			state( state )
			, index( lua_checkFunction( state , narg ) )
		{}
		
		_luaCallback( lua_State* state ) : 
			state( state )
			, index( LUA_NOREF )
		{}
		
		~_luaCallback();

};

#endif