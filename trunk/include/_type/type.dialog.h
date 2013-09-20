#ifndef _WIN_T_DIALOG_
#define _WIN_T_DIALOG_

#include "_type/type.h"
#include "_type/type.gadget.h"

enum class _dialogResult : _u8
{
	undefined,
	cancel,
	yes,
	no,
	apply
};

class _dialog{
	
	protected:
		
		_callback<void(_dialogResult)>* callback; // Will be called right after Exit
		
		_s32						intResult;
		string						strResult;
		bool						runs;
		
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
		template<typename T>
		void setCallback( T&& cb )
		{
			typedef typename std::remove_reference<T>::type T2;
			typedef typename T2::_callback def;
			
			// Delete old
			if( this->callback )
				delete this->callback;
			
			// Set new callback
			this->callback = new T2( move(cb) );
		}
		
		//! Removes the callback
		void deleteCallback();
		
		//! Check if the dialog finished already
		//! @note this also returns 'true' if the dialog hasn't even started
		bool isRunning(){ return this->runs; }
		
		//! Dtor
		virtual ~_dialog(){
			this->deleteCallback();
			this->terminate();
		}
};

#endif