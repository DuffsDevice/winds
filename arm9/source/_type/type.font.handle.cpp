#include <_type/type.font.handle.h>
#include <_controller/controller.font.h>

_fontHandle _fontHandle::next()
{
	if( !this->ptr )
		return this->front();
	
	const _fontList& list = _fontController::getRegisteredFonts();
	
	_fontList::const_iterator it = find_if(
		list.cbegin()
		, list.cend()
		, [this]( const _fontList::value_type& prog )->bool{ return prog == this->ptr; }
	);
	
	_fontList::const_iterator end = list.cend();
	if( it != end && ++it != end )
		return this->ptr = *it;
	return this->front();
}

_fontHandle _fontHandle::prev()
{
	if( !this->ptr )
		return this->back();
	
	const _fontList& list = _fontController::getRegisteredFonts();
	
	_fontList::const_reverse_iterator it = find_if(
		list.crbegin()
		, list.crend()
		, [this]( const _fontList::value_type& prog )->bool{ return prog == this->ptr; }
	);
	
	_fontList::const_reverse_iterator end = list.crend();
	if( it != end && ++it != end )
		return this->ptr = *it;
	
	return this->front();
}

_fontHandle _fontHandle::front()
{
	if( !_fontController::getRegisteredFonts().empty() )
		return this->ptr = _fontController::getRegisteredFonts().front();
	
	return this->ptr = nullptr;
}

_fontHandle _fontHandle::back()
{
	if( !_fontController::getRegisteredFonts().empty() )
		return this->ptr = _fontController::getRegisteredFonts().back();
	
	return this->ptr = nullptr;
}

_fontPtr _fontHandle::validate() const
{
	if( this->ptr )
	{
		// Check if font ptr is still valid
		if( _fontController::isExistent( this->ptr ) )
			return this->ptr;
		
		// Reset to null
		this->ptr = nullptr;
	}
	
	// Return fallback font
	return _fontController::getStandardFont();
}