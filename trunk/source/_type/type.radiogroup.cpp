#include "_type/type.radiogroup.h"

void _radiogroup::removeRadio( _radio* radio )
{
	auto end = radios.end();
	auto iter = radios.begin();
	
	// Cache the "end()" ->faster
	for( ; iter != end ; iter++ )
		if( iter->second == radio )
			break;
	
	if( iter != end )
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
		this->radios[assocValue] = radio;
	else
	{
		int i = -1;
		
		// Look for empty entry
		while( this->radios.count( ++i ) != 0 );
		
		// Push new entry
		this->radios[i] = radio;
	}
	
	radio->radiogroup = this;
}

void _radiogroup::enableRadio( _radio* radio )
{
	if( this->activeRadio )
		this->activeRadio->setIntValue( 0 );
	
	if( !radio )
		return;
	
	this->activeRadio = radio;
	this->activeRadio->setIntValue( 1 );
}

void _radiogroup::setIntValue( _s32 value )
{
	_map<_s32,_radio*>::iterator iter = this->radios.find( value );
	
	if( this->activeRadio )
		this->activeRadio->setIntValue( 0 );
	
	// Check for validity
	if( iter == this->radios.end() || !(iter->second) || value )
		return;
	
	this->activeRadio = iter->second;
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