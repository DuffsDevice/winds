#include "_type/type.radiogroup.h"

void _radiogroup::removeRadio( _radio* radio )
{	
	auto iter = radios.begin();
	
	// Cache the "end()" ->faster
	for( auto end = radios.end() ; iter != end ; iter++ )
		if( iter->second == radio )
			break;
	
	if( iter != this->radios.end() )
		this->radios.erase( iter );
}

_s32 _radiogroup::addRadio( _radio* radio )
{	
	// Cache the "end()" ->faster
	auto end = radios.end();
	
	int i = 0;
	
	// The "IntValue" is basically the index of the radio in the list
	for( auto iter = radios.begin() ; iter != end ; iter++ , i++ );
	
	this->radios[i] = radio;
	
	return i;
}

void _radiogroup::enableRadio( _radio* radio )
{
	auto iter = radios.begin();
	
	// Cache the "end()" ->faster
	for( auto end = radios.end() ; iter != end ; iter++ )
		if( iter->second == radio )
			break;
	
	if( iter != this->radios.end() )
		this->setIntValue( iter->first );
}

void _radiogroup::setIntValue( _s32 value )
{
	_radio* rd = this->radios[value];
	
	if( !rd )
		return;
	
	if( this->activeRadio )
		this->activeRadio->setIntValue( 0 );
	
	this->activeRadio = rd;
	this->activeRadio->setIntValue( 1 );
}

// The "IntValue" is basically the index of the radio in the list
_s32 _radiogroup::getIntValue()
{
	if( !this->activeRadio )
		return -1;
	
	auto iter = radios.begin();
	
	// Cache the "end()" ->faster
	for( auto end = radios.end() ; iter != end ; iter++ )
		if( iter->second == this->activeRadio )
			return iter->first;
	
	return -1;
}