#ifndef _WIN_T_RUNTIMEATTRIBUTES_
#define _WIN_T_RUNTIMEATTRIBUTES_

#include "type.bitmap.h"
#include "type.h"
#include <string>
using namespace std;

enum _wallpaperViewType{
	WALLPAPER_ADJUST,
	WALLPAPER_SMALL,
	WALLPAPER_ORIG
};


class _runtimeAttributes{
	public:
		_bitmap*		wallpaper;
		_wallpaperViewType wallpaperView;
		_pixel			wallpaperBgColor;
		_u8 			maxClickCycles;
		_u8				maxDoubleClickCycles;
		_u8				maxDoubleClickArea;
		_u8 			minDragDistance;
		_font*			defaultFont;
		string			startButtonText;
		vector<_char>	keyboardChar[2];
		vector<string>	keyboardText[2];
		_bitmap*		windowBar;
		_u8				fileObjectHeight;
		bool 			showFileExtension;
		
		// Associative Directories: e.g. %WINDIR%
		vector<pair<string,string>> assocDirectories;
		
		
		//! Default Constructor
		_runtimeAttributes();
};

#endif