#ifndef _WIN_T_RUNTIMEATTRIBUTES_
#define _WIN_T_RUNTIMEATTRIBUTES_

#include "type.h"
#include "type.bitmap.h"
using namespace std;

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
		_pixel			wallpaperBgColor;
		_u8 			maxClickCycles;
		_u8				maxDoubleClickCycles;
		_u8				maxDoubleClickArea;
		_u8 			minDragDistance;
		_u8				keyRepetitionDelay;
		_u8				keyRepetitionSpeed;
		_font*			defaultFont;
		string			startButtonText;
		vector<_char>	keyboardChar[2];
		vector<string>	keyboardText[2];
		_bitmap*		windowBar;
		_pixel			selectForeground;
		_pixel			selectBackground;
		_pixel			defaultForeground;
		_pixel			defaultBackground;
		_u8				fileObjectHeight;
		_u8				selectObjectHeight;
		bool 			showFileExtension;
		_bitmap*		userLogo;
		_bitmap*		userImage;
		string			userName;
		
		// Associative Directories: e.g. %WINDIR%
		vector<pair<string,string>> assocDirectories;
		
		
		//! Default Constructor
		_runtimeAttributes();
		
		//! Default Destructor
		~_runtimeAttributes();
};

#endif