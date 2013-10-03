#ifndef _WIN_T_RUNTIMEATTRIBUTES_
#define _WIN_T_RUNTIMEATTRIBUTES_

#include "_type/type.h"
#include "_type/type.bitmap.h"
#include "_type/type.user.h"


class _runtimeAttributes
{
	private:

		string			defaultFont;
		_u8				defaultFontSize;
		_vector<_char>	keyboardChar[2];
		_vector<string>	keyboardText[2];
		_bitmap			windowBar;
		_bitmap			windowBarBlurred;
		_pixel			selectForeground;
		_pixel			selectBackground;
		_pixel			defaultForeground;
		_pixel			defaultBackground;
		_user*			user;
		bool			magnifyKeyboardFocus;
		
		// Associative Directories: e.g. %WINDIR%
		_vector<_pair<string,string>> assocDirectories;
		
	public:
		
		//! Default Constructor
		_runtimeAttributes( _user* user );
		
		//! Default Destructor
		~_runtimeAttributes();
		
		//! Getters
		_u8		getDefaultFontSize() const { return defaultFontSize; }
		_pixel	getDefaultForeground() const { return this->defaultForeground; }
		_pixel	getDefaultBackground() const { return this->defaultBackground; }
		string	getDefaultFontName() const { return defaultFont; }
		_pixel	getSelectionForeground() const { return this->selectForeground; }
		_pixel	getSelectionBackground() const { return this->selectBackground; }
		bool	isKeyboardMagnifEnabled(){ return this->magnifyKeyboardFocus; }
		const _user*			getUser() const { return this->user; }
		const _vector<_char>& 	getKeyboardChars( _u8 layout ) const { return keyboardChar[layout]; }
		const _vector<string>& 	getKeyboardTexts( _u8 layout ) const { return keyboardText[layout]; }
		_constbitmap& 			getWindowsDesignActive() const { return this->windowBar; }
		_constbitmap& 			getWindowsDesignBlurred() const { return this->windowBarBlurred; }
		const _vector<_pair<string,string>>&	getAssociativeDirectories() const { return this->assocDirectories; }
		
		
		//! Setters
		void setUser( _user* user ){
			if( this->user )
				delete this->user;
			this->user = user;
		}
		void enableKeyboardMagnif(){ this->magnifyKeyboardFocus = true; }
		void disableKeyboardMagnif(){ this->magnifyKeyboardFocus = false; }
		void setDefaultFont( string name ){ this->defaultFont = name; }
		void setDefaultFontSize( _u8 size ){ this->defaultFontSize = size; }
};

#endif