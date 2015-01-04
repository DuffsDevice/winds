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
		
		//! Move/Copy Ctor
		_uniquePtr( _uniquePtr<T>&& ) = default;
		_uniquePtr( const _uniquePtr<T>& ) = delete;
		
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
		
		
		//! Move/Copy Assignment operators
		_uniquePtr& operator=( _uniquePtr<T>&& ) = default;
		_uniquePtr& operator=( const _uniquePtr<T>& ) = delete;
		
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

// Array-Specialization
template<typename T>
class _uniquePtr<T[]> : public std::unique_ptr<T[]>
{
	private:
		using base = std::unique_ptr<T[]>;
	
	public:
		
		//! Default Ctor
		_uniquePtr() : base() {}
		
		//! Curstom Ctor for pointers
		_uniquePtr( T*&& val ) :
			base( std::move( val ) )
		{}
		
		//! Move/Copy Ctor
		_uniquePtr( _uniquePtr<T[]>&& ) = default;
		_uniquePtr( const _uniquePtr<T[]>& ) = delete;
		
		
		//! Move/Copy Assignment operators
		_uniquePtr& operator=( _uniquePtr<T[]>&& ) = default;
		_uniquePtr& operator=( const _uniquePtr<T[]>& ) = delete;
		
		//! Assignment operator for pointers
		_uniquePtr& operator=( T*&& val ){
			base::reset( move(val) );
			return *this;
		}
		
		//! Cast to the obtained pointer type
		operator T*() const { return base::get(); }
		
		//! Random Acces operator
		T& operator[]( std::size_t index ){ return base::operator[]( index ); }
		const T& operator[]( std::size_t index ) const { return base::operator[]( index ); }
};

#endif