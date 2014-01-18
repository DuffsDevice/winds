#ifndef _WIN_T_TASKINFO_
#define _WIN_T_TASKINFO_

#include "_type/type.gadget.h"
#include "_gadget/gadget.image.h"

class _windowsTaskInfo;

using _taskInfoHandler = _callback<void(_windowsTaskInfo*)>;

class _windowsTaskInfo : private _imageGadget
{
	private:
		
		friend class _windowsTaskInfoController;
		
		static _callbackReturn  refreshHandler( _event );
		
		// List of active tasks
		static _list<_windowsTaskInfo*>	taskInfos;
		bool							validTaskInfo;
		
		// List of handlers that want to be notified about added or removed taskInfos
		static _list<_taskInfoHandler*>	taskInfoHandlers;
		
		// Notifies all handlers in the list of registered handlers
		void notifyTaskInfoHandlers(){
			for( auto handler : _windowsTaskInfo::taskInfoHandlers )
				(*handler)( this );
		}
	
	public:
		
		using _gadget::setUserEventHandler;
		using _gadget::removeUserEventHandler;
		
		//! Ctor
		_windowsTaskInfo( _bitmap icon , _style&& style = _style() );
		
		//! Dtor
		~_windowsTaskInfo();
		
		//! Set a new bitmap
		void setImage( _bitmap newOne ){
			newOne.resize( 6 , 6 );
			_imageGadget::setImage( move(newOne) );
		}
		
		//! Get the currently used image
		_constBitmap& getImage(){
			return _imageGadget::getImage();
		}
		
		//! Check if the _windowsTaskInfo is still valid
		bool isValid(){ return validTaskInfo; }
		
		//! Remove the supplied taskInfo - Handler
		static void removeTaskInfoHandler( const _taskInfoHandler& cb ){
			remove_if(
				_windowsTaskInfo::taskInfoHandlers.begin()
				, _windowsTaskInfo::taskInfoHandlers.end()
				, [&cb]( _taskInfoHandler* val )->bool{ return (cb == *val) == 1; }
			);
		}
		
		//! Register a handler that wants to be notified about added or removed _taskInfos
		static void addTaskInfoHandler( _paramAlloc<_taskInfoHandler> handler ){
			_windowsTaskInfo::taskInfoHandlers.push_back( handler.get() );
		}
};

#endif