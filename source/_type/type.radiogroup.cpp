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
	radio->radiogroup = nullptr;
}

void _radiogroup::addRadio( _radio* radio , _s32 assocValue )
{
	if( !radio )
		return;
	
	if( radio->radiogroup )
		radio->radiogroup->removeRadio( radio );
	
	if( assocValue != -1 )
		// Cache the "end()" ->faster
		this->radios[assocValue] = radio;
	else
	{
		int i = -1;
		// Look for ne entry
		while( this->radios.count( ++i ) != 0 );
		
		// Push new entry
		this->radios[i] = radio;
	}
	
	radio->radiogroup = this;
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
	
	if( this->activeRadio )
		this->activeRadio->setIntValue( 0 );
	
	if( !rd )
		return;
	
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