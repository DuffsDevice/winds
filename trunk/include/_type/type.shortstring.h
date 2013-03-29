#ifndef _SHORTSTRING_H_
#define _SHORTSTRING_H_

#include <string.h>

namespace std
{

	template<int maxBytes>
	class shortString
	{
		private:
			
			//! Char Array to hold the buffer
			char			data[maxBytes];
		
		public:
		
			//! Ctor
			shortString(){
				this->data[0] = 0;
			}
			
			shortString( const char* str ){
				*this = str;
			}
			
			shortString( const shortString& str ){
				*this = str;
			}
			
			shortString( shortString&& str ){
				*this = str;
			}
			
			shortString( const string str )
			{
				*this = str;
			}
			
			shortString& operator=( const string str )
			{
				int idx = 0;
				for( const char c : str )
				{
					if( idx > ( maxBytes - 2 ) )
						break;
					this->data[idx++] = c;
				}
				this->data[idx] = 0;
				
				return *this;
			}
			
			shortString& operator=( const char* str )
			{
				int idx = 0;
				// Check if the data is valid
				if( str )
					while( *str )
					{
						if( idx > ( maxBytes - 2 ) )
							break;
						this->data[idx++] = *str++;
					}
				
				this->data[idx] = 0; // Add delim
				
				return *this;
			}
			
			shortString& operator=( const shortString& sstr )
			{
				int idx = 0;
				const char* str = sstr.data;
				
				// Check if the data is valid
				if( str )
					while( *str )
					{
						if( idx > ( maxBytes - 2 ) )
							break;
						this->data[idx++] = *str++;
					}
				
				this->data[idx] = 0; // Add delim
				
				return *this;
			}
			
			shortString& operator=( shortString&& sstr )
			{
				int idx = 0;
				const char* str = sstr.data;
				
				// Check if the data is valid
				if( str )
					while( *str )
					{
						if( idx > ( maxBytes - 2 ) )
							break;
						this->data[idx++] = *str++;
					}
				
				this->data[idx] = 0; // Add delim
				
				return *this;
			}
			
			operator char*()
			{
				return this->data;
			}
			
			operator std::string()
			{
				return std::string( this->data );
			}
			
			char operator[]( int idx )
			{
				return this->data[ std::max( 0 , std::min( maxBytes , idx ) ) ];
			}
		
	};
	
	using sstring = shortString<6>;
}

#endif