#ifndef _WIN_T_REF_
#define _WIN_T_REF_

#include <_type/type.h>
#include <type_traits>

// This class is a pointer wrapper that can hold both weak and strong pointers
template<typename T>
class _ref
{
	private:
		
		T*		ref;
		bool	strong : 8;
		
		// Eventually frees owning storage
		void destruct(){
			if( this->strong && this->ref )
				delete this->ref;
			this->ref = nullptr;
		}
		
		template <typename Type> struct is__ref : std::false_type {};
		template <typename Type> struct is__ref<_ref<Type>> : std::true_type {};
		
		template<typename Type>
		using is_value = typename std::enable_if<
			!std::is_pointer<typename std::decay<Type>::type>::value
			&& !is__ref<typename std::decay<Type>::type>::value
			, void*
		>::type;
		
		// Friend all other templates
		template<typename>
		friend class _ref;
		
		// Friend comparison operators
		template<typename T2>						friend bool operator==( const _ref<T2>&	, T2* );
		template<typename T2>						friend bool operator!=( const _ref<T2>&	, T2* );
		template<typename T2>						friend bool operator==( T2*				, const _ref<T2>& );
		template<typename T2>						friend bool operator!=( T2*				, const _ref<T2>& );
		template<typename T2, typename T3>			friend bool operator==( const _ref<T2>&	, const _ref<T3>& );
		template<typename T2, typename T3>			friend bool operator!=( const _ref<T2>&	, const _ref<T3>& );
		template<typename T2>						friend _ref<T2> ref_own( T2* ptr );
		template<typename T2, typename T3, class>	friend _ref<T3> ref_own( T2& ptr );
		
	public:
		
		
		//! Default Ctor
		_ref() :
			ref( nullptr )
			, strong( false )
		{}
		_ref( std::nullptr_t ) :
			ref( nullptr )
			, strong( false )
		{}
		
		//! Ctor using (weak) ptr
		template<typename T2>
		_ref( T2* ptr ) :
			ref( (T*)ptr )
			, strong( false )
		{}
		
		//! Move contructor from different class
		template<typename T2>
		_ref( _ref<T2>&& from ) :
			ref( (T*)from.ref )
			, strong( from.strong )
		{
			from.strong = false;
		}
		
		//! Weak pointer constructor
		template<typename T2>
		_ref( const _ref<T2>& from ) :
			ref( (T*) from.ref )
			, strong( false )
		{}
		
		//! Dtor
		~_ref(){
			destruct();
		}
		
		
		
		//! Assignment operators taking _ref
		template<typename T2>
		_ref<T>& operator=( _ref<T2>&& from ){
			destruct();
			this->ref = (T*)from.ref;
			this->strong = from.strong;
			from.strong = false;
			return *this;
		}
		template<typename T2>
		_ref<T>& operator=( const _ref<T2>& from ){
			destruct();
			this->ref = (T*)from.ref;
			this->strong = false;
			return *this;
		}
		
		//! Resets the _ref
		_ref<T>& operator=( std::nullptr_t ptr ){
			destruct();
			this->ref = nullptr;
			this->strong = false;
			return *this;
		}
		
		//! Assigns the pointer of the supplied instance to this one
		//! If the supplied object holds the same pointer as this one,
		//! the resulting _ref object will preserve ownership, if one
		//! of the input _ref-objects has ownership of the pointer
		template<typename T2>
		_ref<T>& operator |= ( _ref<T2>&& from ){
			if( from.ref == this->ref )
				this->strong |= from.strong;
			else{
				destruct();
				this->ref = (T*)from.ref;
				this->strong = from.strong;
				from.strong = false;
			}
			return *this;
		}
		
		
		//! Other Assignment operators
		template<typename T2>
		_ref<T>& operator=( T2* ptr ){
			destruct();
			this->ref = (T*)ptr;
			this->strong = false;
			return *this;
		}
		
		
		//! Get the contained pointer
		T* data() const { return this->ref; }
		
		//! Transfers ownership to this _ref instance
		void give( T*&& ptr ){
			this->destruct();
			this->ref = ptr;
			this->strong = true;
		}
		void give( T&& obj ){
			typedef typename std::decay<T>::type T2;
			this->destruct();
			this->ref = new T2(move(obj));
			this->strong = true;
		}
		
		//! Let this _ref instance point to the supplied object while not owning it
		void set( T* ptr ){
			this->destruct();
			this->ref = ptr;
			this->strong = false;
		}
		void set( T& obj ){
			this->destruct();
			this->ref = &obj;
			this->strong = false;
		}
		
		//! Returns true, if the currently contained pointer is strong
		bool isStrong() const { return this->strong; }
		
		//! Dereferencing operator
		T& operator*() const { return *this->ref; }
		
		//! Dereferencing access operator
		T* operator->() const { return this->ref; }
		
		//! Cast to template Type reference
		template<typename T2>
		operator T2&() const { return (T2&)*this->ref; }
		
		//! Cast to template Type pointer
		template<typename T2>
		operator T2*() const { return (T2*)this->ref; }
		
		//! Cast to const _ref
		operator _ref<const T>&() const { return _ref<const T>( this->ref ); }
		
		//! Cast to bool
		operator bool() const { return this->ref; }
		
		//! Cast to negated bool
		bool operator !() const { return !this->ref; }
};


// No Arrays!
template<typename T>
class _ref<T[]>
{
	public:
		_ref() = delete;
};


//! Comparison operators
template<typename T>
static unused inline bool operator==( const _ref<T>& ref , T* ptr ){ return ref.ref == ptr;}
template<typename T>
static unused inline bool operator==( T* ptr , const _ref<T>& ref ){ return ref.ref == ptr; }
template<typename T, typename T2>
static unused inline bool operator==( const _ref<T>& ref , const _ref<T2>& ptr ){ return ref.ref == ref.ref; }

template<typename T>
static unused inline bool operator!=( const _ref<T>& ref , T* ptr ){ return ref.ref != ptr; }
template<typename T>
static unused inline bool operator!=( T* ptr , const _ref<T>& ref ){ return ref.ref != ptr; }
template<typename T, typename T2>
static unused inline bool operator!=( const _ref<T>& ref , const _ref<T2>& ptr ){ return ref.ref != ref.ref; }

//! Named Ctor with strong pointer
template<typename T>
static unused inline  _ref<T> ref_own( T* ptr ){
	_ref<T> ref{ptr};
	ref.strong = true;
	return move(ref);
}
//! Named Ctor with strong pointer
template<
	typename T
	, typename T2 = typename std::decay<T>::type
	, class = typename std::enable_if<!std::is_pointer<T2>::value>::type
>
static unused inline  _ref<T2> ref_own( T& obj ){
	_ref<T2> ref{new T2(move(obj))};
	ref.strong = true;
	return move(ref);
}

#endif