#include "wstring.h"
#include <string.h>

wstring::wstring( _u32 number , _wchar ch ) :
	stringLen( number )
	, indicesOfMultibyte( new _s32[ ( ch <= 0x7F ) ? 1 : number + 1 ] )
	, indicesLen( ( ch <= 0x7F ) ? 1 : number + 1 )
{
	_u8 numBytes = this->getNumBytesOfUTF8Codepoint( ch );
	
	this->bufferLen = ch ? number * numBytes + 1 : 1;
	this->buffer = new _char[this->bufferLen];
	
	if( numBytes == 1 ){
		while( number-- > 0 )
			this->buffer[number] = ch;
	}else{
		_char representation[6];
		this->encodeUTF8( ch , representation , this->misFormated );
		while( number-- > 0 ){
			_u8 byte = numBytes;
			while( byte-- > 0 )
				this->buffer[ number * numBytes + byte ] = representation[byte];
		}
	}
	
	// Set trailling zero
	this->buffer[this->bufferLen-1] = 0;
	
	if( ch <= 0x7F )
		indicesOfMultibyte[0] = -1;
	else{
		number = indicesLen - 1;
		while( number-- > 0 )
			this->indicesOfMultibyte[number] = number;
		this->indicesOfMultibyte[this->indicesLen-1] = -1;
	}
}

wstring::wstring( _literal str ) :
	stringLen( 0 )
	, indicesOfMultibyte( nullptr )
	, misFormated( false )
{
	if( str ){
		this->bufferLen = strlen( (const char*)str ) + 1;
		this->buffer = new _char[this->bufferLen];
		this->indicesLen = this->bufferLen / multibytePartGuess;
		strcpy( (char*)this->buffer , (const char*)str );
		refreshIndices();
	}
	else{
		this->buffer = nullptr;
		this->bufferLen = 0;
		this->indicesLen = 0;
	}
}

wstring::wstring( _literal str , _u32 len ) :
	stringLen( len )
	, indicesOfMultibyte( nullptr )
	, misFormated( false )
{
	if( str ){
		this->bufferLen = std::min<_u32>( len , strlen( (const char*)str ) ) + 1;
		this->buffer = new _char[this->bufferLen];
		this->indicesLen = this->bufferLen / multibytePartGuess;
		strncpy( (char*)this->buffer , (const char*)str , bufferLen - 1 );
		this->buffer[ this->bufferLen - 1 ] = '\0';
		refreshIndices();
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
		this->indicesLen = numMultibytes + 1;
		this->indicesOfMultibyte = new _s32[this->indicesLen];
		
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
		
		// Add trailling -1 to index buffer
		this->indicesOfMultibyte[this->indicesLen-1] = -1;
		this->stringLen = stringLen;
		
		// Add trailling \0 char to utf8 buffer
		*curWriter = 0;
		
		//printf("Len: %d\n",this->bufferLen);
	}
	else{
		this->buffer = nullptr;
		this->bufferLen = 0;
	}
}

wstring::wstring( wstring&& str ) :
	buffer( str.buffer )
	, bufferLen( str.bufferLen )
	, stringLen( str.stringLen )
	, indicesOfMultibyte( str.indicesOfMultibyte )
	, indicesLen( str.indicesLen )
	, misFormated( str.misFormated )
{
	// Set to null
	str.buffer = nullptr;
	str.indicesOfMultibyte = nullptr;
	str.indicesLen = 0;
	str.bufferLen = 0;
	str.stringLen = 0;
}

wstring::wstring( const wstring& str ) :
	buffer( new _char[str.bufferLen] )
	, bufferLen( str.bufferLen )
	, stringLen( str.stringLen )
	, indicesOfMultibyte( new _s32[str.indicesLen] )
	, indicesLen( str.indicesLen )
	, misFormated( str.misFormated )
{
	// Clone data
	for( _u32 i = 0 ; i < bufferLen ; i++ )
		this->buffer[i] = str.buffer[i];
	
	// Clone indices
	for( _u32 i = 01 ; i < indicesLen ; i++ )
		this->indicesOfMultibyte[i] = str.indicesOfMultibyte[i];
}

wstring& wstring::operator=( const wstring& str )
{
	_char*	newBuffer = new _char[str.bufferLen];
	_s32*	newIndices = new _s32[str.indicesLen];
	
	// Clone data
	for( _u32 i = 0 ; i < str.bufferLen ; i++ )
		newBuffer[i] = str.buffer[i];
	
	// Clone indices
	for( _u32 i = 01 ; i < str.indicesLen ; i++ )
		newIndices[i] = str.indicesOfMultibyte[i];
	
	this->indicesLen = str.indicesLen;
	this->bufferLen = str.bufferLen;
	this->misFormated = str.misFormated;
	this->resetBuffer( newBuffer , str.bufferLen );
	this->resetIndices( newIndices , str.indicesLen );
	return *this;
}

wstring& wstring::operator=( wstring&& str )
{
	this->indicesLen = str.indicesLen;
	this->bufferLen = str.bufferLen;
	this->misFormated = str.misFormated;
	this->stringLen = str.stringLen;
	this->resetBuffer( str.buffer , str.bufferLen );
	this->resetIndices( str.indicesOfMultibyte , str.indicesLen );
	str.indicesOfMultibyte = nullptr;
	str.buffer = nullptr;
	return *this;
}

void wstring::clear(){
	_char*	newBuffer = new _char[1];
	_s32*	newIndices = new _s32[1];
	this->stringLen = 0;
	this->misFormated = false;
	this->buffer[0] = 0;
	this->indicesOfMultibyte[0] = -1;
	this->resetBuffer( newBuffer , 1 );
	this->resetIndices( newIndices , 1 );
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
		codepoint = firstChar;
		numBytes = 1;
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

_u32 wstring::getNumberOfRequiredBytes( _wliteral lit , _u32& numMultibytes )
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

_u32 wstring::getNumberOfResultingCodepoints( _u32 byteStart , _u32 byteCount ) const 
{
	_u32	curIndex = byteStart;
	_u32	codepointCount = 0;
	
	while( byteCount > 0 ){
		_u8 numBytes = this->getNumBytesOfUTF8Char( this->buffer[curIndex] );
		curIndex += numBytes;
		byteCount -= numBytes;
		codepointCount++;
	}
	return codepointCount;
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

void wstring::resizeIndices( _u32 indicesToCopy , _u32 newIndicesLen )
{
	_s32* newIndices = new _s32[newIndicesLen];
	
	// Copy values
	for( _u32 i = 0 ; i < indicesToCopy ; i++ )
		newIndices[i] = this->indicesOfMultibyte[i];
	
	// Move and reset
	this->resetIndices( newIndices , newIndicesLen );
}

optimized void wstring::refreshIndices()
{
	_u32 nextIndex = 0;
	_u32 stringLen = 0;
	_u32 indicesLen = this->indicesLen;
	
	// Create array
	if( this->indicesOfMultibyte == nullptr )
		this->resetIndices( new _s32[indicesLen] , indicesLen );
	
	_u8 bytesToSkip = 0;
	
	for( _u32 i = 0; buffer[i] ; i++ )
	{
		stringLen++; // Count number of codepoints
		
		if( buffer[i] <= 0x7F ) // lead bit is zero, must be a single ascii
			continue;
		
		// We already know its a multibyte!
		if( nextIndex >= this->indicesLen ) // Do we have to resize our array of indices?
			resizeIndices( this->indicesLen , this->indicesLen + std::max<_u32>( 5 , ( this->indicesLen * ( bufferLen - i ) / bufferLen ) ) );
		
		this->indicesOfMultibyte[nextIndex++] = i;
		
		if( (buffer[i] & 0x80) != 0xC0 )		// 110xxxxx (2-byte)
			bytesToSkip = 1;
		else if( (buffer[i] & 0xF0 ) == 0xE0 )	// 1110xxxx (3-byte)
			bytesToSkip = 2;
		else if( (buffer[i] & 0xF8 ) == 0xF0 )	// 11110xxx (4-byte)
			bytesToSkip = 3;
		else if( (buffer[i] & 0xFC ) == 0xF8 )	// 111110xx (5-byte)
			bytesToSkip = 4;
		else if( (buffer[i] & 0xFE ) == 0xFC )	// 1111110x (6-byte)
			bytesToSkip = 5;
		else{
			this->misFormated = true;
			break;
		}
		
		// Check if the string doesn't end just right inside the multibyte part!
		do{
			if( !buffer[i] || (buffer[i] & 0xC0) != 0x80 )
				break;
			i++;
		}while( --bytesToSkip > 0 );
	}
	
	// Reset remaining indices to -1
	for( _u32 i = nextIndex ; i < this->indicesLen ; i++ )
		this->indicesOfMultibyte[i] = -1;
	
	if( bytesToSkip != 0 ) // Codepoint suddently ended!
		this->misFormated = true;
	
	this->stringLen = stringLen;
}

optimized _u32 wstring::getActualIndex( _u32 requestedIndex ) const
{
	_u32 indexMultibyteTable = 0;
	_s32 currentOverhead = 0;
	
	while( true )
	{
		_s32 multibytePosition = this->indicesOfMultibyte[indexMultibyteTable];
		if( multibytePosition < 0 || multibytePosition >= _s32( requestedIndex + currentOverhead ) )
			break;
		indexMultibyteTable++;
		currentOverhead += getNumBytesOfUTF8Char( this->buffer[multibytePosition] ) - 1;
	}
	
	return requestedIndex + currentOverhead;
}

optimized wstring wstring::rawSubstr( _u32 byteIndex , _u32 byteCount , _u32 numCodepoints ) const
{
	byteCount = std::min( byteCount , this->size() - byteIndex );
	
	if( !byteCount )
		return wstring();
	
	_u32	actualStartIndex = byteIndex;
	_u32	actualEndIndex = byteIndex + byteCount;
	
	//
	// Manage indices
	//
	_u32	startOfMultibytesInRange;
	
	// Look for start of indices
	_u32 i = 0;
	while( this->indicesOfMultibyte[i] >= 0 && (_u32)this->indicesOfMultibyte[i] < actualStartIndex )
		i++;
	startOfMultibytesInRange = i;
	
	// Look for the end
	while( this->indicesOfMultibyte[i] >= 0 && (_u32)this->indicesOfMultibyte[i] < actualEndIndex )
		i++;
	
	// Compute number of indices 
	_u32 indicesLen = i - startOfMultibytesInRange; // +1 for trailling (-1)
	_s32* newIndices = new _s32[indicesLen+1];
	
	// Copy indices
	for( _u32 i = 0 ; i < indicesLen ; i++ )
		newIndices[ i ] = this->indicesOfMultibyte[startOfMultibytesInRange + i] - actualStartIndex;
	
	newIndices[indicesLen] = -1; // Add trailling -1
	indicesLen++;
	
	
	//
	// Manage utf8 string
	//
	
	// Allocate new buffer
	_char*	newBuffer = new _char[byteCount+1];
	
	// Partly copy
	memcpy( newBuffer , this->buffer + actualStartIndex , byteCount );
	
	newBuffer[byteCount] = -1;
	byteCount++;
	
	return wstring( newBuffer , bufferLen , numCodepoints , newIndices , indicesLen );
}

optimized void wstring::rawReplace( _u32 actualStartIndex , _u32 replacedBytes , const wstring& replacement )
{
	_u32	actualEndIndex = actualStartIndex + replacedBytes;
	_u32	replacementBytes = replacement.size();
	_u32	replacementIndices = replacement.indicesLen - 1;
	_s32	byteDifference = replacedBytes - replacementBytes;
	
	//printf("[%d-%d] +%d => +%d\n",actualStartIndex,actualEndIndex,replacedBytes,replacementBytes);
	
	//for( int i = 0 ; i < this->indicesLen ; i++ )
	//	printf("%d, ",this->indicesOfMultibyte[i] );
	//printf("\n");
	
	
	
	//
	// Manage indices
	//
	_u32	startOfMultibytesInRange;
	_u32	numberOfMultibytesInRange;
	
	// Look for start of indices
	_u32 i = 0;
	while( this->indicesOfMultibyte[i] >= 0 && (_u32)this->indicesOfMultibyte[i] < actualStartIndex )
		i++;
	startOfMultibytesInRange = i;
	
	// Look for the end
	while( this->indicesOfMultibyte[i] >= 0 && (_u32)this->indicesOfMultibyte[i] < actualEndIndex )
		i++;
	
	// Compute number of indices 
	numberOfMultibytesInRange = i - startOfMultibytesInRange;
	
	// Compute difference in number of indices
	_s32	indicesDifference = numberOfMultibytesInRange - ( replacement.indicesLen - 1 );
	
	// Create new buffer
	_u32	newIndicesLen = this->indicesLen - indicesDifference;
	_s32*	newIndices = new _s32[ newIndicesLen ];
	
	
	// Copy values before replacement start
	for( _u32 i = 0 ; this->indicesOfMultibyte[i] >= 0 && (_u32)this->indicesOfMultibyte[i] < actualStartIndex ; i++ )
		newIndices[i] = this->indicesOfMultibyte[i];
	
	// Copy the values after the replacement
	if( indicesDifference != 0 ){
		for(
			_u32 i = startOfMultibytesInRange
			; this->indicesOfMultibyte[ i + numberOfMultibytesInRange ] >= 0
			; i++
		)
			newIndices[ i + replacementIndices ] = this->indicesOfMultibyte[ i + numberOfMultibytesInRange ] - byteDifference;
	}
	else{
		_u32 i = startOfMultibytesInRange + numberOfMultibytesInRange;
		while( this->indicesOfMultibyte[i] >= 0 ){
			newIndices[i] = this->indicesOfMultibyte[i] - byteDifference;
			i++;
		}
	}
	
	// Insert indices from replacement
	for( int i = 0 ; replacement.indicesOfMultibyte[i] >= 0 ; i++ )
		newIndices[ startOfMultibytesInRange + i ] = replacement.indicesOfMultibyte[i] + actualStartIndex;
	
	// Set Terminator
	newIndices[ newIndicesLen - 1 ] = -1;
	
	// Move and reset
	this->resetIndices( newIndices , newIndicesLen );
	
	
	
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
	for( _u32 i = 0 ; i < replacementBytes ; i++ )
		newBuffer[actualStartIndex + i] = replacement.buffer[i];
	
	// Rewrite buffer
	this->resetBuffer( newBuffer , newBufferLen );
	
	//for( int i = 0 ; i < this->indicesLen ; i++ )
	//	printf("%d, ",this->indicesOfMultibyte[i] );
	//printf("\n\n");
}

_wchar wstring::at( _u32 requestedIndex ) const
{
	// Decode internal buffer at position n
	_wchar codepoint = 0;
	decodeUTF8( this->buffer + this->getActualIndex( requestedIndex ) , codepoint , this->misFormated );
	
	return codepoint;
}

_uniquePtr<_wchar[]> wstring::toWideLiteral() const
{
	_wchar*	dest = new _wchar[ this->length() ];
	_wchar*	tempDest = dest;
	_char*	source = this->buffer;
	
	while( *source && !this->misFormated )
		source += this->decodeUTF8( source , *tempDest++ , this->misFormated );
	
	return std::move(dest);
}

optimized _s32 wstring::compare( const wstring& str ) const
{
	wstring_const_iterator	it1 = this->cbegin();
	wstring_const_iterator	it2 = str.cbegin();
	wstring_const_iterator	end1 = this->cend();
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
	}
	return *it1 == *it2;
}

_u32 wstring::find( _wchar ch , _u32 startPos ) const {
	for( wstring::iterator it = this->begin() + startPos ; it < this->end() ; it++ )
		if( *it == ch )
			return it - this->begin();
	return wstring::npos;
}

_u32 wstring::rfind( _wchar ch , _u32 startPos ) const {
	for( wstring::reverse_iterator it = this->rbegin() + startPos ; it < this->rend() ; it++ )
		if( *it == ch )
			return it - this->rbegin();
	return wstring::npos;
}

_u32 wstring::find_first_of( _wliteral str , _u32 startPos ) const {
	for( wstring::iterator it = this->begin() + startPos ; it < this->end() ; it++ )
	{
		_wliteral tmp = str;
		do{
			if( *it == *tmp )
				return it - this->begin();
		}while( *++tmp );
	}
	return wstring::npos;
}

_u32 wstring::find_last_of( _wliteral str , _u32 startPos ) const {
	for( wstring::reverse_iterator it = this->rbegin() + startPos ; it < this->rend() ; it++ )
	{
		_wliteral tmp = str;
		do{
			if( *it == *tmp )
				return it - this->rbegin();
		}while( *++tmp );
	}
	return wstring::npos;
}

_u32 wstring::find_first_not_of( _wliteral str , _u32 startPos ) const {
	for( wstring::iterator it = this->begin() + startPos ; it < this->end() ; it++ )
	{
		_wliteral tmp = str;
		do{
			if( *it != *tmp )
				return it - this->begin();
		}while( *++tmp );
	}
	return wstring::npos;
}

_u32 wstring::find_last_not_of( _wliteral str , _u32 startPos ) const {
	for( wstring::reverse_iterator it = this->rbegin() + startPos ; it < this->rend() ; it++ )
	{
		_wliteral tmp = str;
		do{
			if( *it == *tmp )
				return it - this->rbegin();
		}while( *++tmp );
	}
	return wstring::npos;
}