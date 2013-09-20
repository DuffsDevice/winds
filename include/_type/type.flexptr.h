#ifndef _WIN_T_FLEXPTR_
#define _WIN_T_FLEXPTR_

namespace std
{
	template<class Type>
	class flex_ptr
	{
		private:
		
			Type*	ptr;
			
			void freeData(){
				delete this->ptr;
				this->ptr = nullptr;
			}
		
		public:
		
			// Default Ctor
			flex_ptr( Type*&& ptr = nullptr ) : ptr( ptr ) {}
			flex_ptr( const Type& other ) : ptr( new Type( other ) ) {}
			flex_ptr( Type&& other ) : ptr( new Type( move(other) ) ) {}
			
			// Copy Ctor
			flex_ptr( const flex_ptr<Type>& other ) : ptr( other.ptr ? new Type( *other.ptr ) : nullptr ) {}
			
			// Move Ctor
			flex_ptr( flex_ptr<Type>&& other ) : ptr( other.ptr ) { other.ptr = nullptr; }
			
			// Dtor
			~flex_ptr(){ this->freeData(); }
			
			// Copy operator
			flex_ptr<Type>& operator=( const flex_ptr<Type>& other ){
				this->freeData();
				this->ptr = other.ptr ? new Type( *other.ptr ) : nullptr;
				return *this;
			}
			
			// Copy operator for pointer Data
			flex_ptr<Type>& operator=( const Type& other ){
				this->freeData();
				this->ptr = new Type( other );
				return *this;
			}
			
			// Move operator for pointer Data
			flex_ptr<Type>& operator=( Type&& other ){
				this->freeData();
				this->ptr = new Type( move(other) );
				return *this;
			}
			
			// Move operator for pointers
			flex_ptr<Type>& operator=( Type*&& other ){
				this->freeData();
				this->ptr = (Type*)other;
				return *this;
			}
			
			// Move operator
			flex_ptr<Type>& operator=( flex_ptr<Type>&& other ){
				this->freeData();
				this->ptr = other.ptr;
				other.ptr = nullptr;
				return *this;
			}
			
			// Delete operation
			flex_ptr<Type>& operator=( std::nullptr_t ){
				this->freeData();
				return *this;
			}
			
			// Dereferencing operator
			Type& operator*(){
				if( this->ptr )
					return *this->ptr;
				static Type t = Type();
				return t;
			}
			
			// Const Dereferencing operator
			const Type& operator*() const {
				if( this->ptr )
					return *this->ptr;
				static Type t = Type();
				return t;
			}
			
			// Dereferencing operator
			Type* operator->(){
				return this->ptr;
			}
			
			// Const Dereferencing operator
			const Type* operator->() const {
				return this->ptr;
			}
			
			// Cast to boolean
			operator bool() const {
				return this->ptr != nullptr;
			}
			
			// Type cast operator
			operator Type*() const {
				return this->ptr;
			}
			
			// Equality operator
			bool operator==( const flex_ptr<Type>& other ) const {
				return this->ptr == other.ptr;
			}
			
			// Equality operator
			bool operator==( const Type* other ) const {
				return this->ptr == other;
			}
			
			// Un-Equality operator
			bool operator!=( const flex_ptr<Type>& other ) const {
				return this->ptr != other.ptr;
			}
			
			// Un-Equality operator
			template<class Type2>
			bool operator!=( const Type* other ) const {
				return this->ptr != other;
			}
			
			// Get the contained pointer
			Type* data() const {
				return this->ptr;
			}
			
			// Release the contained object
			Type* release(){
				Type* tmp = this->ptr;
				this->ptr = nullptr;
				return tmp;
			}
	};
}

using std::flex_ptr;

#endif