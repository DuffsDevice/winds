#ifndef _WIN_T_RUNTIMEATTRIBUTES_
#define _WIN_T_RUNTIMEATTRIBUTES_

#include "_type/type.h"
#include "_type/type.bitmap.h"
#include "_type/type.user.h"
#include "_type/type.flexptr.h"


class _runtimeAttributes
{
	private:

		string			defaultFont;
		_u8				defaultFontSize;
		_vector<_char>	keyboardChar[2];
		_vector<string>	keyboardText[2];
		flex_ptr<_user>	user;
		_bitmap			windowBar;
		_bitmap			windowBarBlurred;
		_pixel			highlightForeground;
		_pixel			highlightBackground;
		_pixel			itemForeground;
		_pixel			itemBackground;
		_pixel			controlBackground;
		_pixel			controlForeground;
		bool			magnifyKeyboardFocus;
		
		// Associative Directories: e.g. %WINDIR%
		_vector<_pair<string,string>> assocDirectories;
		
	public:
		
		//! Default Constructor
		_runtimeAttributes( _paramAlloc<_user> user );
		
		//! Default Destructor
		~_runtimeAttributes();
		
		//! Getters
		_u8		getDefaultFontSize() const { return defaultFontSize; }
		_pixel	getItemForeground( bool highlighted = false ) const { return highlighted ? this->highlightForeground : this->itemForeground; }
		_pixel	getItemBackground( bool highlighted = false ) const { return highlighted ? this->highlightBackground : this->itemBackground; }
		string	getDefaultFontName() const { return defaultFont; }
		_pixel	getControlForeground() const { return this->controlForeground; }
		_pixel	getControlBackground() const { return this->controlBackground; }
		bool	isKeyboardMagnifEnabled(){ return this->magnifyKeyboardFocus; }
		const _user&			getUser() const { return *this->user; }
		const _vector<_char>& 	getKeyboardChars( _u8 layout ) const { return keyboardChar[layout]; }
		const _vector<string>& 	getKeyboardTexts( _u8 layout ) const { return keyboardText[layout]; }
		_constbitmap& 			getWindowsDesignActive() const { return this->windowBar; }
		_constbitmap& 			getWindowsDesignBlurred() const { return this->windowBarBlurred; }
		const _vector<_pair<string,string>>&	getAssociativeDirectories() const { return this->assocDirectories; }
		
		
		//! Setters
		void setUser( _paramAlloc<_user> user ){
			this->user = user.get();
		}
		void enableKeyboardMagnif(){ this->magnifyKeyboardFocus = true; }
		void disableKeyboardMagnif(){ this->magnifyKeyboardFocus = false; }
		void setDefaultFont( string name ){ this->defaultFont = name; }
		void setDefaultFontSize( _u8 size ){ this->defaultFontSize = size; }
};

#endif