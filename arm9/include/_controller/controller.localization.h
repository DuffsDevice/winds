#ifndef _WIN_C_LOCALIZATION_
#define _WIN_C_LOCALIZATION_

#include "_type/type.h"
#include "_type/type.controller.h"
#include "_type/type.language.h"
#include "_type/type.uniqueptr.h"
#include "_type/type.ini.h"
#include "_type/type.key.h"

class _localizationController : public _controller
{
	private:
		
		static _language		currentLanguage;
		static _uniquePtr<_ini>	localizationTextTable;
		static _uniquePtr<_ini>	localizationMonthTable;
		
		//! Layouts for the keyboard
		static _vector<_char>	keyboardKey[2];
		static _vector<string>	keyboardText[2];
		
		//! Displayed as replacement if a language-specific term
		//! is not available in the currently selected language
		static string			invalidStringSignature;
		
	public:
		
		//! Inits the localization system
		static bool init();
		
		//! Cleans everything up
		static void end();
		
		//! Get string in the current language
		static const string& getBuiltInString( const string& name );
		
		/**
		 * Get month name in the current language
		 * @note Numbers range from 0 (Jan) to 11 (Dec)
		 */
		static const string& getMonth( _u8 month );
		
		//! Set Language
		static void setLanguage( _language lang ){ currentLanguage = move(lang); }
		
		//! Get the current Langauge
		static _language getLanguage(){ return currentLanguage; }
		
		//! Get the Character values for eack keyboard button
		static const _vector<_char>& 	getKeyboardKeys( _u8 layout ){ return keyboardKey[layout]; }
		
		//! Get the displayed text for each button
		static const _vector<string>& 	getKeyboardTexts( _u8 layout ){ return keyboardText[layout]; }
};

#endif