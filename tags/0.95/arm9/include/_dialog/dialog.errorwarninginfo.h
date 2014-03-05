#ifndef _WIN_D_ERRORWARNINGINFO_
#define _WIN_D_ERRORWARNINGINFO_

#include "_dialog/dialog.image.h"
#include "_type/type.bitmap.h"

//! Error - Dialog
class _errorDialog : public _imageDialog
{
	private:
		
		static _constBitmap image;
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_errorDialog( string message , _optValue<string> okLabel = ignore , _optValue<string> otherLabel = ignore );
};

//! Warning - Dialog
class _warningDialog : public _imageDialog
{
	private:
		
		static _constBitmap image;
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_warningDialog( string message , _optValue<string> okLabel = ignore , _optValue<string> otherLabel = ignore );
};

//! Info - Dialog
class _infoDialog : public _imageDialog
{
	private:
		
		static _constBitmap image;
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_infoDialog( string message , _optValue<string> okLabel = ignore , _optValue<string> otherLabel = ignore );
};

#endif