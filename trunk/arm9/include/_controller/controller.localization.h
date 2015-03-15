#ifndef _WIN_C_LOCALIZATION_
#define _WIN_C_LOCALIZATION_

#include <_type/type.h>
#include <_type/type.controller.h>
#include <_type/type.language.h>
#include <_type/type.uniqueptr.h>
#include <_type/type.ini.h>
#include <_type/type.key.h>
#include <_library/library.syllable.h>

class _localizationController : public _controller
{
	private:
		
		static _language		currentLanguage;
		static _uniquePtr<_ini>	localizationTextTable;
		static _uniquePtr<_ini>	localizationMonthTable;
		
		//! Layouts for the keyboard
		static _vector<_wchar>	keyboardKey[2];
		static _vector<wstring>	keyboardText[2];
		
		//! Displayed as replacement if a language-specific term
		//! is not available in the currently selected language
		static wstring			invalidStringSignature;
		
		//! Object that hyphenates a given text
		static TinyHyphenator	hyphenatorInstance;
		
	public:
		
		//! Inits the localization system
		static bool init();
		
		//! Cleans everything up
		static void end();
		
		//! Get string in the current language
		static const wstring& getBuiltInString( const string& name );
		
		/**
		 * Get month name in the current language
		 * @note Numbers range from 0 (Jan) to 11 (Dec)
		 */
		static const wstring& getMonth( _u8 month );
		
		//! Set Language
		static void setLanguage( _language lang ){ currentLanguage = move(lang); }
		
		//! Get the current Langauge
		static _language getLanguage(){ return currentLanguage; }
		
		//! Get the Character values for eack keyboard button
		static const _vector<_wchar>& 	getKeyboardKeys( _u8 layout ){ return keyboardKey[layout]; }
		
		//! Get the displayed text for each button
		static const _vector<wstring>& 	getKeyboardTexts( _u8 layout ){ return keyboardText[layout]; }
		
		//! Get the hyphenator instance
		static const TinyHyphenator&	getHyphenator(){ return hyphenatorInstance; }
};

#endif