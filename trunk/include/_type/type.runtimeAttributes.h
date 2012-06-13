#ifndef _WIN_T_RUNTIMEATTRIBUTES_
#define _WIN_T_RUNTIMEATTRIBUTES_

#include "_type/type.h"
#include "_type/type.bitmap.h"
#include "_type/type.user.h"

enum _wallpaperViewType{
	WALLPAPER_ADJUST,
	WALLPAPER_SMALL,
	WALLPAPER_ORIG
};


class _runtimeAttributes
{
	public:

		_bitmap*		wallpaper;
		_wallpaperViewType wallpaperView;
		_font*			defaultFont;
		vector<_char>	keyboardChar[2];
		vector<string>	keyboardText[2];
		_bitmap*		windowBar;
		_pixel			selectForeground;
		_pixel			selectBackground;
		_pixel			defaultForeground;
		_pixel			defaultBackground;
		_user*			user;
		
		// Associative Directories: e.g. %WINDIR%
		vector<pair<string,string>> assocDirectories;
		
		
		//! Default Constructor
		_runtimeAttributes();
		
		//! Default Destructor
		~_runtimeAttributes();
};

#endif