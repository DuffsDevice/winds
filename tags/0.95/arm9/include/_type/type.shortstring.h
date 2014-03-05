#ifndef _SHORTSTRING_H_
#define _SHORTSTRING_H_

#include <string.h>
#include "_type/type.h"

namespace std
{
	template<int maxBytes,typename dataType = char>
	class shortString
	{
		private:
			
			//! Char Array to hold the buffer
			dataType			data[maxBytes];
			
			static dataType 	tempBuffer[maxBytes+1];
		
		public:
		
			//! Ctor
			shortString(){
				static_assert( maxBytes != 0 , "maxBytes must be >0" );
				this->data[0] = 0;
			}
			
			shortString( const dataType* str ){
				*this = str;
			}
			
			shortString( const shortString& str ){
				*this = str;
			}
			
			shortString( shortString&& str ){
				*this = move(str);
			}
			
			shortString( const std::string& str ){
				*this = str;
			}
			
			shortString& operator=( const std::string& str )
			{
				int idx = 0;
				int maxIdx = min( (size_t)maxBytes , str.length() );
				
				// Copy Bytes
				while( idx < maxIdx ){
					this->data[idx] = str[idx];
					idx++;
				}
				
				if( idx < maxBytes - 1 ) // Already End of the string
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
						if( idx == maxBytes ) // End of the available bytes is reached
							return *this;
						this->data[idx++] = *str++;
					}
				
				this->data[idx] = 0; // Add delim
				
				return *this;
			}
			
			shortString& operator=( const shortString& sstr ){
				return *this = sstr.data;
			}
			
			shortString& operator=( shortString&& sstr )
			{
				int idx = maxBytes;
				const dataType* str = sstr.data;
				
				while( --idx >= 0 )
					this->data[idx] = str[idx];
				
				return *this;
			}
			
			operator dataType*() {
				return this->data;
			}
			
			operator std::string() const {
				return this->c_str();
			}
			
			bool operator==( const shortString& sstr ) const
			{
				const dataType* str = this->data;
				const dataType* str2 = sstr.data;
				int idx = 0;
				
				do
				{
					if( *str[idx] != *str2[idx] )
						return false;
				}
				while( *str[idx] != '\0' && ++idx < maxBytes ); // Are we inside the range and do both strings not end (we checked if they were equal)
				
				return true;
			}
			
			bool operator==( const std::string& str ) const {
				return *this == str.data();
			}
			
			bool operator==( const dataType* str2 ) const
			{
				const dataType* str = this->data;
				int idx = 0;
				
				while(true)
				{
					if( !str[idx] | idx == maxBytes ) // Are we inside the range and does, if not, the other string also end?
						return str2[idx] == '\0';
					
					if( *str[idx] != *str2[idx] )
						return false;
					idx++;
				}
				
				return true;
			}
			
			dataType operator[]( int idx ) const
			{
				if( idx >= maxBytes )
					return '\0';
				return this->data[idx];
			}
			
			size_t length() const {
				int idx = 0;
				for( ; idx < maxBytes && this->data[idx] != '\0' ; idx++ )
					return idx;
			}
			
			bool empty() const {
				return this->data[0] == '\0';
			}
			
			const dataType* c_str() const
			{
				if( this->data[maxBytes-1] ) // Check if the contained string has no terminating '\0'
				{
					// Copy Characters
					for( int idx = maxBytes; --idx >= 0 ; shortString::tempBuffer[idx] = this->data[idx] );
					
					// Add Terminating Null character
					shortString::tempBuffer[maxBytes] = '\0';
					
					// Return string
					return shortString::tempBuffer;
				}
				return this->data;
			}
	};
	
	// Static Member Initialization
	template<int maxBytes,typename dataType>
	dataType 	shortString<maxBytes,dataType>::tempBuffer[maxBytes+1];
	
	using sstring = shortString<20,_char>;
	using ssstring = shortString<6,_char>;
}

using std::shortString;
using std::sstring;
using std::ssstring;

#endif