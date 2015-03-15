#ifndef _WIN_T_FLEXPTR_
#define _WIN_T_FLEXPTR_

template<class Type>
class _flexPtr
{
	private:
	
		Type*	ptr;
		
		void destruct(){
			delete this->ptr;
			this->ptr = nullptr;
		}
		
		template<typename T>
		using CopyCtible = typename std::is_copy_constructible<T>;
		
		// Does copy construction if possible
		template<typename T>
			typename std::enable_if<CopyCtible<T>::value,T*>::type
		makeCopy( const T& from ){
			return new T( from );
		}
		template<typename T>
			typename std::enable_if<!CopyCtible<T>::value,T*>::type
		makeCopy( const T& from ){
			return nullptr;
		}
	
	public:
	
		// Default Ctor
		_flexPtr( Type*&& ptr = nullptr ) : ptr( ptr ) {}
		_flexPtr( const Type& other ) : ptr( new Type( other ) ) {}
		_flexPtr( Type&& other ) : ptr( new Type( move(other) ) ) {}
		
		// Copy Ctor
		_flexPtr( const _flexPtr<Type>& other ) : ptr( other.ptr ? makeCopy<Type>(*other.ptr) : nullptr ) {}
		
		// Move Ctor
		_flexPtr( _flexPtr<Type>&& other ) : ptr( other.ptr ) { other.ptr = nullptr; }
		
		// Dtor
		~_flexPtr(){ this->destruct(); }
		
		// Copy operator
		_flexPtr<Type>& operator=( const _flexPtr<Type>& other ){
			this->destruct();
			this->ptr = other.ptr ? makeCopy<Type>(*other.ptr) : nullptr ;
			return *this;
		}
		
		// Copy operator for pointer Data
		_flexPtr<Type>& operator=( const Type& other ){
			this->destruct();
			this->ptr = makeCopy<Type>(other);
			return *this;
		}
		
		// Move operator for pointer Data
		_flexPtr<Type>& operator=( Type&& other ){
			this->destruct();
			this->ptr = new Type( move(other) );
			return *this;
		}
		
		// Move operator for pointers
		_flexPtr<Type>& operator=( Type*&& other ){
			this->destruct();
			this->ptr = (Type*)other;
			return *this;
		}
		
		// Move operator
		_flexPtr<Type>& operator=( _flexPtr<Type>&& other ){
			this->destruct();
			this->ptr = other.ptr;
			other.ptr = nullptr;
			return *this;
		}
		
		// Delete operation
		_flexPtr<Type>& operator=( std::nullptr_t ){
			this->destruct();
			return *this;
		}
		
		// Dereferencing operator
		Type& operator*(){ return *this->ptr; }
		
		// Const Dereferencing operator
		const Type& operator*() const { return *this->ptr; }
		
		// Dereferencing operator
		Type* operator->(){ return this->ptr; }
		
		// Const Dereferencing operator
		const Type* operator->() const { return this->ptr; }
		
		// Cast to boolean
		operator bool() const {
			return this->ptr != nullptr;
		}
		
		// Type cast operator
		operator Type*() const{ return this->ptr; }
		
		// Equality operator
		bool operator==( const _flexPtr<Type>& other ) const {
			return this->ptr == other.ptr;
		}
		
		// Equality operator
		bool operator==( const Type* other ) const {
			return this->ptr == other;
		}
		
		// Un-Equality operator
		bool operator!=( const _flexPtr<Type>& other ) const {
			return this->ptr != other.ptr;
		}
		
		// Un-Equality operator
		bool operator!=( const Type* other ) const {
			return this->ptr != other;
		}
		
		// Get the contained pointer
		Type* data() const { return this->ptr; }
		
		// Release the contained object
		Type*&& release(){
			Type* tmp = this->ptr;
			this->ptr = nullptr;
			return move(tmp);
		}
};

#endif