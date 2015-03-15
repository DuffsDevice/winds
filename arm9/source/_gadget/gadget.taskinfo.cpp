#include <_gadget/gadget.taskinfo.h>
#include <_gadget/gadget.bubble.h>

_list<_taskInfo*>					_taskInfo::taskInfos;
_list<_uniquePtr<_taskInfoHandler>>	_taskInfo::taskInfoHandlers;

_taskInfo::_taskInfo( _bitmap icon , _style&& style ) :
	_imageGadget( ignore , ignore , move(icon) , 6 , 6 , move(style) )
	, validTaskInfo( true )
	, notificationBubble( nullptr )
{
	this->setType( _gadgetType::taskinfo );
	
	// Register this taskInfo instance in the global list of taskInfos
	_taskInfo::taskInfos.push_back( this );
	this->notifyTaskInfoHandlers();
}

_taskInfo::~_taskInfo()
{
	this->validTaskInfo = false;
	
	// Remove this taskInfo from the list of active taskInfo-instances
	_taskInfo::taskInfos.remove( this );
	
	// Update all gadget that want to get informed about my destruction
	this->notifyTaskInfoHandlers();
}

_callbackReturn _taskInfo::notificationClickHandler( _event event )
{
	if( this->notificationBubbleHandler ){
		(*this->notificationBubbleHandler)();
		this->notificationBubble = nullptr;
	}
	return handled;
}

void _taskInfo::displayNotification( wstring title , wstring content , _bitmap icon )
{
	this->notificationBubbleHandler = nullptr;
	this->notificationBubble = new _bubble( content.length() > 22 ? 85 : 60 , move(title) , move(content) , icon );
	this->notificationBubble->setUserEventHandler( onMouseClick , make_callback( this , &_taskInfo::notificationClickHandler ) );
	this->notificationBubble->show( this , false );
}