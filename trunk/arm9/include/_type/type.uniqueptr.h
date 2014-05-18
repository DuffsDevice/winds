#ifndef _WIN_T_UNIQUEPTR_
#define _WIN_T_UNIQUEPTR_

#include <memory>

template<typename T>
class _uniquePtr : public std::unique_ptr<T>
{
	private:
		using base = std::unique_ptr<T>;
	
	public:
		
		//! Default Ctor
		_uniquePtr() : base() {}
		
		//! Custom Ctor for objects
		_uniquePtr( T&& val ) :
			base( new T( move( val ) ) )
		{}
		_uniquePtr( const T& val ) :
			base( new T( val ) )
		{}
		
		//! Curstom Ctor for pointers
		_uniquePtr( T*&& val ) :
			base( move( val ) )
		{}
		_uniquePtr( const T*& val ) :
			base( val ? new T( *val ) : nullptr )
		{}
		
		
		//! Assigment operator for objects
		_uniquePtr& operator=( T&& val ){
			base::reset( new T( move(val) ) );
			return *this;
		}
		_uniquePtr& operator=( const T& val ){
			base::reset( new T( val ) );
			return *this;
		}
		
		//! Assignment operator for pointers
		_uniquePtr& operator=( T*&& val ){
			base::reset( move(val) );
			return *this;
		}
		_uniquePtr& operator=( const T*& val ){
			base::reset( val ? new T( *val ) : nullptr );
			return *this;
		}
		
		//! Cast to the obtained object type
		operator T&() const { return *base::get(); }
		
		//! Cast to the obtained pointer type
		operator T*() const { return base::get(); }
};

#endif