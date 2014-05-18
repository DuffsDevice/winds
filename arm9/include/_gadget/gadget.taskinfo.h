#ifndef _WIN_G_TASKINFO_
#define _WIN_G_TASKINFO_

#include "_type/type.gadget.h"
#include "_gadget/gadget.image.h"

class _taskInfo;
using _taskInfoHandler = _callback<void(_taskInfo*)>;

class _taskInfo : public _imageGadget
{
	private:
		
		friend class _taskInfoController;
		
		static _callbackReturn  refreshHandler( _event );
		
		// List of active tasks
		static _list<_taskInfo*>		taskInfos;
		bool							validTaskInfo;
		
		// List of handlers that want to be notified about added or removed taskInfos
		static _list<_uniquePtr<_taskInfoHandler>>	taskInfoHandlers;
		
		// Notifies all handlers in the list of registered handlers
		void notifyTaskInfoHandlers(){
			for( auto& handler : _taskInfo::taskInfoHandlers )
				(*handler)( this );
		}
	
	public:
		
		using _gadget::setUserEventHandler;
		using _gadget::removeUserEventHandler;
		
		//! Ctor
		_taskInfo( _bitmap icon , _style&& style = _style() );
		
		//! Dtor
		virtual ~_taskInfo();
		
		//! Set a new bitmap
		void setImage( _bitmap newOne ){
			newOne.resize( 6 , 6 );
			_imageGadget::setImage( move(newOne) );
		}
		
		//! Get the currently used image
		_constBitmap& getImage(){
			return _imageGadget::getImage();
		}
		
		//! Check if the _taskInfo is still valid
		bool isValid(){ return validTaskInfo; }
		
		//! Remove the supplied taskInfo - Handler
		static void removeTaskInfoHandler( const _taskInfoHandler& cb ){
			_taskInfo::taskInfoHandlers.remove_if(
				[&cb]( _uniquePtr<_taskInfoHandler>& val )->bool{
					return cb == *val;
				}
			);
		}
		
		//! Register a handler that wants to be notified about added or removed _taskInfos
		static void addTaskInfoHandler( _paramAlloc<_taskInfoHandler> handler ){
			_taskInfo::taskInfoHandlers.emplace_back( handler.get() );
		}
		
		//! Get a List of all registered taskInfos
		static _list<_taskInfo*> getTaskInfos(){
			return _taskInfo::taskInfos;
		}
};

#endif