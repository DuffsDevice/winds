#ifndef _WIN_D_ERRORWARNINGINFO_
#define _WIN_D_ERRORWARNINGINFO_

#include <_dialog/dialog.image.h>
#include <_type/type.bitmap.h>

//! Error - Dialog
class _errorDialog : public _imageDialog
{
	private:
		
		static _constBitmap image;
		
		// First playes a tone and then calls _imageDialog::executeInternal()
		void executeInternal();
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_errorDialog( wstring message , _optValue<wstring> okLabel = ignore , _optValue<wstring> otherLabel = ignore );
};

//! Warning - Dialog
class _warningDialog : public _imageDialog
{
	private:
		
		static _constBitmap image;
		
		// First playes a tone and then calls _imageDialog::executeInternal()
		void executeInternal();
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_warningDialog( wstring message , _optValue<wstring> okLabel = ignore , _optValue<wstring> otherLabel = ignore );
};

//! Info - Dialog
class _infoDialog : public _imageDialog
{
	private:
		
		static _constBitmap image;
		
		// First playes a tone and then calls _imageDialog::executeInternal()
		void executeInternal();
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_infoDialog( wstring message , _optValue<wstring> okLabel = ignore , _optValue<wstring> otherLabel = ignore );
};

#endif