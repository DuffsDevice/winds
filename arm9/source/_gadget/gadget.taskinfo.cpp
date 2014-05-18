#include "_gadget/gadget.taskinfo.h"

_list<_taskInfo*>					_taskInfo::taskInfos;
_list<_uniquePtr<_taskInfoHandler>>	_taskInfo::taskInfoHandlers;

_taskInfo::_taskInfo( _bitmap icon , _style&& style ) :
	_imageGadget( ignore , ignore , move(icon) , 6 , 6 , move(style) )
	, validTaskInfo( true )
{
	// Register this taskInfo instance in the global list of taskInfos
	_taskInfo::taskInfos.push_back( this );
	this->notifyTaskInfoHandlers();
}

_taskInfo::~_taskInfo()
{
	validTaskInfo = false;
	
	// Remove this taskInfo from the list of active taskInfo-instances
	_taskInfo::taskInfos.remove( this );
	
	// Update all gadget that want to get informed about my destruction
	this->notifyTaskInfoHandlers();
}