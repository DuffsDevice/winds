#ifndef _WIN_T_SHORTSTRING_
#define _WIN_T_SHORTSTRING_

#include <string.h>
#include <_type/type.h>

template<int maxBytes,typename dataType = char>
class _shortString
{
	private:
		
		//! Char Array to hold the buffer
		dataType			data[maxBytes];
		
		//! Temporary Buffer that allows the _shortString to omit the \0 at the end of the string and save memory
		static dataType 	tempBuffer[maxBytes+1];
	
	public:
	
		//! Ctor
		_shortString(){
			static_assert( maxBytes != 0 , "maxBytes must be >0" );
			this->data[0] = 0;
		}
		//! Ctor from std::string
		_shortString( const std::string& str ){
			*this = str;
		}
		//! Copy Ctor from Character Array
		_shortString( const dataType* str ){
			*this = str;
		}
		//! Copy Ctor
		_shortString( const _shortString& str ){
			*this = str;
		}
		
		//! Move Ctor
		_shortString( _shortString&& str ){
			*this = str;
		}
		
		//! Get Raw Character Array
		dataType* array(){
			return this->data;
		}
		const dataType* array() const {
			return this->data;
		}
		
		//! std::string Assignment Operator
		_shortString& operator=( const std::string& str )
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
		
		//! Character Array Assignment Operator
		_shortString& operator=( const dataType* str )
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
		
		//! Assignment Operator
		_shortString& operator=( _shortString&& sstr ){
			return *this = (const _shortString&)sstr;
		}
		_shortString& operator=( const _shortString& sstr )
		{
			int idx = maxBytes;
			const dataType* str = sstr.data;
			
			while( --idx >= 0 )
				this->data[idx] = str[idx];
			
			return *this;
		}
		
		//! Convert to e.g. char* array
		//!@note Same as .c_str()
		operator dataType*(){
			return this->c_str();
		}
		operator const dataType*() const {
			return this->c_str();
		}
		
		//! Conversion Operator to std::string
		operator std::string() const {
			return this->c_str();
		}
		
		//! Comparison Operators
		bool operator<( const _shortString& sstr ) const {
			return strncmp( this->data , sstr.data , maxBytes ) < 0;
		}
		bool operator>( const _shortString& sstr ) const {
			return strncmp( this->data , sstr.data , maxBytes ) > 0;
		}
		bool operator>=( const _shortString& sstr ) const {
			return strncmp( this->data , sstr.data , maxBytes ) >= 0;
		}
		bool operator<=( const _shortString& sstr ) const {
			return strncmp( this->data , sstr.data , maxBytes ) <= 0;
		}
		bool operator==( const _shortString& sstr ) const {
			return strncmp( this->data , sstr.data , maxBytes ) == 0;
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
		
		//! Random Access Operator
		dataType& operator[]( int idx )
		{
			if( idx >= maxBytes )
				return _shortString::tempBuffer[0] = '\0';
			return this->data[idx];
		}
		const dataType& operator[]( int idx ) const
		{
			if( idx >= maxBytes )
				return _shortString::tempBuffer[0] = '\0';
			return this->data[idx];
		}
		
		//! Get String length of the string
		size_t length() const {
			int idx = 0;
			for( ; idx < maxBytes && this->data[idx] != '\0' ; idx++ )
				return idx;
		}
		
		//! Check if the shortString is empty
		bool empty() const {
			return this->data[0] == '\0';
		}
		
		//! Get the currently used character array used by the shortString
		const dataType* c_str() const
		{
			if( this->data[maxBytes-1] ) // Check if the contained string has no terminating '\0'
			{
				// Copy Characters
				for( int idx = maxBytes; --idx >= 0 ; _shortString::tempBuffer[idx] = this->data[idx] );
				
				// Add Terminating Null character
				_shortString::tempBuffer[maxBytes] = '\0';
				
				// Return string
				return _shortString::tempBuffer;
			}
			return this->data;
		}
};

// Hashing Function for use in e.g. std::unordered_map
namespace std
{
	template<int maxBytes,typename dataType>
	struct hash<_shortString<maxBytes,dataType>>
	{
		size_t operator()( const _shortString<maxBytes,dataType>& sstr ) const
		{
			// Hashing Function: djb2
			size_t hash = 5381;
			dataType curChar;
			const dataType* data = sstr.array();
			int curByte = maxBytes; // Counter
			while (curChar = *data++ && curByte-- > 0 )
				hash = ( hash * 33 ) ^ curChar;
			return hash;
		}
	};
}

// Static Member Initialization
template<int maxBytes,typename dataType>
dataType 	_shortString<maxBytes,dataType>::tempBuffer[maxBytes+1];

using sstring = _shortString<20,_char>;
using ssstring = _shortString<6,_char>;

#endif