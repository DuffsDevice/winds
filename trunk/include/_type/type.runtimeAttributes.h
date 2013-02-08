#ifndef _WIN_T_RUNTIMEATTRIBUTES_
#define _WIN_T_RUNTIMEATTRIBUTES_

#include "_type/type.h"
#include "_type/type.bitmap.h"
#include "_type/type.user.h"


class _runtimeAttributes
{
	public:

		string			defaultFont;
		_u8				defaultFontSize;
		vector<_char>	keyboardChar[2];
		vector<string>	keyboardText[2];
		_bitmap			windowBar;
		_bitmap			windowBarBlurred;
		_pixel			selectForeground;
		_pixel			selectBackground;
		_pixel			defaultForeground;
		_pixel			defaultBackground;
		_user*			user;
		bool			magnifyKeyboardFocus;
		
		// Associative Directories: e.g. %WINDIR%
		vector<pair<string,string>> assocDirectories;
		
		//! Default Constructor
		_runtimeAttributes();
		
		//! Default Destructor
		~_runtimeAttributes();
};

#endif