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
		_color			focusForeground;
		_color			focusBackground;
		_color			selectForeground;
		_color			selectBackground;
		_color			itemForeground;
		_color			itemBackground;
		_color			controlBackground;
		_color			controlForeground;
		bool			magnifyKeyboardFocus;
		
		// Associative Directories: e.g. %WINDIR%
		_assocVector<string,string> assocDirectories;
		
		void updateUserDirectory();
		
	public:
		
		//! Default Constructor
		_runtimeAttributes( flex_ptr<_user> user );
		
		//! Default Destructor
		~_runtimeAttributes();
		
		//! Getters
		_u8		getDefaultFontSize() const { return defaultFontSize; }
		_color	getItemForeground( bool focused = false , bool selected = false ) const {
			if( focused )
				return this->focusForeground;
			return selected ? this->selectForeground : this->itemForeground;
		}
		_color	getItemBackground( bool focused = false , bool selected = false ) const {
			if( focused )
				return this->focusBackground;
			return selected ? this->selectBackground : this->itemBackground;
		}
		string	getDefaultFontName() const { return defaultFont; }
		_color	getControlForeground() const { return this->controlForeground; }
		_color	getControlBackground() const { return this->controlBackground; }
		bool	isKeyboardMagnifEnabled() const { return this->magnifyKeyboardFocus; }
		const _user&			getUser() const { return *this->user; }
		const _vector<_char>& 	getKeyboardChars( _u8 layout ) const { return keyboardChar[layout]; }
		const _vector<string>& 	getKeyboardTexts( _u8 layout ) const { return keyboardText[layout]; }
		_constBitmap& 			getWindowsDesignActive() const { return this->windowBar; }
		_constBitmap& 			getWindowsDesignBlurred() const { return this->windowBarBlurred; }
		const _assocVector
			<string,string>&	getAssociativeDirectories() const { return this->assocDirectories; }
		
		
		//! Setters
		void setUser( flex_ptr<_user> user ){
			this->user = move(user);
			this->updateUserDirectory();
		}
		void enableKeyboardMagnif(){ this->magnifyKeyboardFocus = true; }
		void disableKeyboardMagnif(){ this->magnifyKeyboardFocus = false; }
		void setDefaultFont( string name ){ this->defaultFont = name; }
		void setDefaultFontSize( _u8 size ){ this->defaultFontSize = size; }
};

#endif