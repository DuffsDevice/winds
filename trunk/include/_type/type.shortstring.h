#ifndef _SHORTSTRING_H_
#define _SHORTSTRING_H_

#include <string.h>

namespace std
{
	template<int maxBytes,typename dataType = char>
	class shortString
	{
		private:
			
			//! Char Array to hold the buffer
			dataType			data[maxBytes];
		
		public:
		
			//! Ctor
			shortString(){
				this->data[0] = 0;
			}
			
			shortString( const dataType* str ){
				*this = str;
			}
			
			shortString( const shortString& str ){
				*this = str;
			}
			
			shortString( shortString&& str ){
				*this = str;
			}
			
			shortString( const std::string str ){
				*this = str;
			}
			
			shortString& operator=( const std::string str )
			{
				int idx = 0;
				for( const dataType c : str )
				{
					if( idx == ( maxBytes - 1 ) )
						break;
					this->data[idx++] = c;
				}
				this->data[idx] = 0;
				
				return *this;
			}
			
			shortString& operator=( const dataType* str )
			{
				int idx = 0;
				// Check if the data is valid
				if( str )
					while( *str )
					{
						if( idx == ( maxBytes - 1 ) )
							break;
						this->data[idx++] = *str++;
					}
				
				this->data[idx] = 0; // Add delim
				
				return *this;
			}
			
			shortString& operator=( const shortString& sstr )
			{
				int idx = 0;
				const dataType* str = sstr.data;
				
				// Check if the data is valid
				if( str )
					while( *str )
					{
						if( idx == ( maxBytes - 1 ) )
							break;
						this->data[idx++] = *str++;
					}
				
				this->data[idx] = 0; // Add delim
				
				return *this;
			}
			
			shortString& operator=( shortString&& sstr )
			{
				int idx = 0;
				const dataType* str = sstr.data;
				
				// Check if the data is valid
				if( str )
					while( *str )
					{
						if( idx == ( maxBytes - 1 ) )
							break;
						this->data[idx++] = *str++;
					}
				
				this->data[idx] = 0; // Add delim
				
				return *this;
			}
			
			operator dataType*()
			{
				return this->data;
			}
			
			operator std::string() const
			{
				return std::string( this->data );
			}
			
			dataType operator[]( int idx ) const
			{
				return this->data[ std::max( 0 , std::min( maxBytes - 1 , idx ) ) ];
			}
			
			const char* c_str() const { return this->data; }
		
	};
	
	using sstring = shortString<20,_char>;
	using ssstring = shortString<6,_char>;
}

using std::sstring;
using std::ssstring;

#endif