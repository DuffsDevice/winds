#include "_gadget/gadget.windows.taskinfo.h"

_list<_windowsTaskInfo*>	_windowsTaskInfo::taskInfos;
_list<_taskInfoHandler*>	_windowsTaskInfo::taskInfoHandlers;

_windowsTaskInfo::_windowsTaskInfo( _bitmap icon , _style&& style ) :
	_imageGadget( ignore , ignore , move(icon) , 6 , 6 , move(style) )
	, validTaskInfo( true )
{
	_windowsTaskInfo::taskInfos.push_back( this );
	this->notifyTaskInfoHandlers();
}

_windowsTaskInfo::~_windowsTaskInfo(){
	validTaskInfo = false;
	_windowsTaskInfo::taskInfos.remove( this );
	this->notifyTaskInfoHandlers();
}