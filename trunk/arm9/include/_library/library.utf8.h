// Copyright (c) 2014 Jakob Riedle (DuffsDevice)
// 
// Version 1.0
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef _UTF8_LIB_
#define _UTF8_LIB_

#include <_type/type.h>
#include <_type/type.uniqueptr.h>

class wstring
{
	private:
		
		class wstring_reference
		{
			private:
			
				_s32		index;
				wstring&	instance;
			
			public:
				
				//! Ctor
				wstring_reference( _s32 index , wstring& instance ) :
					index( index )
					, instance( instance )
				{}
				
				//! Cast to wide char
				operator _wchar() const { return instance.at( index ); }
				
				//! Assignment operator
				wstring_reference& operator=( _wchar ch ){
					instance.replace( index , ch );
					return *this;
				}
		};
		
		class wstring_raw_reference
		{
			private:
			
				_s32		rawIndex;
				wstring&	instance;
			
			public:
				
				//! Ctor
				wstring_raw_reference( _s32 rawIndex , wstring& instance ) :
					rawIndex( rawIndex )
					, instance( instance )
				{}
				
				//! Cast to wide char
				operator _wchar() const { return this->instance.rawAt( this->rawIndex ); }
				
				//! Assignment operator
				wstring_raw_reference& operator=( _wchar ch ){
					this->instance.rawReplace( this->rawIndex , this->instance.getIndexBytes( this->rawIndex ) , wstring( 1 , ch ) );
					return *this;
				}
		};
		
		class wstring_iterator : public std::iterator<std::forward_iterator_tag,_u32>
		{
			protected:
			
				_u32		rawIndex;
				wstring&	instance;
			
			public:
			
				//! Ctor
				wstring_iterator( _u32 rawIndex , wstring& instance ) :
					rawIndex( rawIndex )
					, instance( instance )
				{}
				
				//! Default function
				wstring_iterator( const wstring_iterator& ) = default;
				wstring_iterator& operator=( const wstring_iterator& it ){ this->rawIndex = it.rawIndex; return *this; }
				
				//! Get the index
				_s32 getIndex() const {
					return this->rawIndex;
				}
				
				//! Set the index
				void setIndex( _s32 rawIndex ){
					this->rawIndex = rawIndex;
				}
				
				//! Returns the wstring instance the iterator refers to
				wstring& getInstance() const { return this->instance; }
				
				//! Increase the Iterator by one
				wstring_iterator&  operator++(){ // prefix ++iter
					this->rawIndex += this->instance.getIndexBytes( this->rawIndex );
					return *this;
				}
				wstring_iterator& operator++( int ){ // postfix iter++
					this->rawIndex += this->instance.getIndexBytes( this->rawIndex );
					return *this;
				}
				
				//! Increase the Iterator %nth times
				friend wstring_iterator operator+( const wstring_iterator& it , _u32 nth );
				wstring_iterator& operator+=( _u32 nth ){
					while( nth-- > 0 )
						this->rawIndex += this->instance.getIndexBytes( this->rawIndex );
					return *this;
				}
				
				//! Get the difference between two iterators
				friend int operator-( const wstring_iterator& left , const wstring_iterator& right );
				
				//! Compare two iterators
				bool operator==( const wstring_iterator& it ) const { return this->rawIndex == it.rawIndex; }
				bool operator!=( const wstring_iterator& it ) const { return this->rawIndex != it.rawIndex; }
				bool operator>( const wstring_iterator& it ) const { return this->rawIndex > it.rawIndex; }
				bool operator>=( const wstring_iterator& it ) const { return this->rawIndex >= it.rawIndex; }
				bool operator<( const wstring_iterator& it ) const { return this->rawIndex < it.rawIndex; }
				bool operator<=( const wstring_iterator& it ) const { return this->rawIndex <= it.rawIndex; }
				
				//! Returns the value of the codepoint behind the iterator
				_wchar getValue(){ return this->instance.rawAt( this->rawIndex ); }
				
				//! Returns the value of the codepoint behind the iterator
				wstring_raw_reference operator*(){ return this->instance( this->rawIndex ); }
				
				//! Check whether there is a Node following this one
				bool hasNext() const { return this->instance.rawAt( this->rawIndex + this->instance.getIndexBytes( this->rawIndex ) ) != 0; }
				
				//! Sets the codepoint
				void setValue( _wchar value ){ instance.rawReplace( this->rawIndex , this->instance.getIndexBytes( this->rawIndex ) , wstring( 1 , value ) ); }
				
				//! Check if the Iterator is valid
				operator bool() const { return this->valid(); }
				
				//! Check if the Iterator is valid
				bool valid() const { return this->rawIndex < this->instance.size(); }
		};
		
		class wstring_const_iterator : public wstring_iterator
		{
			public:
				
				//! Ctor
				wstring_const_iterator( _u32 rawIndex , const wstring& instance ) :
					wstring_iterator( rawIndex , const_cast<wstring&>(instance) )
				{}
				
				//! Cast ctor
				wstring_const_iterator& operator=( const wstring_iterator& it ){ this->rawIndex = it.getIndex(); this->instance = it.getInstance(); return *this; }
				wstring_const_iterator( const wstring_iterator& it ) : wstring_iterator( it.getIndex() , it.getInstance() ) { }
				
				//! Default function
				wstring_const_iterator( const wstring_const_iterator& ) = default;
				wstring_const_iterator& operator=( const wstring_const_iterator& it ){ this->rawIndex = it.rawIndex; return *this; }
				
				//! Remove setter
				void setValue( _wchar value ) = delete;
				
				//! Returns the value behind the iterator
				_wchar operator*(){ return this->instance.rawAt( this->rawIndex ); }
		};
		
		class wstring_reverse_iterator
		{
			protected:
				
				_s32		rawIndex;
				wstring&	instance;
				
			public:
				
				//! Ctor
				wstring_reverse_iterator( _s32 rawIndex , wstring& instance ) :
					rawIndex( rawIndex )
					, instance( instance )
				{}
				
				//! Default function
				wstring_reverse_iterator( const wstring_reverse_iterator& ) = default;
				wstring_reverse_iterator& operator=( const wstring_reverse_iterator& it ){ this->rawIndex = it.rawIndex; return *this; }
				
				//! From wstring_iterator to wstring_reverse_iterator
				wstring_reverse_iterator& operator=( const wstring_iterator& it ){ this->rawIndex = it.getIndex(); this->instance = it.getInstance(); return *this; }
				wstring_reverse_iterator( const wstring_iterator& it ) : rawIndex( it.getIndex() ) , instance( it.getInstance() ) { }
				
				//! Get the index
				_s32 getIndex() const {
					return this->rawIndex;
				}
				
				//! Set the index
				void setIndex( _s32 rawIndex ){
					this->rawIndex = rawIndex;
				}
				
				//! Returns the wstring instance the iterator refers to
				wstring& getInstance() const { return this->instance; }
				
				wstring_reverse_iterator& operator++(){ // prefix iter++
					this->rawIndex -= this->instance.getIndexPreBytes( this->rawIndex );
					return *this;
				}
				wstring_reverse_iterator& operator++( int ){ // postfix iter++
					this->rawIndex -= this->instance.getIndexPreBytes( this->rawIndex );
					return *this;
				}
				friend wstring_reverse_iterator operator+( const wstring_reverse_iterator& it , _u32 nth );
				wstring_reverse_iterator& operator+=( _u32 nth ){
					while( nth-- > 0 )
						this->rawIndex -= this->instance.getIndexPreBytes( this->rawIndex );
					return *this;
				}
				
				//! Get the difference between two iterators
				friend int operator-( const wstring_reverse_iterator& left , const wstring_reverse_iterator& right );
				
				//! Compare two iterators
				bool operator==( const wstring_reverse_iterator& it ) const { return this->rawIndex == it.rawIndex; }
				bool operator!=( const wstring_reverse_iterator& it ) const { return this->rawIndex != it.rawIndex; }
				bool operator>( const wstring_reverse_iterator& it ) const { return this->rawIndex < it.rawIndex; }
				bool operator>=( const wstring_reverse_iterator& it ) const { return this->rawIndex <= it.rawIndex; }
				bool operator<( const wstring_reverse_iterator& it ) const { return this->rawIndex > it.rawIndex; }
				bool operator<=( const wstring_reverse_iterator& it ) const { return this->rawIndex >= it.rawIndex; }
				
				//! Returns the value of the codepoint behind the iterator
				_wchar getValue(){ return this->instance.rawAt( this->rawIndex ); }
				
				//! Sets the codepoint
				void setValue( _wchar value ){ instance.rawReplace( this->rawIndex , this->instance.getIndexBytes( this->rawIndex ) , wstring( 1 , value ) ); }
				
				//! Returns the value of the codepoint behind the iterator
				wstring_raw_reference operator*(){ return this->instance( this->rawIndex ); }
				
				//! Check whether there is a Node following this one
				bool hasNext() const { return this->rawIndex > 0; }
				
				//! Check if the Iterator is valid
				operator bool() const { return this->valid(); }
				
				//! Check if the Iterator is valid
				bool valid() const { return this->rawIndex >= 0; }
		};
		
		class wstring_const_reverse_iterator : public wstring_reverse_iterator
		{
			public:
				
				//! Ctor
				wstring_const_reverse_iterator( _u32 rawIndex , const wstring& instance ) :
					wstring_reverse_iterator( rawIndex , const_cast<wstring&>(instance) )
				{}
				
				//! Cast ctor
				wstring_const_reverse_iterator& operator=( const wstring_reverse_iterator& it ){ this->setIndex( it.getIndex() ); this->instance = it.getInstance(); return *this; }
				wstring_const_reverse_iterator( const wstring_reverse_iterator& it ) : wstring_reverse_iterator( it.getIndex() , it.getInstance() ) { }
				
				//! Default functions
				wstring_const_reverse_iterator( const wstring_const_reverse_iterator& ) = default;
				wstring_const_reverse_iterator& operator=( const wstring_const_reverse_iterator& it ){ this->rawIndex = it.rawIndex; return *this; }
				
				//! Remove setter
				void setValue( _wchar value ) = delete;
				
				//! Returns the value behind the iterator
				_wchar operator*(){ return this->instance.rawAt( this->rawIndex ); }
		};
		
		//! Attributes
		_char*			buffer;
		_u32			bufferLen;
		_u32			stringLen;
		_u32*			indicesOfMultibyte;
		_u32			indicesLen;
		mutable bool	misFormated;
		
		//! Frees the internal buffer of indices and sets it to the supplied value
		void resetIndices( _u32* indicesOfMultibyte = nullptr , _u32 indicesLen = 0 ){
			if( this->indicesOfMultibyte )
				delete[] this->indicesOfMultibyte;
			this->indicesLen = indicesLen;
			this->indicesOfMultibyte = indicesOfMultibyte;
		}
		
		//! Frees the internal buffer of indices
		void resetBuffer( _char* buffer = nullptr , _u32 bufferLen = 0 ){
			if( this->buffer )
				delete[] this->buffer;
			this->bufferLen = bufferLen;
			this->buffer = buffer;
		}
		
		/**
		 * Returns the number of bytes to expect behind this one (including this one) that belong to this utf8 char
		 */
		static _u8 getNumBytesOfUTF8Char( _u8 firstByte )
		{
			if( (firstByte & 0xE0) == 0xC0 )  // 110XXXXX  two bytes
				return 2;
			else if( (firstByte & 0xF0) == 0xE0 )  // 1110XXXX  three bytes
				return 3;
			else if( (firstByte & 0xF8) == 0xF0 )  // 11110XXX  four bytes
				return 4;
			else if( (firstByte & 0xFC) == 0xF8 )  // 111110XX  five bytes
				return 5;
			else if( (firstByte & 0xFE) == 0xFC )  // 1111110X  six bytes
				return 6;
			return 1; // one byte
		}
		
		/**
		 * Returns the number of bytes to expect before this one (including this one) that belong to this utf8 char
		 */
		static _u8 getNumBytesOfUTF8CharBefore( _literal data )
		{
			if( (data[-2] & 0xE0) == 0xC0 )  // 110XXXXX  two bytes
				return 2;
			else if( (data[-3] & 0xF0) == 0xE0 )  // 1110XXXX  three bytes
				return 3;
			else if( (data[-4] & 0xF8) == 0xF0 )  // 11110XXX  four bytes
				return 4;
			else if( (data[-5] & 0xFC) == 0xF8 )  // 111110XX  five bytes
				return 5;
			else if( (data[-6] & 0xFE) == 0xFC )  // 1111110X  six bytes
				return 6;
			return 1; // one byte
		}
		
		/**
		 * Returns the number of bytes to expect behind this one that belong to this utf8 char
		 */
		static _u8 getNumBytesOfUTF8Codepoint( _wchar codepoint )
		{
			if( codepoint <= 0x7F )
				return 1;
			else if( codepoint <= 0x7FF )
				return 2;
			else if( codepoint <= 0xFFFF )
				return 3;
			else if( codepoint <= 0x1FFFFF )
				return 4;
			else if( codepoint <= 0x3FFFFFF )
				return 5;
			else if( codepoint <= 0x7FFFFFFF )
				return 6;
			return 0;
		}
		
		//! Returns the actual byte index of the supplied codepoint index
		_u32 getActualIndex( _u32 codepointIndex ) const ;
		
		/**
		 * Get the byte index of the last codepoint
		 */
		_u32 backIndex() const { return size() - getIndexPreBytes( size() ); }
		/**
		 * Get the byte index of the index behind the last codepoint
		 */
		_u32 endIndex() const { return size(); }
		
		/**
		 * Decodes a given input of rle utf8 data to a
		 * unicode codepoint and returns the number of bytes it used
		 */
		static _u8 decodeUTF8( _literal data , _wchar& codepoint , bool& hasError );
		
		/**
		 * Encodes a given codepoint to a character buffer of at least 7 bytes
		 */
		static _u8 encodeUTF8( _wchar codepoint , _char* dest , bool& hasError );
		
		/**
		 * Counts the number of bytes required
		 * to hold the given wide character string.
		 * numBytes is set to the number of multibyte
		 * codepoints (ones that consume mor than 1 byte).
		 */
		static _u32 getNumberOfRequiredBytes( _wliteral lit , _u32& numMultibytes );
		
		/**
		 * Counts the number of codepoints
		 * that are built up of the supplied amout of bytes
		 */
		_u32 getNumberOfResultingCodepoints( _u32 byteStart , _u32 byteCount ) const ;
		
		/**
		 * Counts the number of bytes
		 * required to hold the supplied amount of codepoints starting at the supplied byte index
		 */
		_u32 getNumberOfResultingBytes( _u32 byteStart , _u32 codepointCount ) const ;
		
		//! Ctor from buffer and indices
		wstring( _char* buffer , _wchar bufferLen , _u32 stringLen , _u32* indicesOfMultibyte , _u32 indicesLen ) :
			buffer( buffer )
			, bufferLen( bufferLen )
			, stringLen( stringLen )
			, indicesOfMultibyte( indicesOfMultibyte )
			, indicesLen( indicesLen )
			, misFormated( false )
		{}
		
	public:
		
		typedef _u32									size_type;
		typedef _wchar									value_type;
		typedef wstring::wstring_iterator				iterator;
		typedef wstring::wstring_const_iterator			const_iterator;
		typedef wstring::wstring_reverse_iterator		reverse_iterator;
		typedef wstring::wstring_const_reverse_iterator	const_reverse_iterator;
		constexpr static _u32							npos = -1;
		
		
		/**
		 * Default Ctor
		 * 
		 * @note Creates an Instance of type wstring that is empty
		 */
		wstring() :
			buffer( new _char[1]{0} )
			, bufferLen( 1 )
			, stringLen( 0 )
			, indicesOfMultibyte( nullptr )
			, indicesLen( 0 )
			, misFormated( false )
		{}
		
		/**
		 * Contructor taking an utf8 sequence and the maximum length to read from it (in number of codepoints)
		 * 
		 * @note	Creates an Instance of type wstring that holds the given utf8 sequence
		 * @param	str		The UTF-8 sequence to fill the wstring with
		 * @param	len		(Optional) The maximum number of codepoints to read from the sequence
		 */
		wstring( _literal str , _u32 len = wstring::npos );
		
		
		/**
		 * Contructor taking an std::string
		 * 
		 * @note	Creates an Instance of type wstring that holds the given data sequence
		 * @param	str		The byte data, that will be interpreted as UTF-8
		 */
		wstring( string str ) :
			wstring( str.c_str() )
		{}
		
		
		/**
		 * Contructor that fills itself with a certain amount of codepoints
		 * 
		 * @note	Creates an Instance of type wstring that gets filled with 'n' codepoints
		 * @param	n		The number of codepoints generated
		 * @param	ch		The codepoint that the whole buffer will be set to
		 */
		wstring( _u32 n , _wchar ch );
		/**
		 * Contructor that fills itself with a certain amount of characters
		 * 
		 * @note	Creates an Instance of type wstring that gets filled with 'n' characters
		 * @param	n		The number of characters generated
		 * @param	ch		The characters that the whole buffer will be set to
		 */
		wstring( _u32 n , _char ch ) :
			wstring( n , (_wchar)ch )
		{}
		
		
		/**
		 * Copy Constructor that copies the supplied wstring to construct itself
		 * 
		 * @note	Creates an Instance of type wstring that has the exact same data as 'str'
		 * @param	str		The wstring to copy from
		 */
		wstring( const wstring& str );
		/**
		 * Move Constructor that moves the supplied wstring content into the new wstring
		 * 
		 * @note	Creates an Instance of type wstring that takes all data from 'str'
		 * 			The supplied wstring is invalid afterwards and may not be used anymore
		 * @param	str		The wstring to move from
		 */
		wstring( wstring&& str );
		
		
		/**
		 * Contructor taking a wide codepoint literal that will be copied to construct this wstring
		 * 
		 * @note	Creates an Instance of type wstring that holds the given codepoints
					The data itself will be first converted to UTF-8
		 * @param	str		The codepoint sequence to fill the wstring with
		 */
		wstring( _wliteral str );
		
		
		/**
		 * Destructor
		 * 
		 * @note	Destructs a wstring at the end of its lifetime releasing all held memory
		 */
		~wstring(){
			this->resetIndices();
			this->resetBuffer();
		}
		
		
		/**
		 * Copy Assignment operator that sets the wstring to a copy of the supplied one
		 * 
		 * @note	Assigns a copy of 'str' to this wstring deleting all data that previously was in there
		 * @param	str		The wstring to copy from
		 * @return	A reference to the string now holding the data (*this)
		 */
		wstring& operator=( const wstring& str );
		/**
		 * Move Assignment operator that moves all data out of the supplied and into this wstring
		 * 
		 * @note	Moves all data from 'str' into this wstring deleting all data that previously was in there
		 *			The supplied wstring is invalid afterwards and may not be used anymore
		 * @param	str		The wstring to move from
		 * @return	A reference to the string now holding the data (*this)
		 */
		wstring& operator=( wstring&& str );
		
		
		/**
		 * Swaps the contents of this wstring with the supplied one
		 * 
		 * @note	Swaps all data with the supplied wstring
		 * @param	str		The wstring to swap contents with
		 */
		void swap( wstring& str ){
			std::swap( *this , str );
		}
		
		
		/**
		 * Clears the content of this wstring
		 * 
		 * @note	Resets the data to an empty string ("")
		 */
		void clear(){
			this->stringLen = 0;
			this->misFormated = false;
			resetBuffer( new _char[1]{0} , 1 );
			resetIndices();
		}
		
		
		/**
		 * Returns whether there is an UTF-8 error in the string
		 * 
		 * @return	True, if there is an encoding error, false, if the contained data is properly encoded
		 */
		bool isMisFormated() const { return this->misFormated; }
		
		
		/**
		 * Returns the codepoint at the supplied index
		 * 
		 * @param	n	The codepoint index of the codepoint to receive
		 * @return	The Codepoint at position 'n'
		 */
		_wchar at( _u32 n ) const ;
		/**
		 * Returns the codepoint at the supplied byte position
		 * 
		 * @note	As this access is raw, that is not looking up for the actual byte position,
		 *			it is very fast
		 * @param	byteIndex	The byte position of the codepoint to receive
		 * @return	The codepoint at the supplied position
		 */
		_wchar rawAt( _u32 byteIndex ) const {
			if( !this->requiresUnicode() )
				return this->buffer[byteIndex];
			_wchar dest;
			decodeUTF8( this->buffer + byteIndex , dest , this->misFormated );
			return dest;
		}
		
		
		/**
		 * Returns an iterator pointing to the supplied codepoint index
		 * 
		 * @param	n	The index of the codepoint to get the iterator to
		 * @return	An iterator pointing to the specified codepoint index
		 */
		iterator get( _u32 n ){ return iterator( getActualIndex(n) , *this ); }
		const_iterator get( _u32 n ) const { return const_iterator( getActualIndex(n) , *this ); }
		/**
		 * Returns an iterator pointing to the codepoint at the supplied byte position
		 * 
		 * @note	As this access is raw, that is not looking up for the actual byte position,
		 *			it is very fast
		 * @param	n	The byte position of the codepoint to get the iterator to
		 * @return	An iterator pointing to the specified byte position
		 */
		iterator rawGet( _u32 n ){ return iterator( n , *this ); }
		const_iterator rawGet( _u32 n ) const { return const_iterator( n , *this ); }
		
		
		/**
		 * Returns a reverse iterator pointing to the supplied codepoint index
		 * 
		 * @param	n	The index of the codepoint to get the reverse iterator to
		 * @return	A reverse iterator pointing to the specified codepoint index
		 */
		reverse_iterator rget( _u32 n ){ return reverse_iterator( getActualIndex(n) , *this ); }
		const_reverse_iterator rget( _u32 n ) const { return const_reverse_iterator( getActualIndex(n) , *this ); }
		/**
		 * Returns a reverse iterator pointing to the codepoint at the supplied byte position
		 * 
		 * @note	As this access is raw, that is not looking up for the actual byte position,
		 *			it is very fast
		 * @param	n	The byte position of the codepoint to get the reverse iterator to
		 * @return	A reverse iterator pointing to the specified byte position
		 */
		reverse_iterator rawRget( _u32 n ){ return reverse_iterator( n , *this ); }
		const_reverse_iterator rawRget( _u32 n ) const { return const_reverse_iterator( n , *this ); }
		
		
		/**
		 * Returns a reference to the codepoint at the supplied index
		 * 
		 * @param	n	The codepoint index of the codepoint to receive
		 * @return	A reference wrapper to the codepoint at position 'n'
		 */
		wstring_reference operator[]( _u32 n ){ return wstring_reference( n , *this ); }
		_wchar operator[]( _u32 n ) const { return at( n ); }
		/**
		 * Returns a reference to the codepoint at the supplied byte position
		 * 
		 * @note	As this access is raw, that is not looking up for the actual byte position,
		 *			it is very fast
		 * @param	n	The byte position of the codepoint to receive
		 * @return	A reference wrapper to the codepoint at byte position 'n'
		 */
		wstring_raw_reference operator()( _u32 n ){ return wstring_raw_reference( n , *this ); }
		_wchar operator()( _u32 n ) const { return rawAt( n ); }
		
		
		/**
		 * Get the raw data contained in this wstring
		 * 
		 * @note	Returns the UTF-8 formatted content of this wstring
		 * @return	UTF-8 formatted data, trailled by a '\0'
		 */
		_literal c_str() const { return this->buffer; }
		_literal data() const { return this->buffer; }
		
		
		/**
		 * Get the raw data contained in this wstring wrapped by an std::string
		 * 
		 * @note	Returns the UTF-8 formatted content of this wstring
		 * @return	UTF-8 formatted data, wrapped inside an std::string
		 */
		string cpp_str() const { return string( this->buffer ); }
		
		
		/**
		 * Get the number of codepoints in this wstring
		 * 
		 * @note	Returns the number of codepoints that are taken care of
		 *			For the number of bytes, @see size()
		 * @return	Number of codepoints (not bytes!)
		 */
		_u32 length() const { return this->stringLen; }
		
		
		/**
		 * Get the number of bytes that are used by this wstring
		 * 
		 * @note	Returns the number of bytes required to hold the contained wide string,
		 *			That is, without counting the trailling '\0'
		 * @return	Number of bytes (not codepoints!)
		 */
		_u32 size() const { return this->bufferLen - 1; }
		
		
		/**
		 * Check, whether this wstring is empty
		 * 
		 * @note	Returns True, if this wstring is empty, that also is comparing true with ""
		 * @return	True, if this wstring is empty, false if its length is >0
		 */
		bool empty() const { return !this->buffer[0]; }
		
		
		/**
		 * Check whether the data inside this wstring cannot be iterated by an std::string
		 * 
		 * @note	Returns true, if the wstring has codepoints that exceed 7 bits to be stored
		 * @return	True, if there are UTF-8 formatted byte sequences,
		 *			false, if there are only ANSI characters inside
		 */
		bool requiresUnicode() const { return this->indicesLen > 0; }
		
		
		/**
		 * Get a wide string literal representing this UTF-8 string
		 * 
		 * @note	As the underlying UTF-8 data has to be converted to a wide character array and 
		 *			as this array has to be allocated, this method is quite slow, that is O(n)
		 * @return	A wrapper class holding the wide character array
		 */
		_uniquePtr<_wchar[]> toWideLiteral() const ;
		
		
		/**
		 * Get an iterator to the beginning of the wstring
		 * 
		 * @return	An iterator class pointing to the beginning of this wstring
		 */
		iterator begin(){ return iterator( 0 , *this ); }
		const_iterator begin() const { return const_iterator( 0 , *this ); }
		/**
		 * Get an iterator to the end of the wstring
		 * 
		 * @return	An iterator class pointing to the end of this wstring, that is pointing behind the last codepoint
		 */
		iterator end(){ return iterator( endIndex() , *this ); }
		const_iterator end() const { return const_iterator( endIndex() , *this ); }
		
		
		/**
		 * Get a reverse iterator to the end of this wstring
		 * 
		 * @return	A reverse iterator class pointing to the end of this wstring,
		 *			that is exactly to the last codepoint
		 */
		reverse_iterator rbegin(){ return reverse_iterator( backIndex() , *this ); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator( backIndex() , *this ); }
		/**
		 * Get a reverse iterator to the beginning of this wstring
		 * 
		 * @return	A reverse iterator class pointing to the end of this wstring,
		 *			that is pointing before the first codepoint
		 */
		reverse_iterator rend(){ return reverse_iterator( -1 , *this ); }
		const_reverse_iterator rend() const { return const_reverse_iterator( -1 , *this ); }
		
		
		/**
		 * Get a const iterator to the beginning of the wstring
		 * 
		 * @return	A const iterator class pointing to the beginning of this wstring,
		 * 			which cannot alter things inside this wstring
		 */
		wstring_const_iterator cbegin() const { return wstring_const_iterator( 0 , *this ); }
		/**
		 * Get an iterator to the end of the wstring
		 * 
		 * @return	A const iterator class, which cannot alter this wstring, pointing to
		 *			the end of this wstring, that is pointing behind the last codepoint
		 */
		wstring_const_iterator cend() const { return wstring_const_iterator( endIndex() , *this ); }
		
		
		/**
		 * Get a const reverse iterator to the end of this wstring
		 * 
		 * @return	A const reverse iterator class, which cannot alter this wstring, pointing to
		 *			the end of this wstring, that is exactly to the last codepoint
		 */
		wstring_const_reverse_iterator crbegin() const { return wstring_const_reverse_iterator( backIndex() , *this ); }
		/**
		 * Get a const reverse iterator to the beginning of this wstring
		 * 
		 * @return	A const reverse iterator class, which cannot alter this wstring, pointing to
		 *			the end of this wstring, that is pointing before the first codepoint
		 */
		wstring_const_reverse_iterator crend() const { return wstring_const_reverse_iterator( -1 , *this ); }
		
		
		/**
		 * Returns a reference to the first codepoint in the wstring
		 * 
		 * @return	A reference wrapper to the first codepoint in the wstring
		 */
		wstring_raw_reference front(){ return wstring_raw_reference( 0 , *this ); }
		_wchar front() const { return rawAt( 0 ); }
		/**
		 * Returns a reference to the last codepoint in the wstring
		 * 
		 * @return	A reference wrapper to the last codepoint in the wstring
		 */
		wstring_raw_reference back(){ return wstring_raw_reference( empty() ? 0 : backIndex() , *this ); }
		_wchar back() const { return rawAt( empty() ? 0 : backIndex() ); }
		
		
		/**
		 * Replace a codepoint of this wstring by a number of codepoints
		 * 
		 * @param	index	The codpoint index to be replaced
		 * @param	repl	The wide character that will be used to replace the codepoint
		 * @param	n		The number of codepoint that will be inserted
		 *					instead of the one residing at position 'index'
		 * @return	A reference to this wstring, which now has the replaced part in it
		 */
		wstring& replace( _u32 index , _wchar repl , _u32 n = 1 ){
			replace( index , 1 , wstring( n , repl ) );
			return *this;
		}
		/**
		 * Replace a number of codepoints of this wstring by a number of other codepoints
		 * 
		 * @param	index	The codpoint index at which the replacement is being started
		 * @param	len		The number of codepoints that are being replaced
		 * @param	repl	The wide character that will be used to replace the codepoints
		 * @param	n		The number of codepoint that will replace the old ones
		 * @return	A reference to this wstring, which now has the replaced part in it
		 */
		wstring& replace( _u32 index , _u32 len , _wchar repl , _u32 n = 1 ){
			replace( index , len , wstring( n , repl ) );
			return *this;
		}
		/**
		 * Replace a range of codepoints by a number of codepoints
		 * 
		 * @param	first	An iterator pointing to the first codepoint to be replaced
		 * @param	last	An iterator pointing to the codepoint behind the last codepoint to be replaced
		 * @param	repl	The wide character that will be used to replace the codepoints in the range
		 * @param	n		The number of codepoint that will replace the old ones
		 * @return	A reference to this wstring, which now has the replaced part in it
		 */
		wstring& replace( wstring_iterator first , wstring_iterator last , _wchar repl , _u32 n = 1 ){
			rawReplace( first.getIndex() , last.getIndex() - first.getIndex() , wstring( n , repl ) );
			return *this;
		}
		/**
		 * Replace a range of codepoints with the contents of the supplied wstring
		 * 
		 * @param	first	An iterator pointing to the first codepoint to be replaced
		 * @param	last	An iterator pointing to the codepoint behind the last codepoint to be replaced
		 * @param	repl	The wstring to replace all codepoints in the range
		 * @return	A reference to this wstring, which now has the replaced part in it
		 */
		wstring& replace( wstring_iterator first , wstring_iterator last , const wstring& repl ){
			rawReplace( first.getIndex() , last.getIndex() - first.getIndex() , repl );
			return *this;
		}
		/**
		 * Replace a number of codepoints of this wstring with the contents of the supplied wstring
		 * 
		 * @param	index	The codpoint index at which the replacement is being started
		 * @param	len		The number of codepoints that are being replaced
		 * @param	repl	The wstring to replace all codepoints in the range
		 * @return	A reference to this wstring, which now has the replaced part in it
		 */
		wstring& replace( _u32 index , _u32 count , const wstring& repl )
		{
			_s32		actualStartIndex = getActualIndex( index );
			
			if( count == wstring::npos )
				rawReplace( actualStartIndex , wstring::npos , repl );
			else
			{
				_s32	actualEndIndex = count ? getActualIndex( index + count ) : actualStartIndex;
				rawReplace( actualStartIndex , actualEndIndex - actualStartIndex , repl );
			}
			
			return *this;
		}
		/**
		 * Replace a number of bytes of this wstring with the contents of the supplied wstring
		 * 
		 * @note	As this function is raw, that is not having to compute byte indices,
		 *			it is much faster than the codepoint-base replace function
		 * @param	byteIndex	The byte position at which the replacement is being started
		 * @param	byteCount	The number of bytes that are being replaced
		 * @param	repl		The wstring to replace all bytes inside the range
		 * @return	A reference to this wstring, which now has the replaced part in it
		 */
		void rawReplace( _u32 byteIndex , _u32 byteCount , const wstring& repl );
		
		
		/**
		 * Appends the supplied wstring to the end of this wstring
		 * 
		 * @param	appendix	The wstring to be appended
		 * @return	A reference to this wstring, which now has the supplied string appended
		 */
		wstring& append( const wstring& appendix ){
			replace( length() , 0 , appendix );
			return *this;
		}
		wstring& operator+=( const wstring& summand ){
			replace( length() , 0 , summand );
			return *this;
		}
		
		
		/**
		 * Appends the supplied codepoint to the end of this wstring
		 * 
		 * @param	ch	The codepoint to be appended
		 * @return	A reference to this wstring, which now has the supplied string appended
		 */
		wstring& push_back( _wchar ch ){
			replace( length() , 0 , wstring( 1 , ch ) );
			return *this;
		}
		
		
		/**
		 * Adds the supplied wstring to a copy of this wstring
		 * 
		 * @param	summand		The wstring to be added 
		 * @return	A reference to the newly created wstring, which now has the supplied string appended
		 */
		wstring operator+( const wstring& summand ) const {
			wstring str = *this;
			str.append( summand );
			return std::move( str );
		}
		
		
		/**
		 * Inserts a given codepoint into this wstring at the supplied codepoint index
		 * 
		 * @param	pos		The codepoint index to insert at
		 * @param	ch		The codepoint to be inserted
		 * @return	A reference to this wstring, with the supplied codepoint inserted
		 */
		wstring& insert( _u32 pos , _wchar ch ){
			replace( pos , 0 , wstring( 1 , ch ) );
			return *this;
		}
		/**
		 * Inserts a given wstring into this wstring at the supplied codepoint index
		 * 
		 * @param	pos		The codepoint index to insert at
		 * @param	str		The wstring to be inserted
		 * @return	A reference to this wstring, with the supplied wstring inserted
		 */
		wstring& insert( _u32 pos , const wstring& str ){
			replace( pos , 0 , str );
			return *this;
		}
		/**
		 * Inserts a given wstring into this wstring at the supplied iterator position
		 * 
		 * @param	it	The iterator psoition to insert at
		 * @param	ch	The codepoint to be inserted
		 * @return	A reference to this wstring, with the supplied codepoint inserted
		 */
		wstring& insert( wstring_iterator it , _wchar ch ){
			rawReplace( it.getIndex() , 0 , wstring( 1 , ch ) );
			return *this;
		}
		/**
		 * Inserts a given wstring into this wstring at the supplied byte position
		 * 
		 * @note	As this function is raw, that is without having to compute
		 *			actual byte indices, it is much faster that insert()
		 * @param	pos		The byte position index to insert at
		 * @param	str		The wstring to be inserted
		 * @return	A reference to this wstring, with the supplied wstring inserted
		 */
		wstring& rawInsert( _u32 pos , wstring& str ){
			rawReplace( pos , 0 , str );
			return *this;
		}
		
		
		/**
		 * Erases the codepoint at the supplied iterator position
		 * 
		 * @param	pos		The iterator pointing to the position being erased
		 * @return	A reference to this wstring, which now has the codepoint erased
		 */
		wstring& erase( wstring_iterator pos ){
			rawReplace( pos.getIndex() , getIndexBytes( pos.getIndex() )  , wstring() );
			return *this;
		}
		/**
		 * Erases the codepoints inside the supplied range
		 * 
		 * @param	first	An iterator pointing to the first codepoint to be erased
		 * @param	last	An iterator pointing to the codepoint behind the last codepoint to be erased
		 * @return	A reference to this wstring, which now has the codepoints erased
		 */
		wstring& erase( wstring_iterator first , wstring_iterator last ){
			rawReplace( first.getIndex() , last.getIndex() - first.getIndex() , wstring() );
			return *this;
		}
		/**
		 * Erases a portion of this string
		 * 
		 * @param	pos		The codepoint index to start eraseing from
		 * @param	len		The number of codepoints to be erased from this wstring
		 * @return	A reference to this wstring, with the supplied portion erased
		 */
		wstring& erase( _u32 pos , _u32 len ){
			replace( pos , len , wstring() );
			return *this;
		}
		/**
		 * Erases a byte range of this string
		 * 
		 * @note	As this function is raw, that is without having to compute
		 *			actual byte indices, it is much faster that erase()
		 * @param	pos		The byte position index to start erasing from
		 * @param	len		The number of bytes to be erased from the wstring
		 * @return	A reference to this wstring, with the supplied bytes erased
		 */
		wstring& rawErase( _u32 pos , _u32 len ){
			rawReplace( pos , len , wstring() );
			return *this;
		}
		
		
		/**
		 * Returns a portion of the wstring
		 * 
		 * @param	first	An iterator pointing to the first codepoint to be included in the substring
		 * @param	last	An iterator pointing to the codepoint behind the last codepoint in the substring
		 * @return	The wstring holding the specified range
		 */
		wstring substr( wstring_iterator first , wstring_iterator last ) const {
			_u32 byteCount = last.getIndex() - first.getIndex();
			return rawSubstr( first.getIndex() , byteCount , getNumberOfResultingCodepoints( first.getIndex() , byteCount ) );
		}
		/**
		 * Returns a portion of the wstring
		 * 
		 * @param	pos		The codepoint index that should mark the start of the substring
		 * @param	len		The number codepoints to be included within the substring
		 * @return	The wstring holding the specified codepoints
		 */
		wstring substr( _u32 pos , _u32 len ) const
		{
			_s32	actualStartIndex = getActualIndex( pos );
			
			if( len == wstring::npos )
				return rawSubstr( actualStartIndex , wstring::npos );
			
			_s32	actualEndIndex = len ? getActualIndex( pos + len ) : actualStartIndex;
			
			return rawSubstr( actualStartIndex , actualEndIndex - actualStartIndex , len );
		}
		/**
		 * Returns a portion of the wstring (indexed on byte-base)
		 * 
		 * @note	As this function is raw, that is without having to compute
		 *			actual byte indices, it is much faster that substr()
		 * @param	byteIndex		The byte position where the substring shall start
		 * @param	byteCount		The number of bytes that the substring shall have
		 * @param	numCodepoints	(Optional) The number of codepoints
		 *							the substring will have, in case this is already known
		 * @return	The wstring holding the specified bytes
		 */
		wstring rawSubstr( _u32 byteIndex , _u32 byteCount , _u32 numCodepoints ) const ;
		wstring rawSubstr( _u32 byteIndex , _u32 byteCount ) const {
			if( byteCount == wstring::npos )
				byteCount = size() - byteIndex;
			return rawSubstr( byteIndex , byteCount , getNumberOfResultingCodepoints( byteIndex , byteCount ) );
		}
		
		
		/**
		 * Finds a specific codepoint inside the wstring starting at the supplied codepoint index
		 * 
		 * @param	ch				The codepoint to look for
		 * @param	startCodepoint	The index of the first codepoint to start looking from
		 * @return	The codepoint index where and if the codepoint was found or wstring::npos
		 */
		_u32 find( _wchar ch , _u32 startCodepoint = 0 ) const ;
		/**
		 * Finds a specific codepoint inside the wstring starting at the supplied byte position
		 * 
		 * @param	ch			The codepoint to look for
		 * @param	startByte	The byte position of the first codepoint to start looking from
		 * @return	The byte position where and if the codepoint was found or wstring::npos
		 */
		_u32 rawFind( _wchar ch , _u32 startByte = 0 ) const ;
		
		//! Find content in string
		//! @todo implement
		//_u32 rfind( wstring str , _u32 startCodepoint = wstring::npos ) const ;
		//_u32 rawRfind( wstring str , _u32 startByte = wstring::npos ) const ;
		//_u32 rfind( _literal str , _u32 startCodepoint = wstring::npos ) const ;
		//_u32 rawRfind( _literal str , _u32 startByte = wstring::npos ) const ;
		_u32 rfind( _wchar ch , _u32 startCodepoint = wstring::npos ) const ;
		_u32 rawRfind( _wchar ch , _u32 startByte = wstring::npos ) const ;
		
		//! Find character in string
		_u32 find_first_of( _wliteral str , _u32 startCodepoint = 0 ) const ;
		_u32 rawFind_first_of( _wliteral str , _u32 startByte = 0 ) const ;
		_u32 find_last_of( _wliteral str , _u32 startCodepoint = wstring::npos ) const ;
		_u32 rawFind_last_of( _wliteral str , _u32 startByte = wstring::npos ) const ;
		
		//! Find absence of character in string
		_u32 find_first_not_of( _wliteral str , _u32 startCodepoint = 0 ) const ;
		_u32 rawFind_first_not_of( _wliteral str , _u32 startByte = 0 ) const ;
		_u32 find_last_not_of( _wliteral str , _u32 startCodepoint = wstring::npos ) const ;
		_u32 rawFind_last_not_of( _wliteral str , _u32 startByte = wstring::npos ) const ;
		
		
		//! Removes the last codepoint in the wstring
		wstring& pop_back(){
			_u32 pos = backIndex();
			rawReplace( pos , getIndexBytes( pos ) , wstring() );
			return *this;
		}
		
		/**
		 * Compares this wstring to the supplied one
		 *
		 * @return	0	They compare equal
		 *			<0	Either the value of the first character that does not match is lower in
		 *			the compared string, or all compared characters match but the compared string is shorter.
		 *			>0	Either the value of the first character that does not match is greater in
		 *			the compared string, or all compared characters match but the compared string is longer.
		 */
		_s32 compare( const wstring& str ) const ;
		
		//! Returns true, if the supplied string compares equal to this one
		bool equals( const wstring& str ) const { return equals( str.c_str() ); }
		bool equals( const string& str ) const { return equals( str.c_str() ); }
		bool equals( _literal str ) const ;
		bool equals( _wliteral str ) const ;
		
		//! Compare this wstring to another string
		bool operator==( const wstring& str ) const { return equals( str ); }
		bool operator!=( const wstring& str ) const { return !equals( str ); }
		bool operator==( _literal str ) const { return equals( str ); }
		bool operator!=( _literal str ) const { return !equals( str ); }
		bool operator==( _wliteral str ) const { return equals( str ); }
		bool operator!=( _wliteral str ) const { return !equals( str ); }
		bool operator==( const string& str ) const { return equals( str ); }
		bool operator!=( const string& str ) const { return !equals( str ); }
		
		bool operator>( const wstring& str ) const { return str.compare( *this ) > 0; }
		bool operator>=( const wstring& str ) const { return str.compare( *this ) >= 0; }
		bool operator<( const wstring& str ) const { return str.compare( *this ) < 0; }
		bool operator<=( const wstring& str ) const { return str.compare( *this ) <= 0; }
		
		
		//! Get the number of bytes of codepoint in wstring
		_u8 getCodepointBytes( _u32 codepointIndex ) const {
			return getNumBytesOfUTF8Char( this->buffer[ getActualIndex(codepointIndex) ] );
		}
		_u8 getIndexBytes( _u32 byteIndex ) const {
			return getNumBytesOfUTF8Char( this->buffer[byteIndex] );
		}
		
		
		//! Get the number of bytes before a codepoint, that build up a new codepoint
		_u8 getCodepointPreBytes( _u32 codepointIndex ) const {
			return codepointIndex > 0 ? getNumBytesOfUTF8CharBefore( this->buffer + getActualIndex(codepointIndex) ) : 1;
		}
		_u8 getIndexPreBytes( _u32 byteIndex ) const {
			return byteIndex > 0 ? getNumBytesOfUTF8CharBefore( this->buffer + byteIndex ) : 1;
		}
		
		
		
		//! Friend iterator difference computation functions
		friend int				operator-( const iterator& left , const iterator& right );
		friend int				operator-( const reverse_iterator& left , const reverse_iterator& right );
		
		//! Friend summation functions
		friend iterator			operator+( const iterator& it , _u32 nth );
		friend reverse_iterator	operator+( const reverse_iterator& it , _u32 nth );
};

extern int							operator-( const wstring::iterator& left , const wstring::iterator& right );
extern int							operator-( const wstring::reverse_iterator& left , const wstring::reverse_iterator& right );
extern wstring::iterator			operator+( const wstring::iterator& it , _u32 nth );
extern wstring::reverse_iterator	operator+( const wstring::reverse_iterator& it , _u32 nth );

#endif