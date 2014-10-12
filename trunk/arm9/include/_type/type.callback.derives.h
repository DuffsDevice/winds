// Check if already included
#ifndef _WIN_T_CALLBACK_DERIVES_
#define _WIN_T_CALLBACK_DERIVES_

#include <type_traits>
#include <_type/type.callback.h>
#include <_lua/lua.func.h>

template<typename T> class _staticCallback{ };

template<typename T,typename... Parameters>
class _staticCallback<T(Parameters...)> : public _callback<T(Parameters...)>
{
	private:
		
		// Pointer to function
		T 			(*func)(Parameters...);
		
		// Internal
		_s8 equals( const _callback<T(Parameters...)>& param ) const {
			return ((const _staticCallback<T(Parameters...)>&)param).func == this->func;
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
		
		class unknownClass;
		
		// Instance to invoke on
		unknownClass* 	instance;
		
		// Pointer to member function
		T				(unknownClass::*func)(Parameters...);
		
		// Internal
		_s8 equals( const _callback<T(Parameters...)>& param ) const {
			return ((const _classCallback<T(Parameters...)>&)param).func == this->func;
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
		
		// Function object that holds the function
		function<T(Parameters...)> func;
		
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
			return ((const _dummyCallback<T(Parameters...)>&)param).func == this->func;
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