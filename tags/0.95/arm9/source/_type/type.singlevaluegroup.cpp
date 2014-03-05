#include "_type/type.singlevaluegroup.h"

template<class _selector>
void _singleValueGroup<_selector>::removeSelector( _selector* radio )
{
	auto end = registeredSelectors.end();
	auto iter = registeredSelectors.begin();
	
	// Cache the "end()" ->faster
	for( ; iter != end ; iter++ )
		if( iter->second == radio )
			break;
	
	if( iter != end )
		this->registeredSelectors.erase( iter );
	
	radio->group = nullptr;
}


template<class _selector>
void _singleValueGroup<_selector>::addSelector( _selector* radio , _s32 assocValue )
{
	if( !radio )
		return;
	
	if( radio->group )
		radio->group->removeSelector( radio );
	
	if( assocValue != -1 )
		this->registeredSelectors[assocValue] = radio;
	else
	{
		int i = -1;
		
		// Look for empty entry
		while( this->registeredSelectors.count( ++i ) != 0 );
		
		// Push new entry
		this->registeredSelectors[i] = radio;
	}
	
	radio->group = this;
}


template<class _selector>
void _singleValueGroup<_selector>::enableSelector( _selector* radio )
{
	if( this->activeSelector )
		this->activeSelector->setIntValue( 0 );
	
	if( !radio )
		return;
	
	this->activeSelector = radio;
	this->activeSelector->setIntValue( 1 );
}

template<class _selector>
void _singleValueGroup<_selector>::enableNext()
{
	if( this->activeSelector )
		this->activeSelector->setIntValue( 0 );
	
	auto iter = registeredSelectors.begin();
	
	// Cache the "end()" ->faster
	for( auto end = registeredSelectors.end() ; iter != end ; iter++ )
		if( iter->second == this->activeSelector )
			break;
	
	if( iter != registeredSelectors.end() && ++iter != registeredSelectors.end() )
		this->activeSelector = iter->second;
	else
		this->activeSelector = this->registeredSelectors.begin()->second;
	
	if( this->activeSelector )
		this->activeSelector->setIntValue( 1 );
}

template<class _selector>
void _singleValueGroup<_selector>::enablePrev()
{	
	if( this->activeSelector )
		this->activeSelector->setIntValue( 0 );
	
	auto iter = registeredSelectors.rbegin();
	
	// Cache the "end()" ->faster
	for( auto end = registeredSelectors.rend() ; iter != end ; iter++ )
		if( iter->second == this->activeSelector )
			break;
	
	if( iter != registeredSelectors.rend() && ++iter != registeredSelectors.rend() )
		this->activeSelector = iter->second;
	else
		this->activeSelector = this->registeredSelectors.rbegin()->second;
	
	if( this->activeSelector )
		this->activeSelector->setIntValue( 1 );
}

template<class _selector>
void _singleValueGroup<_selector>::setIntValue( _s32 value )
{
	typename _map<_s32,_selector*>::iterator iter = this->registeredSelectors.find( value );
	
	if( this->activeSelector )
		this->activeSelector->setIntValue( 0 );
	
	// Check for validity
	if( iter == this->registeredSelectors.end() || !(iter->second) || value < 0 )
		return;
	
	this->activeSelector = iter->second;
	this->activeSelector->setIntValue( 1 );
}


// The "IntValue" is basically the index of the radio in the list
template<class _selector>
_s32 _singleValueGroup<_selector>::getIntValue()
{
	if( !this->activeSelector )
		return -1;
	
	auto iter = registeredSelectors.begin();
	
	// Cache the "end()" ->faster
	for( auto end = registeredSelectors.end() ; iter != end ; iter++ )
		if( iter->second == this->activeSelector )
			return iter->first;
	
	return -1;
}


#include "_gadget/gadget.checkbox.h"
#include "_gadget/gadget.button.sticky.h"
#include "_gadget/gadget.radio.h"


// Create Specializations
template class _singleValueGroup<_radio>;
template class _singleValueGroup<_stickyButton>;