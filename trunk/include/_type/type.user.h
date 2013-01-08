#ifndef _WIN_T_USER_
#define _WIN_T_USER_

#include "_type/type.h"
#include "_type/type.registry.h"

enum _wallpaperViewType{
	WALLPAPER_ORIG = 0,
	WALLPAPER_ADJUST = 1,
	WALLPAPER_PATTERN = 2,
};

class _user : public _registry
{
	private:
		
		string				folderName;
		_bitmap				userLogo;
		_bitmap				wallpaper;
		_wallpaperViewType	wallpaperView;
	
	public:
		
		static _bitmap getUserLogoFromImage( const _bitmap& image );
		static _bitmap getUserImage( string path );
		
		// Fast Int Attributes
		_u32 	mDD, mCC, mDC, mDA, kRD, kRS, fOH, sOH, sFE, sBTC, dTC;
		
		//! Ctor
		_user( string username );
		
		//! Dtor
		~_user();
		
		string getUsername();
		
		//! Get User-Logo
		const _bitmap& getLogo() const { return this->userLogo; }
		
		//! Get the wallpaper
		const _bitmap& getWallpaper() const { return this->wallpaper; }
		
		//! For view type of wallpaper
		_wallpaperViewType getWallpaperViewType() const { return this->wallpaperView; }
		
		bool checkPassword( string pw );
		
		bool hasPassword();
		
		void setPassword( string pw );
		
		void setUsername( string un );
		
		void remove();
		
		_s32 getIntAttr( string idx );
		
		string getStrAttr( string idx );
};

#endif
	