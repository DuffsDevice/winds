#include "_type/type.directory.h"
#include "_file/file.shortcut.h"
#include <sys/dir.h>
#include <stdio.h>
#include <dirent.h>

#include "_graphic/BMP_FolderIcon.h"
_bitmap* icon_folder = new BMP_FolderIcon();

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
	this->mode = _fileModeOpen::read;
	
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

bool _directory::create( bool fullCreate ){
	if( fullCreate )
		return _file::_createPath( this->filename , false );
	return mkdir( this->filename.c_str() , S_IRWXU | S_IRWXG | S_IRWXO ) == 0;
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
		if( dir->d_type == DT_DIR ){
			this->children.push_back( new _directory( this->filename + "/" + dir->d_name ) );
		}
		else{
			_file tmpFile = { this->filename + "/" + dir->d_name };
			if( tmpFile.getMimeType() == _mime::application_x_ms_shortcut ) // Symbolic Link
				this->children.push_back( new _shortcut( this->filename + "/" + dir->d_name ) );
			else
				this->children.push_back( new _file( this->filename + "/" + dir->d_name ) );
		}
    }

    if( modePrev == closed )
		closedir( this->handle );
	
	return true;
}

const _bitmap* _directory::getFileImage(){
	return icon_folder;
}

string _directory::getExtension(){
	return "";
}

/// Root Directory
_directory* _diskRoot_ = new _directory("/");