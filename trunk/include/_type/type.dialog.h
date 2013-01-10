#ifndef _WIN_T_DIALOG_
#define _WIN_T_DIALOG_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_resource/BMP_AlertImages.h"

enum class _dialogType : _u8
{
	none,
	yesNo,
	enterText,
	image
};

class _dialog{
	
	private:
		
		_dialogType			dType;
		
		void**				data; // Temporary data
		
		const _callback* 	exitFunc; // Will be called right after Exit
		
		_s32				intResult;
		string				strResult;
		
		bool				finished;
		
		void 				internalExecution();
		void 				internalCleanup();
		
		_callbackReturn 	internalEventHandler( _event e );
	
	public:
	
		_dialog() : 
			dType( _dialogType::none )
			, data( nullptr )
			, exitFunc( nullptr )
			, intResult( 1 << 31 )
			, finished( true )
		{ }
		
		//! For Yes/No - Dialog
		static void yesNoDialog( _dialog& d , string msg , string windowLbl , string yesLbl = "Yes" , string noLbl = "No" );
		
		//! For text prompt
		static void enterTextDialog( _dialog& d , string msg , string windowLbl , string okLbl = "OK" , string cancelLbl = "Cancel" );
		
		//! For a Popup with an image
		static void imageDialog( _dialog& d , string msg , string windowLbl , const _bitmap& bmp , string okLbl = "OK" , string otherLbl = "" );
		
		//! For an Error-Popup
		static void errorDialog( _dialog& d , string msg , string windowLbl , string okLbl = "Dismiss" , string otherLbl = "" )
		{
			imageDialog( d , msg , windowLbl , BMP_AlertImageError() , okLbl , otherLbl );
		}
		//! For an Info-Dialog
		static void infoDialog( _dialog& d , string msg , string windowLbl , string okLbl = "OK" , string otherLbl = "" )
		{
			imageDialog( d , msg , windowLbl , BMP_AlertImageInfo() , okLbl , otherLbl );
		}
		//! For a Warning Popup
		static void warningDialog( _dialog& d , string msg , string windowLbl , string okLbl = "Dismiss" , string otherLbl = "" )
		{
			imageDialog( d , msg , windowLbl , BMP_AlertImageWarning() , okLbl , otherLbl );
		}
		
		
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
		void onExit( const _callback* cb ){ this->exitFunc = cb; }
		
		//! Check if the dialog finished already
		//! @note this also returns 'true' if the dialog hasn't even started
		bool hasFinished(){ return this->finished; }
		
		//! Check if the dialog was closed or the 'Cancel'-button was clicked
		bool wasAborted(){ return this->intResult == -1; }
		
		//! Dtor
		~_dialog();
};

#endif