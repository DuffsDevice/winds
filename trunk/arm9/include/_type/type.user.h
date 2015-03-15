#ifndef _WIN_T_USER_
#define _WIN_T_USER_

#include <_type/type.ini.file.h>
#include <_type/type.bitmap.h>

enum class _wallpaperViewType{
	original = 0,
	stretch = 1,
	cover = 2,
	contain = 3,
	pattern = 4
};

using _startMenuList = _vector<_pair<string,wstring>>;

class _user : public _iniFile
{
	private:
		
		_color				desktopColor;
		bool				hasAdminRights;
		_bitmap				userLogo;
		_bitmap				wallpaper;
		_wallpaperViewType	wallpaperViewType;
		_startMenuList		startMenuEntries;
		
		//! Methods to create a users logo
		static _bitmap getLogoFromImage( _constBitmap& image );
		static _bitmap getImage( string path );
		
		static _array<_literal,24>		standardLogos;
	
	public:
		
		//! Ctor
		_user( string pathToIni );
		
		//! Dtor
		~_user();
		
		//! Get/Set the Background color of the desktop
		_color			getDesktopColor() const { return desktopColor; }
		void			setDesktopColor( _color col ){ desktopColor = col; }
		
		//! Get/Set whether the user has administrator privileges
		bool			isAdmin() const { return hasAdminRights; }
		void			setAdmin( bool value ){ hasAdminRights = value; }
		
		//! Get/Set the Logo of the user
		_constBitmap&	getLogo() const { return userLogo; }
		void			setLogo( string path );
		void			setLogo( _u8 standardLogoNumber );
		
		//! Get the User's Wallpaper
		_constBitmap&	getWallpaper() const { return wallpaper; }
		void			setWallpaper( string path );
		
		//! Get/Set the Users Name
		virtual const wstring&	getUserName() const ;
		void			setUserName( wstring );
		
		//! Get/Set the path to the home folder
		string			getHomeFolder() const ;
		void			setHomeFolder( string path );
		
		//! Match a supplied raw string against the password
		bool			checkPassword( const string& pw ) const ;
		
		//! Check if the user has a password
		bool			hasPassword() const ;
		
		//! Set a users password
		void			setPassword( const string& pw );
		
		//! Get/Set the Wallpaper View Type of the User
		void					setWallpaperViewType( _wallpaperViewType );
		_wallpaperViewType		getWallpaperViewType() const { return wallpaperViewType; }
		
		//! Get the Users StartMenu Entries
		const _startMenuList&	getStartMenuEntries() const { return startMenuEntries; }
		
		//! Get a specific default Logo
		static _bitmap getStandardLogo( _u8 logoNumber );
};

#endif