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
			
			operator dataType*() {
				return this->data;
			}
			
			operator std::string() const {
				return std::string( this->data );
			}
			
			template<int maxBytes2,typename dataType2>
			bool operator==( const shortString<maxBytes2,dataType2>& sstr ) const
			{
				const dataType* str = this->data;
				const dataType* str2 = sstr.data;
				
				while( *str && *str2 )
				{
					if( *str++ != *str2++ )
						return false;
				}
				if( *str != *str2 ) // Check if not both reached their ends at once
					return false;
				return true;
			}
			
			bool operator==( const std::string& str ) const
			{
				if( str.length() > maxBytes - 1 )
					return false;
				
				const dataType* myStr = this->data;
				
				for( const std::string::value_type& value : str )
				{
					if( *myStr++ != value )
						return false;
				}
				if( *myStr ) // Check if the shortstring is actually longer that the std::string we compare to
					return false;
				return true;
			}
			
			bool operator==( const dataType* str2 ) const
			{
				const dataType* str = this->data;
				
				while( *str && *str )
				{
					if( *str++ != *str2++ )
						return false;
				}
				if( *str != *str2 ) // Check if the both strings have the same size
					return false;
				return true;
			}
			
			dataType operator[]( int idx ) const
			{
				return this->data[ std::max( 0 , std::min( maxBytes - 1 , idx ) ) ];
			}
			
			size_t length() const {
				return strlen( this->data );
			}
			
			bool empty() const {
				return this->data[0] == '\0';
			}
			
			const char* c_str() const { return this->data; }
		
	};
	
	using sstring = shortString<20,_char>;
	using ssstring = shortString<6,_char>;
}

using std::sstring;
using std::ssstring;

#endif