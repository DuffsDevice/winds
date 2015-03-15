#include <_type/type.h>
#include <string.h>

optimized wstring::wstring( _u32 number , _wchar ch ) :
	stringLen( number )
	, indicesOfMultibyte( ch <= 0x7F ? nullptr : new _u32[number] )
	, indicesLen( ch <= 0x7F ? 0 : number )
{
	_u8 numBytes = getNumBytesOfUTF8Codepoint( ch );
	
	this->bufferLen = ( ch ? number * numBytes : 0 ) + 1;
	this->buffer = new _char[this->bufferLen];
	
	if( numBytes == 1 )
		while( number-- > 0 )
			this->buffer[number] = ch;
	else
	{
		_char representation[6];
		encodeUTF8( ch , representation , this->misFormated );
		
		while( number-- > 0 )
		{
			_u8 byte = numBytes;
			while( byte-- > 0 )
				this->buffer[ number * numBytes + byte ] = representation[byte];
			this->indicesOfMultibyte[number] = number * numBytes;
		}
	}
	
	// Set trailling zero
	this->buffer[this->bufferLen-1] = 0;
}

optimized wstring::wstring( _literal str , _u32 len ) :
	stringLen( 0 )
	, indicesOfMultibyte( nullptr )
	, misFormated( false )
{
	if( str )
	{
		_u32	numMultibytes = 0;
		_u8		bytesToSkip = 0;
		_u32	i = 0;
		
		// Count Multibytes
		for( ; str[i] && this->stringLen < len ; i++ )
		{
			stringLen++; // Count number of codepoints
			
			if( str[i] <= 0x7F ) // lead bit is zero, must be a single ascii
				continue;
			
			numMultibytes++; // Increase counter
			
			// Compute the number of bytes to skip
			bytesToSkip = getNumBytesOfUTF8Char( str[i] ) - 1;
			
			// Check if there is no valid starting byte sequence
			if( !bytesToSkip ){
				this->misFormated = true;
				continue;
			}
			
			// Check if the string doesn't end just right inside the multibyte part!
			do{
				if( !str[i+1] || (str[i+1] & 0xC0) != 0x80 )
					break;
				i++;
			}while( --bytesToSkip > 0 );
		}
		
		if( bytesToSkip != 0 ) // Codepoint suddently ended!
			this->misFormated = true;
		
		// Initialize buffer
		this->bufferLen = i + 1;
		this->buffer = new _char[this->bufferLen];
		memcpy( this->buffer , str , this->bufferLen - 1 );
		this->buffer[bufferLen-1] = 0;
		
		// initialize indices table
		this->indicesLen = numMultibytes;
		this->indicesOfMultibyte = this->indicesLen ? new _u32[this->indicesLen] : nullptr;
		
		_u32 multibyteIndex = 0;
		
		// Fill Multibyte Table
		for( _u32 index = 0 ; index < i ; index++ )
		{
			if( buffer[index] <= 0x7F ) // lead bit is zero, must be a single ascii
				continue;
			
			this->indicesOfMultibyte[multibyteIndex++] = index;
			
			// Compute number of bytes to skip
			bytesToSkip = getNumBytesOfUTF8Char( buffer[index] ) - 1;
			
			if( !bytesToSkip )
				continue;
			
			if( this->misFormated ){
				do{
					if( index + 1 == i || (buffer[index+1] & 0xC0) != 0x80 )
						break;
					index++;
				}while( --bytesToSkip > 0 );
			}
			else
				index += bytesToSkip;
		}
	}
	else{
		this->buffer = nullptr;
		this->bufferLen = 0;
		this->indicesLen = 0;
	}
}

optimized wstring::wstring( _wliteral str ) :
	indicesOfMultibyte( nullptr )
	, indicesLen( 0 )
	, misFormated( false )
{
	if( str )
	{
		_u32	numMultibytes;
		_u32	nextIndexPosition = 0;
		_u32	stringLen = 0;
		_u32	numBytes = getNumberOfRequiredBytes( str , numMultibytes );
		
		// Initialize the internal buffer
		this->bufferLen = numBytes + 1; // +1 for the trailling zero
		this->buffer = new _char[this->bufferLen];
		this->indicesLen = numMultibytes;
		this->indicesOfMultibyte = this->indicesLen ? new _u32[this->indicesLen] : nullptr;
		
		_char*	curWriter = this->buffer;
		
		// Iterate through wide char literal
		for( _u32 i = 0; str[i] ; i++ )
		{
			// Encode wide char to utf8
			_u8 numBytes = encodeUTF8( str[i] , curWriter , misFormated );
			
			// Push position of character to 'indicesOfMultibyte'
			if( numBytes > 1 )
				this->indicesOfMultibyte[nextIndexPosition++] = curWriter - this->buffer;
			
			// Step forward with copying to internal utf8 buffer
			curWriter += numBytes;
			
			// Increase string length by 1
			stringLen++;
		}
		
		this->stringLen = stringLen;
		
		// Add trailling \0 char to utf8 buffer
		*curWriter = 0;
	}
	else{
		this->buffer = nullptr;
		this->bufferLen = 0;
	}
}




optimized wstring::wstring( wstring&& str ) :
	buffer( str.buffer )
	, bufferLen( str.bufferLen )
	, stringLen( str.stringLen )
	, indicesOfMultibyte( str.indicesOfMultibyte )
	, indicesLen( str.indicesLen )
	, misFormated( str.misFormated )
{
	// Reset old string
	str.buffer = nullptr;
	str.indicesOfMultibyte = nullptr;
	str.indicesLen = 0;
	str.bufferLen = 0;
	str.stringLen = 0;
	str.misFormated = true;
}

optimized wstring::wstring( const wstring& str ) :
	buffer( new _char[str.bufferLen] )
	, bufferLen( str.bufferLen )
	, stringLen( str.stringLen )
	, indicesOfMultibyte( str.indicesLen ? new _u32[str.indicesLen] : nullptr )
	, indicesLen( str.indicesLen )
	, misFormated( str.misFormated )
{
	// Clone data
	memcpy( this->buffer , str.buffer , str.bufferLen );
	
	// Clone indices
	if( str.indicesLen > 0 )
		memcpy( this->indicesOfMultibyte , str.indicesOfMultibyte , str.indicesLen * sizeof(_u32) );
}




optimized wstring& wstring::operator=( const wstring& str )
{
	_char*	newBuffer = new _char[str.bufferLen];
	_u32*	newIndices = str.indicesLen ? new _u32[str.indicesLen] : nullptr;
	
	// Clone data
	memcpy( newBuffer , str.buffer , str.bufferLen );
	
	// Clone indices
	if( str.indicesLen > 0 )
		memcpy( newIndices , str.indicesOfMultibyte , str.indicesLen * sizeof(_u32) );
	
	this->misFormated = str.misFormated;
	this->stringLen = str.stringLen;
	resetBuffer( newBuffer , str.bufferLen );
	resetIndices( newIndices , str.indicesLen );
	return *this;
}

optimized wstring& wstring::operator=( wstring&& str )
{
	this->misFormated = str.misFormated;
	this->stringLen = str.stringLen;
	
	// Copy data
	resetBuffer( str.buffer , str.bufferLen );
	resetIndices( str.indicesOfMultibyte , str.indicesLen );
	
	// Reset old string
	str.indicesOfMultibyte = nullptr;
	str.indicesLen = 0;
	str.buffer = nullptr;
	str.bufferLen = 0;
	str.misFormated = true;
	str.stringLen = 0;
	return *this;
}




optimized _u8 wstring::decodeUTF8( _literal data , _wchar& dest , bool& hasError )
{
	_u8 firstChar = *data;
	
	if( !firstChar ){
		dest = 0;
		return 1;
	}
	
	_wchar	codepoint;
	_u8		numBytes;
	
	if( firstChar <= 0x7F ){ // 0XXX XXXX one byte
		dest = firstChar;
		return 1;
	}
	else if( (firstChar & 0xE0) == 0xC0 ){  // 110X XXXX  two bytes
		codepoint = firstChar & 31;
		numBytes = 2;
	}
	else if( (firstChar & 0xF0) == 0xE0 ){  // 1110 XXXX  three bytes
		codepoint = firstChar & 15;
		numBytes = 3;
	}
	else if( (firstChar & 0xF8) == 0xF0 ){  // 1111 0XXX  four bytes
		codepoint = firstChar & 7;
		numBytes = 4;
	}
	else if( (firstChar & 0xFC) == 0xF8 ){  // 1111 10XX  five bytes
		codepoint = firstChar & 3;
		numBytes = 5;
		hasError = true;
	}
	else if( (firstChar & 0xFE) == 0xFC ){  // 1111 110X  six bytes
		codepoint = firstChar & 1;
		numBytes = 6;
		hasError = true;
	}
	else{ // not a valid first byte of a UTF-8 sequence
		codepoint = firstChar;
		numBytes = 1;
		hasError = true;
	}
	
	for( int i = 1 ; i < numBytes ; i++ )
	{
		if( !data[i] || (data[i] & 0xC0) != 0x80 ){
			hasError = true;
			numBytes = i;
			break;
		}
		codepoint = (codepoint << 6) | ( data[i] & 0x3F );
	}
	
	dest = codepoint;
	
	return numBytes;
}




optimized _u32 wstring::getNumberOfRequiredBytes( _wliteral lit , _u32& numMultibytes )
{
	_u32 numBytes = 0;
	numMultibytes = 0;
	for( _u32 i = 0 ; lit[i] ; i++ ){
		_u8 curBytes = getNumBytesOfUTF8Codepoint( lit[i] );
		if( curBytes > 1 )
			numMultibytes++;
		numBytes += curBytes;
	}
	return numBytes;
}

optimized _u32 wstring::getNumberOfResultingCodepoints( _u32 byteStart , _u32 byteCount ) const 
{
	_u32	curIndex = byteStart;
	_u32	codepointCount = 0;
	
	while( byteCount > 0 ){
		_u8 numBytes = getNumBytesOfUTF8Char( this->buffer[curIndex] );
		curIndex += numBytes;
		byteCount -= numBytes;
		codepointCount++;
	}
	return codepointCount;
}

optimized _u32 wstring::getNumberOfResultingBytes( _u32 byteStart , _u32 codepointCount ) const 
{
	_u32 curByte = byteStart;
	
	while( curByte < size() && codepointCount-- > 0 )
		curByte += getIndexBytes( curByte );
	
	return curByte - byteStart;
}




optimized _u8 wstring::encodeUTF8( _wchar codepoint , _char* dest , bool& hasError )
{
	if( !dest )
		return 1;
	
	_u8 numBytes = 0;
	
	if( codepoint <= 0x7F ){ // 0XXXXXXX one byte
		dest[0] = _char(codepoint);
		numBytes = 1;
	}
	else if( codepoint <= 0x7FF ){ // 110XXXXX  two bytes
		dest[0] = _char( 0xC0 | (codepoint >> 6) );
		dest[1] = _char( 0x80 | (codepoint & 0x3F) );
		numBytes = 2;
	}
	else if( codepoint <= 0xFFFF ){ // 1110XXXX  three bytes
		dest[0] = _char( 0xE0 | (codepoint >> 12) );
		dest[1] = _char( 0x80 | ((codepoint >> 6) & 0x3F) );
		dest[2] = _char( 0x80 | (codepoint & 0x3F) );
		numBytes = 3;
	}
	else if( codepoint <= 0x1FFFFF ){ // 11110XXX  four bytes
		dest[0] = _char( 0xF0 | (codepoint >> 18) );
		dest[1] = _char( 0x80 | ((codepoint >> 12) & 0x3F) );
		dest[2] = _char( 0x80 | ((codepoint >> 6) & 0x3F) );
		dest[3] = _char( 0x80 | (codepoint & 0x3F) );
		numBytes = 4;
	}
	else if( codepoint <= 0x3FFFFFF ){ // 111110XX  five bytes
		dest[0] = _char( 0xF8 | (codepoint >> 24) );
		dest[1] = _char( 0x80 | (codepoint >> 18) );
		dest[2] = _char( 0x80 | ((codepoint >> 12) & 0x3F) );
		dest[3] = _char( 0x80 | ((codepoint >> 6) & 0x3F) );
		dest[4] = _char( 0x80 | (codepoint & 0x3F) );
		numBytes = 5;
		hasError = true;
	}
	else if( codepoint <= 0x7FFFFFFF ){ // 1111110X  six bytes
		dest[0] = _char( 0xFC | (codepoint >> 30) );
		dest[1] = _char( 0x80 | ((codepoint >> 24) & 0x3F) );
		dest[2] = _char( 0x80 | ((codepoint >> 18) & 0x3F) );
		dest[3] = _char( 0x80 | ((codepoint >> 12) & 0x3F) );
		dest[4] = _char( 0x80 | ((codepoint >> 6) & 0x3F) );
		dest[5] = _char( 0x80 | (codepoint & 0x3F) );
		numBytes = 6;
		hasError = true;
	}
	else
		hasError = true;
	
	//printf(" [%x] -> %db (0x%x)\n",codepoint,numBytes,dest[0]);
	
	return numBytes;
}




optimized _u32 wstring::getActualIndex( _u32 requestedIndex ) const
{
	_u32 indexMultibyteTable = 0;
	_u32 currentOverhead = 0;
	
	while( indexMultibyteTable < this->indicesLen )
	{
		_u32 multibytePos = this->indicesOfMultibyte[indexMultibyteTable];
		
		if( multibytePos >= requestedIndex + currentOverhead )
			break;
		
		indexMultibyteTable++;
		currentOverhead += getNumBytesOfUTF8Char( this->buffer[multibytePos] ) - 1; // Ad bytes of multibyte to overhead
	}
	
	return requestedIndex + currentOverhead;
}



optimized wstring wstring::rawSubstr( _u32 byteIndex , _u32 tmpByteCount , _u32 numCodepoints ) const
{	
	_s32 byteCount;
	
	if( tmpByteCount < size() - byteIndex )
		byteCount = tmpByteCount;
	else
		byteCount = size() - byteIndex;
	
	if( byteCount <= 0 )
		return wstring();
	
	_u32	actualStartIndex = byteIndex;
	_u32	actualEndIndex = byteIndex + byteCount;
	
	//
	// Manage indices
	//
	_u32	startOfMultibytesInRange;
	
	// Look for start of indices
	_u32 tmp = 0;
	while( tmp < this->indicesLen && this->indicesOfMultibyte[tmp] < actualStartIndex )
		tmp++;
	startOfMultibytesInRange = tmp;
	
	// Look for the end
	while( tmp < this->indicesLen && this->indicesOfMultibyte[tmp] < actualEndIndex )
		tmp++;
	
	// Compute number of indices 
	_u32	newIndicesLen = tmp - startOfMultibytesInRange;
	_u32*	newIndices = newIndicesLen ? new _u32[newIndicesLen] : nullptr;
	
	// Copy indices
	for( _u32 i = 0 ; i < newIndicesLen ; i++ )
		newIndices[i] = this->indicesOfMultibyte[startOfMultibytesInRange + i] - actualStartIndex;
	
	//
	// Manage utf8 string
	//
	
	// Allocate new buffer
	_char*	newBuffer = new _char[byteCount+1];
	
	// Partly copy
	memcpy( newBuffer , this->buffer + actualStartIndex , byteCount );
	
	newBuffer[byteCount] = 0;
	byteCount++;
	
	return wstring( newBuffer , byteCount , numCodepoints , newIndices , newIndicesLen );
}




optimized void wstring::rawReplace( _u32 actualStartIndex , _u32 replacedBytes , const wstring& replacement )
{
	_u32	replacementBytes = replacement.size();
	_u32	replacementIndices = replacement.indicesLen;
	_s32	byteDifference = replacedBytes - replacementBytes;
	_u32	actualEndIndex;
	
	if( replacedBytes < size() - actualStartIndex )
		actualEndIndex = actualStartIndex + replacedBytes;
	else{
		actualEndIndex = size();
		replacedBytes = actualEndIndex - actualStartIndex;
	}
	
	//
	// Manage indices
	//
	_u32	startOfMultibytesInRange;
	_u32	numberOfMultibytesInRange;
	
	// Look for start of indices
	_u32 tmp = 0;
	while( tmp < this->indicesLen && this->indicesOfMultibyte[tmp] < actualStartIndex )
		tmp++;
	startOfMultibytesInRange = tmp;
	
	// Look for the end
	while( tmp < this->indicesLen && this->indicesOfMultibyte[tmp] < actualEndIndex )
		tmp++;
	
	// Compute number of indices 
	numberOfMultibytesInRange = tmp - startOfMultibytesInRange;
	
	// Compute difference in number of indices
	_s32	indicesDifference = numberOfMultibytesInRange - replacementIndices;
	
	// Create new buffer
	_u32	newIndicesLen = this->indicesLen - indicesDifference;
	_u32*	newIndices = newIndicesLen ? new _u32[ newIndicesLen ] : nullptr;
	
	
	// Copy values before replacement start
	for( _u32 i = 0 ; i < this->indicesLen && this->indicesOfMultibyte[i] < actualStartIndex ; i++ )
		newIndices[i] = this->indicesOfMultibyte[i];
	
	// Copy the values after the replacement
	if( indicesDifference != 0 ){
		for(
			_u32 i = startOfMultibytesInRange
			; i + numberOfMultibytesInRange < this->indicesLen
			; i++
		)
			newIndices[ i + replacementIndices ] = this->indicesOfMultibyte[ i + numberOfMultibytesInRange ] - byteDifference;
	}
	else{
		_u32 i = startOfMultibytesInRange + numberOfMultibytesInRange;
		while( i < this->indicesLen ){
			newIndices[i] = this->indicesOfMultibyte[i] - byteDifference;
			i++;
		}
	}
	
	// Insert indices from replacement
	for( _u32 i = 0 ; i < replacementIndices ; i++ )
		newIndices[ startOfMultibytesInRange + i ] = replacement.indicesOfMultibyte[i] + actualStartIndex;
	
	// Move and reset
	resetIndices( newIndices , newIndicesLen );
	
	
	
	//
	// Manage utf8 string
	//
	
	// Allocate new buffer
	_u32	newBufferLen = this->bufferLen + ( replacementBytes - replacedBytes );
	_char*	newBuffer = new _char[newBufferLen];
	
	// Partly copy
	// Copy string until replacement index
	memcpy( newBuffer , this->buffer , actualStartIndex );
	
	// Copy rest
	memcpy( newBuffer + actualStartIndex + replacementBytes , this->buffer + actualEndIndex , this->bufferLen - actualEndIndex );
	
	// Write bytes
	memcpy( newBuffer + actualStartIndex , replacement.buffer , replacementBytes );
	
	// Adjust string length
	this->stringLen -= getNumberOfResultingCodepoints( actualStartIndex , replacedBytes );
	this->stringLen += replacement.length();
	
	// Rewrite buffer
	resetBuffer( newBuffer , newBufferLen );
	
	//for( int i = 0 ; i < this->indicesLen ; i++ )
	//	printf("%d, ",this->indicesOfMultibyte[i] );
	//printf("\n\n");
}




optimized _wchar wstring::at( _u32 requestedIndex ) const
{
	if( !requiresUnicode() )
		return (_wchar) this->buffer[requestedIndex];
	
	// Decode internal buffer at position n
	_wchar codepoint = 0;
	decodeUTF8( this->buffer + getActualIndex( requestedIndex ) , codepoint , this->misFormated );
	
	return codepoint;
}




optimized _uniquePtr<_wchar[]> wstring::toWideLiteral() const
{
	_wchar*	dest = new _wchar[ length() ];
	_wchar*	tempDest = dest;
	_char*	source = this->buffer;
	
	while( *source && !this->misFormated )
		source += decodeUTF8( source , *tempDest++ , this->misFormated );
	
	return std::move(dest);
}




optimized _s32 wstring::compare( const wstring& str ) const
{
	wstring_const_iterator	it1 = cbegin();
	wstring_const_iterator	it2 = str.cbegin();
	wstring_const_iterator	end1 = cend();
	wstring_const_iterator	end2 = str.cend();
	
	while( it1 != end1 && it2 != end2 ){
		_s32 diff = it2.getValue() - it1.getValue();
		if( diff )
			return diff;
		it1++;
		it2++;
	}
	return ( it1 == end1 ? 1 : 0 ) - ( it2 == end2 ? 1 : 0 );
}

optimized bool wstring::equals( _literal str ) const
{
	_literal it1 = this->buffer;
	_literal it2 = str;
	
	while( *it1 && *it2 ){
		if( *it1 != *it2 )
			return false;
		it1++;
		it2++;
	}
	return *it1 == *it2;
}

optimized bool wstring::equals( _wliteral str ) const
{
	wstring_const_iterator	it = cbegin();
	wstring_const_iterator	end = cend();
	
	while( it != end && *str ){
		if( *str != it.getValue() )
			return false;
		it++;
		str++;
	}
	return *it == *str;
}




optimized _u32 wstring::find( _wchar ch , _u32 startPos ) const
{
	for( wstring::iterator it = get(startPos) ; it < end() ; it++ )
		if( *it == ch )
			return it - begin();
	return wstring::npos;
}

optimized _u32 wstring::rawFind( _wchar ch , _u32 byteStart ) const
{
	for( _u32 it = byteStart ; it < size() ; it += getIndexBytes( it ) )
		if( rawAt(it) == ch )
			return it;
	return wstring::npos;
}




optimized _u32 wstring::rfind( _wchar ch , _u32 startPos ) const
{
	wstring::reverse_iterator it = ( startPos >= length() ) ? rbegin() : rget( startPos );
	
	while( it < rend() )
	{
		if( *it == ch )
			return it - rbegin();
		it++;
	}
	return wstring::npos;
}

optimized _u32 wstring::rawRfind( _wchar ch , _u32 byteStart ) const
{
	if( byteStart >= size() )
		byteStart = backIndex();
	
	for( _s32 it = byteStart ; it >= 0 ; it -= getIndexPreBytes( it ) )
		if( rawAt(it) == ch )
			return it;
	
	return wstring::npos;
}




optimized _u32 wstring::find_first_of( _wliteral str , _u32 startPos ) const
{
	if( startPos >= length() )
		return wstring::npos;
	
	for( wstring::iterator it = get( startPos ) ; it < end() ; it++ )
	{
		_wliteral	tmp = str;
		_wchar		cur = *it;
		do{
			if( cur == *tmp )
				return it - begin();
		}while( *++tmp );
	}
	
	return wstring::npos;
}

optimized _u32 wstring::rawFind_first_of( _wliteral str , _u32 byteStart ) const
{
	for( _u32 it = byteStart ; it < size() ; it += getIndexBytes( it ) )
	{
		_wliteral	tmp = str;
		do{
			if( rawAt(it) == *tmp )
				return it;
		}while( *++tmp );
	}
	
	return wstring::npos;
}




optimized _u32 wstring::find_last_of( _wliteral str , _u32 startPos ) const
{
	wstring::reverse_iterator it = ( startPos >= length() ) ? rbegin() : rget( startPos );
	
	while( it < rend() )
	{
		_wliteral	tmp = str;
		_wchar		cur = *it;
		do{
			if( cur == *tmp )
				return it - rbegin();
		}while( *++tmp );
		it++;
	}
	
	return wstring::npos;
}

optimized _u32 wstring::rawFind_last_of( _wliteral str , _u32 byteStart ) const
{
	if( byteStart >= size() )
		byteStart = backIndex();
	
	for( _s32 it = byteStart ; it >= 0 ; it -= getIndexPreBytes( it ) )
	{
		_wliteral	tmp = str;
		_wchar		cur = rawAt(it);
		do{
			if( cur == *tmp )
				return it;
		}while( *++tmp );
	}
	
	return wstring::npos;
}




optimized _u32 wstring::find_first_not_of( _wliteral str , _u32 startPos ) const
{
	if( startPos >= length() )
		return wstring::npos;
	
	for( wstring::iterator it = get(startPos) ; it < end() ; it++ )
	{
		_wliteral	tmp = str;
		_wchar		cur = *it;
		do{
			if( cur == *tmp )
				goto continue2;
		}while( *++tmp );
		
		return it - begin();
		
		continue2:;
	}
	
	return wstring::npos;
}

optimized _u32 wstring::rawFind_first_not_of( _wliteral str , _u32 byteStart ) const
{
	if( byteStart >= size() )
		return wstring::npos;
	
	for( _u32 it = byteStart ; it < size() ; it += getIndexBytes( it ) )
	{
		_wliteral	tmp = str;
		_wchar		cur = rawAt(it);
		do{
			if( cur == *tmp )
				goto continue2;
		}while( *++tmp );
		
		return it;
		
		continue2:;
	}
	
	return wstring::npos;
}




optimized _u32 wstring::find_last_not_of( _wliteral str , _u32 startPos ) const
{
	wstring::reverse_iterator it = ( startPos >= length() ) ? rbegin() : rget( startPos );
	
	while( it < rend() )
	{
		_wliteral	tmp = str;
		_wchar		cur = *it;
		do{
			if( cur == *tmp )
				goto continue2;
		}while( *++tmp );
		
		return it - rbegin();
		
		continue2:
		it++;
	}
	
	return wstring::npos;
}

optimized _u32 wstring::rawFind_last_not_of( _wliteral str , _u32 byteStart ) const
{
	if( byteStart >= size() )
		byteStart = backIndex();
	
	for( _s32 it = byteStart ; it >= 0 ; it -= getIndexPreBytes( it ) )
	{
		_wliteral	tmp = str;
		_wchar		cur = rawAt(it);
		
		do{
			if( cur == *tmp )
				goto continue2;
		}while( *++tmp );
		
		return it;
		
		continue2:;
	}
	
	return wstring::npos;
}




optimized int operator-( const wstring::iterator& left , const wstring::iterator& right )
{
	_u32 minIndex = std::min( left.rawIndex , right.rawIndex );
	_u32 maxIndex = std::max( left.rawIndex , right.rawIndex );
	_u32 numCodepoints = left.instance.getNumberOfResultingCodepoints( minIndex , maxIndex - minIndex );
	return maxIndex == left.rawIndex ? numCodepoints : -numCodepoints;
}

optimized wstring::iterator operator+( const wstring::iterator& it , _u32 nth )
{
	if( !it.instance.requiresUnicode() )
		return wstring::iterator( it.rawIndex + nth , it.instance );
	return wstring::iterator( it.rawIndex + it.instance.getNumberOfResultingBytes( it.rawIndex , nth ) , it.instance );
}

optimized int operator-( const wstring::reverse_iterator& left , const wstring::reverse_iterator& right )
{
	_s32 minIndex = std::min( left.rawIndex , right.rawIndex );
	_s32 maxIndex = std::max( left.rawIndex , right.rawIndex );
	_u32 numCodepoints = left.instance.getNumberOfResultingCodepoints( minIndex , maxIndex - minIndex );
	return maxIndex == right.rawIndex ? numCodepoints : -numCodepoints;
}

optimized wstring::reverse_iterator operator+( const wstring::reverse_iterator& it , _u32 nth )
{
	_s32 newIndex = it.rawIndex;
	while( nth-- > 0 )
		newIndex -= it.instance.getIndexPreBytes( newIndex );
	return wstring::reverse_iterator( newIndex , it.instance );
}