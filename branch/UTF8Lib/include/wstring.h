#include "type.h"
#include "type.uniqueptr.h"

#define multibytePartGuess 10 /* A tenth = 10% */

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
					instance.replace( index , 1 , wstring( 1, ch ) );
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
					this->instance.rawReplace( this->rawIndex , this->instance.getIndexBytes( this->rawIndex ) , wstring( 1, ch ) );
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
				wstring_iterator operator+( _u32 nth ){
					_u32 newIndex = this->rawIndex;
					while( nth-- > 0 )
						newIndex += this->instance.getIndexBytes( newIndex );
					return wstring_iterator( newIndex , this->instance );
				}
				wstring_iterator& operator+=( _u32 nth ){
					while( nth-- > 0 )
						this->rawIndex += this->instance.getIndexBytes( this->rawIndex );
					return *this;
				}
				
				//! Convert the iterator to an int
				operator int() const { return this->instance.getNumberOfResultingCodepoints( 0 , rawIndex ); }
				
				//! Get the difference between two iterators
				int operator-( const wstring_iterator& it ){
					_u32 minIndex = std::min( this->rawIndex , it.rawIndex );
					_u32 maxIndex = std::max( this->rawIndex , it.rawIndex );
					_u32 numCodepoints = this->instance.getNumberOfResultingCodepoints( minIndex , maxIndex - minIndex );
					return maxIndex == this->rawIndex ? numCodepoints : -numCodepoints;
				}
				
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
				
				//! Get the index
				_s32 getIndex() const {
					return this->rawIndex;
				}
				
				//! Set the index
				void setIndex( _s32 rawIndex ){
					this->rawIndex = rawIndex;
				}
				
				wstring_reverse_iterator& operator++(){ // prefix iter++
					this->rawIndex -= this->instance.getIndexPreBytes( this->rawIndex );
					return *this;
				}
				wstring_reverse_iterator& operator++( int ){ // postfix iter++
					this->rawIndex -= this->instance.getIndexPreBytes( this->rawIndex );
					return *this;
				}
				wstring_reverse_iterator operator+( _u32 nth ){
					_s32 newIndex = this->rawIndex;
					while( nth-- > 0 )
						newIndex -= this->instance.getIndexPreBytes( newIndex );
					return wstring_reverse_iterator( newIndex , this->instance );
				}
				wstring_reverse_iterator& operator+=( _u32 nth ){
					while( nth-- > 0 )
						this->rawIndex -= this->instance.getIndexPreBytes( this->rawIndex );
					return *this;
				}
				
				//! Get the difference between two iterators
				int operator-( const wstring_reverse_iterator& it ) const {
					_s32 minIndex = std::min( this->rawIndex , it.rawIndex );
					_s32 maxIndex = std::max( this->rawIndex , it.rawIndex );
					_u32 numCodepoints = this->instance.getNumberOfResultingCodepoints( minIndex , maxIndex - minIndex );
					return maxIndex == it.rawIndex ? numCodepoints : -numCodepoints;
				}
				
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
				
				//! Convert the iterator to an int
				operator int() const { return this->instance.length() - this->instance.getNumberOfResultingCodepoints( 0 , rawIndex ); }
				
				//! From wstring_iterator to wstring_reverse_iterator
				wstring_reverse_iterator& operator=( const wstring_iterator& it ){ this->rawIndex = it.getIndex(); this->instance = it.getInstance(); return *this; }
				wstring_reverse_iterator& operator=( const wstring_reverse_iterator& it ){ this->rawIndex = it.rawIndex; this->instance = it.instance; return *this; }
				wstring_reverse_iterator( const wstring_iterator& it ) : rawIndex( it.getIndex() ) , instance( it.getInstance() ) { }
				wstring_reverse_iterator( const wstring_reverse_iterator& it ) : rawIndex( it.rawIndex ) , instance( it.instance ) { }
				
				//! Ctor
				wstring_reverse_iterator( _s32 rawIndex , wstring& instance ) :
					rawIndex( rawIndex )
					, instance( instance )
				{}
				
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
				
				//! Remove setter
				void setValue( _wchar value ) = delete;
				
				//! Returns the value behind the iterator
				_wchar operator*(){ return this->instance.rawAt( this->rawIndex ); }
		};
		
		//! Attributes
		_char*			buffer;
		_u32			bufferLen;
		_u32			stringLen;
		_s32*			indicesOfMultibyte;
		_u32			indicesLen;
		mutable bool	misFormated;
		
		//! Resizes the internal array 'indicesOfMultibyte' to the appropriate size (and copies all indices)
		void resizeIndices( _u32 indicesToCopy , _u32 newLen );
		void resizeIndicesCopyBelowPosition( _u32 position , _u32 newLen );
		
		//! Reads the buffer and fills the internal helper array 'indicesOfMultibyte'
		void refreshIndices();
		
		//! Frees the internal buffer of indices and sets it to the supplied value
		void resetIndices( _s32* indicesOfMultibyte = nullptr , _u32 indicesLen = 0 ){
			delete[] this->indicesOfMultibyte;
			this->indicesLen = indicesLen;
			this->indicesOfMultibyte = indicesOfMultibyte;
		}
		
		//! Frees the internal buffer of indices
		void resetBuffer( _char* buffer = nullptr , _u32 bufferLen = 0 ){
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
		static _u8 getNumBytesOfUTF8CharEnd( _literal data )
		{
			if( (data[-1] & 0xE0) == 0xC0 )  // 110XXXXX  two bytes
				return 2;
			else if( (data[-2] & 0xF0) == 0xE0 )  // 1110XXXX  three bytes
				return 3;
			else if( (data[-3] & 0xF8) == 0xF0 )  // 11110XXX  four bytes
				return 4;
			else if( (data[-4] & 0xFC) == 0xF8 )  // 111110XX  five bytes
				return 5;
			else if( (data[-5] & 0xFE) == 0xFC )  // 1111110X  six bytes
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
			if( codepoint <= 0x7FF )
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
		
		//! Ctor from buffer and indices
		wstring( _char* buffer , _wchar bufferLen , _u32 stringLen , _s32* indicesOfMultibyte , _u32 indicesLen ) :
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
		
		//! Default Ctor
		wstring() :
			buffer( new _char[1] )
			, bufferLen( 1 )
			, stringLen( 0 )
			, indicesOfMultibyte( new _s32[1] )
			, indicesLen( 1 )
			, misFormated( false )
		{
			this->buffer[0] = 0;
			this->indicesOfMultibyte[0] = -1;
		}
		
		//! Ctor from string literal
		wstring( _literal str );
		
		//! Ctor from std::string literal
		wstring( string str ) :
			wstring( str.c_str() )
		{}
		
		//! Ctor as wchar buffer
		wstring( _u32 n , _wchar ch );
		
		//! Ctor as char buffer
		wstring( _u32 n , _char ch ) :
			wstring( n , (_wchar)ch )
		{}
		
		//! Copy Ctor
		wstring( const wstring& str );
		
		//! Move Ctor
		wstring( wstring&& str );
		
		//! Constructor from wide character literal
		wstring( _wliteral str );
		
		
		//! Dtor
		~wstring(){
			this->resetIndices();
			this->resetBuffer();
		}
		
		
		//! Copy Assignment operator
		wstring& operator=( const wstring& str );
		
		//! Move Assignment operator
		wstring& operator=( wstring&& str );
		
		
		//! Clears the whole string
		void clear();
		
		
		//! Returns the codepoint at place n
		_wchar at( _u32 n ) const ;
		
		
		//! Returns a reference to the codepoint at position n
		wstring_reference operator[]( _u32 n ){
			return wstring_reference( n , *this );
		}
		
		//! Returns a reference to the codepoint at byte n
		wstring_raw_reference operator()( _u32 n ){
			return wstring_raw_reference( n , *this );
		}
		
		
		//! Get the data as const char*
		_literal c_str() const { return this->buffer; }
		_literal data() const { return this->buffer; }
		string cpp_str() const { return string( this->buffer ); }
		
		
		//! Returns the number of codepoints in this wstring
		_u32 length() const { return this->stringLen; }
		
		
		//! Returns the size in bytes used to store the utf8 string (excluding the trailling zero)
		_u32 size() const { return this->bufferLen - 1; }
		
		
		//! Returns true, if the wstring is empty
		bool empty() const { return !this->buffer || this->buffer[0] == 0; }
		
		
		//! Returns a wide character string literal
		_uniquePtr<_wchar[]> toWideLiteral() const ;
		
		
		//! Get an iterator to the beginning of the wstring
		wstring_iterator begin(){ return wstring_iterator( 0 , *this ); }
		
		//! Get an iterator to the end of the wstring
		wstring_iterator end(){ return wstring_iterator( this->bufferLen - 1 , *this ); }
		
		//! Get an iterator to the beginning of the wstring
		wstring_const_iterator begin() const { return wstring_const_iterator( 0 , *this ); }
		
		//! Get an iterator to the end of the wstring
		wstring_const_iterator end() const { return wstring_const_iterator( this->bufferLen - 1 , *this ); }
		
		//! Get a reverse iterator to the beginning of the wstring
		wstring_reverse_iterator rbegin(){ return wstring_reverse_iterator( this->bufferLen - this->getIndexPreBytes( this->bufferLen - 1 ) , *this ); }
		
		//! Get a reverse iterator to the end of the wstring
		wstring_reverse_iterator rend(){ return wstring_reverse_iterator( -1 , *this ); }
		
		//! Get an iterator to the beginning of the unmodifyable wstring
		wstring_const_iterator cbegin() const { return wstring_const_iterator( 0 , *this ); }
		
		//! Get an iterator to the end of the unmodifyable wstring
		wstring_const_iterator cend() const { return wstring_const_iterator( this->bufferLen - 1 , *this ); }
		
		//! Get a reverse iterator to the beginning of the unmodifyable wstring
		wstring_const_reverse_iterator crbegin() const { return wstring_const_reverse_iterator( this->bufferLen - this->getIndexPreBytes( this->bufferLen - 1 ) , *this ); }
		
		//! Get a reverse iterator to the end of the unmodifyable wstring
		wstring_const_reverse_iterator crend() const { return wstring_const_reverse_iterator( -1 , *this ); }
		
		
		//! Returns a reference to first codepoint in the wstring
		wstring_raw_reference front(){
			return wstring_raw_reference( 0 , *this );
		}
		
		//! Returns a reference to last codepoint in the wstring
		wstring_raw_reference back(){
			return wstring_raw_reference( this->stringLen ? this->bufferLen - 2 : 0 , *this );
		}
		
		
		//! Replace a part of the wstring by another
		wstring& replace( _u32 index , _wchar replacement , _u32 n = 1 ){
			replace( index , 1 , wstring( n ,replacement ) );
			return *this;
		}
		wstring& replace( _u32 index , _u32 len , _wchar replacement , _u32 n = 1 ){
			replace( index , len , wstring( n ,replacement ) );
			return *this;
		}
		wstring& replace( wstring_iterator first , wstring_iterator last , _wchar replacement , _u32 n = 1 ){
			rawReplace( first.getIndex() , last.getIndex() - first.getIndex() , wstring( n ,replacement ) );
			return *this;
		}
		wstring& replace( wstring_iterator first , wstring_iterator last , const wstring& replacement ){
			rawReplace( first.getIndex() , last.getIndex() - first.getIndex() , replacement );
			return *this;
		}
		wstring& replace( _u32 index , _u32 count , const wstring& replacement ){
			_s32	actualStartIndex = this->getActualIndex( index );
			_s32	actualEndIndex = count ? this->getActualIndex( index + count ) : actualStartIndex;
			this->rawReplace( actualStartIndex , actualEndIndex - actualStartIndex , replacement );
			return *this;
		}
		
		
		//! Appends the supplied wstring to the end of this wstring
		wstring& append( const wstring& appendix ){
			replace( this->stringLen , 0 , appendix );
			return *this;
		}
		wstring& operator+=( const wstring& summand ){
			replace( this->stringLen , 0 , summand );
			return *this;
		}
		wstring& push_back( _wchar ch ){
			replace( this->stringLen , 0 , wstring( 1 , ch ) );
			return *this;
		}
		wstring operator+( const wstring& summand ) const {
			wstring str = *this;
			str.append( summand );
			return move( str );
		}
		
		
		//! Insert the supplied wstring/wchar at the supplied position
		wstring& insert( _u32 pos , _wchar ch ){
			replace( pos , 0 , wstring( 1 , ch ) );
			return *this;
		}
		wstring& insert( _u32 pos , wstring str ){
			replace( pos , 0 , str );
			return *this;
		}
		wstring& insert( wstring_iterator it , _wchar ch ){
			rawReplace( it.getIndex() , 0 , wstring( 1 , ch ) );
			return *this;
		}
		
		
		//! Erases a portion of this wstring
		wstring& erase( wstring_iterator pos ){
			rawReplace( pos.getIndex() , this->getIndexBytes(pos.getIndex())  , wstring() );
			return *this;
		}
		wstring& erase( wstring_iterator first , wstring_iterator last ){
			rawReplace( first.getIndex() , last.getIndex() - first.getIndex() , wstring() );
			return *this;
		}
		wstring& erase( _u32 pos , _u32 len ){
			replace( pos , len , wstring() );
			return *this;
		}
		
		
		//! Returns a substring of this string
		wstring substr( wstring_iterator first , wstring_iterator last ) const {
			_u32 byteCount = last.getIndex() - first.getIndex();
			return rawSubstr( first.getIndex() , byteCount , this->getNumberOfResultingCodepoints( first.getIndex() , byteCount ) );
		}
		wstring substr( _u32 pos , _u32 len ) const {
			_s32	actualStartIndex = this->getActualIndex( pos );
			_s32	actualEndIndex = len ? this->getActualIndex( pos + len ) : actualStartIndex;
			return this->rawSubstr( actualStartIndex , actualEndIndex - actualStartIndex , len );
		}
		
		
		//! Removes the last codepoint in the wstring
		wstring& pop_back(){
			rawReplace( this->bufferLen - 1 , this->getIndexBytes(this->bufferLen - 1) , wstring() );
			return *this;
		}
		
		/**
		 * Compares this wstring to the supplied one.
		 *
		 * @return	0	They compare equal
		 *			<0	Either the value of the first character that does not match is lower in
		 *			the compared string, or all compared characters match but the compared string is shorter.
		 *			>0	Either the value of the first character that does not match is greater in
		 *			the compared string, or all compared characters match but the compared string is longer.
		 */
		_s32 compare( const wstring& str ) const ;
		
		//! Compare two wstrings
		bool operator==( const wstring& str ) const { return str.compare( *this ) == 0; }
		bool operator!=( const wstring& str ) const { return str.compare( *this ) != 0; }
		bool operator>( const wstring& str ) const { return str.compare( *this ) > 0; }
		bool operator>=( const wstring& str ) const { return str.compare( *this ) >= 0; }
		bool operator<( const wstring& str ) const { return str.compare( *this ) < 0; }
		bool operator<=( const wstring& str ) const { return str.compare( *this ) <= 0; }
		
		
		//! Get the number of bytes of codepoint in wstring
		_u8 getCodepointBytes( _u32 codepointIndex ) const {
			return getNumBytesOfUTF8Char( this->buffer[this->getActualIndex(codepointIndex)] );
		}
		_u8 getIndexBytes( _u32 byteIndex ) const {
			return getNumBytesOfUTF8Char( this->buffer[byteIndex] );
		}
		
		
		//! Get the number of bytes before a codepoint, that build up a new codepoint
		_u8 getCodepointPreBytes( _u32 codepointIndex ) const {
			return codepointIndex > 0 ? getNumBytesOfUTF8CharEnd( this->buffer + this->getActualIndex(codepointIndex) - 1 ) : 1;
		}
		_u8 getIndexPreBytes( _u32 byteIndex ) const {
			return byteIndex > 0 ? getNumBytesOfUTF8CharEnd( this->buffer + byteIndex - 1 ) : 1;
		}
		
		
		/**
		 * Raw access on byte indexing
		 */
		
		//! Retuns the codepoint at specified byte index
		_wchar rawAt( _u32 byteIndex ) const {
			_wchar dest;
			decodeUTF8( this->buffer + byteIndex , dest , this->misFormated );
			return dest;
		}
		
		//! Replaces a part of the string that gets indexed of byte-base
		void rawReplace( _u32 byteIndex , _u32 byteCount , const wstring& replacement );
		
		//! Returns a portion of the string (indexed on byte-base)
		wstring rawSubstr( _u32 byteIndex , _u32 byteCount , _u32 numCodepoints ) const ;
};