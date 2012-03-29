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
		int 			minClickCycles;
		int 			minDragDistance;
		_font*			defaultFont;
		string			startButtonText;
		vector<_char>	keyboardChar[2];
		vector<string>	keyboardText[2];
		_bitmap*		windowBar;
		bool 			showFileExtension;
		
		
		//! Default Constructor
		_runtimeAttributes();
};

extern _runtimeAttributes _defaultRuntimeAttributes_;

#endif