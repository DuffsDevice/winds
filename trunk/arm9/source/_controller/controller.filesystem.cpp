#include <_controller/controller.filesystem.h>

extern "C"{
	#include <_library/_fat/fatfile.h>
	bool sdio_IsInserted(); // libnds method
}

namespace unistd{
	#include <unistd.h>
}

void _filesystemController::replaceAssocDirsInternal( string& path )
{
	for( const _pair<string,string>& assoc : assocDirs )
	{
		size_t pos = path.find( assoc.first );
		if ( pos != string::npos )
			path.replace( pos , assoc.first.length() , assoc.second );
	}
}

bool _filesystemController::init()
{
	assocDirs = {
		{ "%WINDIR%" , "/windows" }
		, { "%SYSTEM%" , "/windows/system" }
		, { "%APPDATA%" , "/windows/appdata" }
		, { "%USERS%" , "/windows/users" }
	};
	
	//! libFat instantiation
	fatReady = fatInitDefault();
	
	return true;
}

_vector<string> _filesystemController::getDrives()
{
	_vector<string> vec = { "fat:" };
	if( sdio_IsInserted()  )
		vec.push_back("fat2:");
	return vec;
}

void _filesystemController::setCurrentUserDir( string path ){
	if( path.empty() )
	{
		auto iter = assocDirs.find("%CURRENT_USER%");
		if( iter != assocDirs.end() )
			assocDirs.erase( iter );
	}
	else
		assocDirs["%CURRENT_USER%"] = path;
}

bool _filesystemController::getDriveStats( string driveName , _driveStats& dest )
{
	if( driveName.back() != ':' )
		driveName += ':';
	
	if( driveName[3] == '0' || driveName[3] == ':' )
		dest.removable = false;
	else
		dest.removable = true;
	
	/* Any file on the filesystem in question */
	struct statvfs buf;
	if( !statvfs( driveName.c_str() , &buf ) )
	{
		_u64 blockSize, blocks, freeBlocks;
		
		blockSize = buf.f_bsize;
		blocks = buf.f_blocks;
		freeBlocks = buf.f_bfree;
		
		// Write to buffer
		dest.used = ( blocks - freeBlocks ) * blockSize;
		dest.size = blocks * blockSize;
		dest.free = freeBlocks * blockSize;
		dest.blockSize = blockSize;
		return true;
	}
	return false;
}

bool			_filesystemController::fatReady = -1;
_assocDirList	_filesystemController::assocDirs;