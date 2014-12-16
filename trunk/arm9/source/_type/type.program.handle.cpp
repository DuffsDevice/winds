#include <_type/type.program.handle.h>
#include <_controller/controller.execution.h>

_program* _programHandle::prev()
{
	if( !this->ptr )
		return this->back();
	
	const _programList& list = _executionController::getRunningPrograms();
	
	_programList::const_reverse_iterator it = find_if(
		list.crbegin()
		, list.crend()
		, [this]( const _programList::value_type& prog )->bool{ return prog.get() == this->ptr; }
	);
	
	_programList::const_reverse_iterator end = list.crend();
	
	if( it != end && ++it != end )
		return this->ptr = it->get();
	
	return this->front();
}

_program* _programHandle::next()
{
	if( !this->ptr )
		return this->front();
	
	const _programList& list = _executionController::getRunningPrograms();
	
	_programList::const_iterator it = find_if(
		list.cbegin()
		, list.cend()
		, [this]( const _programList::value_type& prog )->bool{ return prog.get() == this->ptr; }
	);
	
	_programList::const_iterator end = list.cend();
	if( it != end && ++it != end )
		return this->ptr = it->get();
	
	return this->front();
}

_program* _programHandle::back()
{
	if( !_executionController::getRunningPrograms().empty() )
		return this->ptr = _executionController::getRunningPrograms().back();
	
	return this->ptr = nullptr;
}

_program* _programHandle::front()
{
	if( !_executionController::getRunningPrograms().empty() )
		return this->ptr = _executionController::getRunningPrograms().front();
	
	return this->ptr = nullptr;
}

_program* _programHandle::validate() const
{
	if( !this->ptr )
		return nullptr;
	
	if( _executionController::isExistent( this->ptr ) )
		return this->ptr;
	
	return this->ptr = nullptr;
}