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
extern _callbackReturn 	lua_callEventFn( lua_State* L , int handler , _event&& e );
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
		noinline _callbackReturn operator()( _event&& e ) const
		{
			if( this->eventFn )
			{				
				switch( this->funcType )
				{
					case _callbackType::eventFunc:
						return this->eventFn( (_event&&)e );
						break;
					// Allow calling an event Handler which is actually an int function
					case _callbackType::intFunc:
						return (_callbackReturn) this->intFn( e.getHeldTime() );
						break;
					// Allow calling an event Handler which is actually a void function
					default:
						this->voidFn();
						return handled;
				}
			}
			
			return not_handled;
		}
		
		_staticCallback( void (*voidFn)() ) : voidFn( voidFn ) , funcType( _callbackType::voidFunc ) {}
		
		_staticCallback( int (*intFn)( int ) ) : intFn( intFn ) , funcType( _callbackType::intFunc ) {}
		
		_staticCallback( _callbackReturn (*eventFn)( _event ) ) : eventFn( eventFn ) , funcType( _callbackType::eventFunc ) {}
};

class _fastEventCallback : public _callback
{
	private:
		
		_callbackReturn (*fn)( _event&& );
		
		// Internal
		_callbackClassType getType() const { return _callbackClassType::fast_event_func; }
		
		// Internal
		_u8 equals( const _callback& param ) const
		{
			return ((_fastEventCallback&)param).fn == this->fn;
		}
		
	public:
		
		noinline _callbackReturn operator()( _event&& e ) const	{
			if( this->fn )
				return this->fn( (_event&&)e );
			return not_handled;
		}
		void operator()() const {}
		int operator()( int ) const { return 0; }
		
		_fastEventCallback( _callbackReturn (*fn)( _event&& ) ) : fn( fn ) {}
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
		noinline _callbackReturn operator()( _event&& e ) const
		{
			if( this->eventFn && this->instance )
			{
				switch( this->funcType )
				{
					case _callbackType::eventFunc:
						return (this->instance->*eventFn)( (_event&&)e );
						break;
					// Allow calling an event Handler which is actually an int function
					case _callbackType::intFunc:
						return (_callbackReturn) (this->instance->*intFn)( e.getHeldTime() );
						break;
					// Allow calling an event Handler which is actually a void function
					default:
						(this->instance->*voidFn)();
						return handled;
						break;
				}
			}
			
			return not_handled;
		}
		
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
		_callbackReturn operator()( _event&& e ) const ;
		
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
		
		// Move operator needed!
		_luaCallback( _luaCallback&& cb ) :
			state( cb.state ),
			index( cb.index )
		{
			// invalidate the passed _luaCallback
			cb.state = 0;
		}
			
		void operator()() const {
			lua_callVoidFn( state , index );
		}
		int operator()( int i ) const {
			return lua_callIntFn( state , index , i );
		}
		_callbackReturn operator()( _event&& e ) const {
			return lua_callEventFn( state , index , (_event&&)e );
		}
		
		_luaCallback( lua_State* state , int narg ) : 
			state( state )
			, index( lua_checkFunction( state , narg ) )
		{}
		
		_luaCallback( lua_State* state ) : 
			state( state )
			, index( LUA_NOREF )
		{}
		
		~_luaCallback(){
			lua_popFunction( state , index );
		}

};

#endif