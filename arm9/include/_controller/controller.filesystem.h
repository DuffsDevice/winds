#ifndef _WIN_C_FILESYSTEM_
#define _WIN_C_FILESYSTEM_

#include <_type/type.h>
#include <_type/type.direntry.h>
#include <_type/type.controller.h>

using _assocDirList = _assocVector<string,string>;

class _filesystemController : public _controller
{
	private:
		
		//! Flag indicating if the FAT32-System is ready to be used
		static bool				fatReady;
		
		//! Associative Directories: e.g. %WINDIR%
		static _assocDirList	assocDirs;
		
	public:
		
		//! Routine to initialize the event buffers
		static bool init();
		
		//! Check if the Fat Controller is ready to be used
		static bool isFatReady(){ return fatReady; }
		
		//! Get List of connected hard drives
		static _vector<string> getDrives();
		
		//! Get statistics about a connected hard drive
		static bool getDriveStats( string driveName , _driveStats& dest );
		
		//! Replace associated filename-patterns
		static string replaceAssocDirs( string path ){
			replaceAssocDirsInternal(path);
			return move(path);
		}
		static void replaceAssocDirsInternal( string& path );
		
		//! Sets the directory of the currently registered user
		static void setCurrentUserDir( string path );
};

#endif