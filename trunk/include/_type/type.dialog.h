#ifndef _WIN_T_DIALOG_
#define _WIN_T_DIALOG_

#include "_type/type.h"
#include "_type/type.gadget.h"

enum class _dialogResult : _u8{
	undefined,
	cancel,
	yes,
	no,
	apply
};

extern _toStr<_dialogResult> dialogResult2string;

class _dialog{
	
	protected:
		
		typedef _callback<void(_dialogResult)> CallbackType;
		
		CallbackType*	callback; // Will be called right after Exit
		
		_s32			intResult;
		string			strResult;
		bool			runs;
		
		virtual void executeInternal() = 0;
		virtual void cleanupInternal() = 0;
		
		// Calls the callback with the result of the dialog as parameter
		void callCallback( _dialogResult result ){
			if( this->callback )
				(*this->callback)( result );
		}
		
		void cleanup(){
			if( !this->runs )
				return;
			this->runs = false;
			this->cleanupInternal();
		}
	
	public:
	
		//! Ctor
		_dialog() : 
			callback( nullptr )
			, intResult( 0 )
			, runs( false )
		{ }		
		
		//! Execute the dialog
		void execute();
		
		//! terminate the dialog
		//! This equals clicking the 'cancel'-button or closing the window
		void terminate();
		
		//! getIntResult
		_int getIntResult(){ return this->intResult; }
		
		//! getStrResult
		string getStrResult(){ return this->strResult; }
		
		//! Set the onExit Func
		void setCallback( _paramAlloc<CallbackType> cb )
		{
			// Delete old
			if( this->callback )
				delete this->callback;
			
			// Set new callback
			this->callback = cb;
		}
		
		//! Removes the callback
		void deleteCallback();
		
		//! Check if the dialog finished already
		//! @note this also returns 'true' if the dialog hasn't even started
		bool isRunning(){ return this->runs; }
		
		//! Dtor
		virtual ~_dialog(){
			this->deleteCallback();
		}
};

#endif