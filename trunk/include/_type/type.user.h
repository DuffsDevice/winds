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
		
		//! Methods to receive create a users logo
		static _bitmap getUserLogoFromImage( const _bitmap& image );
		static _bitmap getUserImage( string path );
		
		//! Fast Int Attributes
		_u16 	mDD; // minDragDistance
		_u16	mCC; // maxClickCycles
		_u8		mDC; // maxDoubleClickCycles
		_u8		mDA; // maxDoubleClickArea
		_u16	kRD; // keyRepetitionDelay (frames until repetition)
		_u16	kRS; // keyRepetitionSpeed (frames between each click)
		_u8		fOH; // fileObjectHeight
		_u8		sOH; // selectObjectHeight
		_u8		sFE; // showFileExtension
		_pixel	sBTC;// startButtonTextColor
		_pixel	dTC; // desktopColor
		bool	mKF; // magnifyKeyboardFocus
		bool	isAdmin; // whether the user has admin rights
		
		//! Ctor
		_user( string username );
		
		//! Dtor
		~_user();
		
		//! Get the user's username
		string getUsername() const { return _registry::readIndex( "_global_" , "userName" ); }
		
		//! Get the foldername of the user ( it's directory in "%USERS%/" )
		string getFoldername() const { return this->folderName; }
		
		//! Get User-Logo
		const _bitmap& getLogo() const { return this->userLogo; }
		
		//! Get the wallpaper
		const _bitmap& getWallpaper() const { return this->wallpaper; }
		
		//! For view type of wallpaper
		_wallpaperViewType getWallpaperViewType() const { return this->wallpaperView; }
		
		//! Match a supplied raw string against the password
		bool checkPassword( string pw ) const ;
		
		//! Check if the user has a password
		bool hasPassword() const ;
		
		//! Set a users password
		void setPassword( string pw );
		
		//! Set a users username
		void setUsername( string name ){ _registry::writeIndex( "_global_" , "userName" , name ); }
		
		//! Remove the user (equals unlinking its directory)
		void remove(){ _registry::unlink(); }
		
		//! Get a string value from the registry parsed as number
		_int getIntAttr( string idx ) const { return _registry::readIndexInt( "_global_" , idx ); }
		
		//! Read a string value from the registry
		string getStrAttr( string idx ) const { return _registry::readIndex( "_global_" , idx ); }
		
		//! Saves the data of this user class to a folder
		void createAs( string folderName );
		
		//! Set the users icon (string)
		void setUsericon( string icon , bool builtIn = false ){ _registry::writeIndex( "_global_" , "userLogo" , icon ); }
};

#endif