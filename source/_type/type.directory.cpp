#include "_type/type.directory.h"
#include <sys/dir.h>
#include <stdio.h>
#include <dirent.h>

bool _directory::open(){

	if( this->handle != nullptr && this->mode != closed )
		return true;
	
	this->handle = opendir( this->filename.c_str() );
	
	if( this->handle == nullptr ){
		this->mode = closed;
		return false;
	}
	
	// Increas size of currently opened Files
	this->openedFiles += 1;
	this->mode = read;
	
	return true;
}

bool _directory::close(){

	if( this->mode != closed || this->handle != nullptr )
	{
		if( closedir( this->handle ) != 0 )
			return false;
		
		this->handle = nullptr;
		this->mode = closed;
	}
	this->openedFiles--;
	return true;
}


bool _directory::readChildren(){
	
	_fileModeOpen modePrev = this->mode;
	
	if( this->handle == nullptr || this->mode == closed )
		this->open();
	
	if( this->mode == closed || this->handle == nullptr )
		return false;
	
	// Dir Entry
	rewinddir( this->handle );

	for( struct dirent *dir = readdir( this->handle ); dir != nullptr ; dir = readdir( this->handle ) )
	{
		if( dir->d_type == DT_DIR )
			this->children.push_back( new _directory( this->filename + "/" + dir->d_name ) );
		else
			this->children.push_back( new _file( this->filename + "/" + dir->d_name ) );
    }

    if( modePrev == closed )
		closedir( this->handle );
	
	return true;
}

/// Root Directory
_directory* _diskRoot_ = new _directory("/");