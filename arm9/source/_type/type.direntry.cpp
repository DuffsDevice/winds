#include <_type/type.direntry.h>
#include <_type/type.program.h>
#include <_type/type.windows.h>
#include <_controller/controller.filesystem.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.registry.h>

extern "C"{
#include <_library/_fat/partition.h>
#include <_library/_fat/fatfile.h>
#include <_library/_fat/file_allocation_table.h>
}

namespace unistd{
#include <unistd.h>
}

/* Return a string containing the path name of the parent
 * directory of PATH.  */
string dirname( string path )
{
	/* Go to the end of the string.  */
	_s32 s = path.length()-1;

	/* Strip off trailing /s (unless it is also the leading /).  */
	while ( s > 0 && path[s] == '/')
		s--;

	/* Strip the last component.  */
	while ( s >= 0 && path[s] != '/' )
		s--;

	while ( s >= 0 && path[s] == '/')
		s--;

	if (s < 0)
		return "/";

	return path.substr( 0 , s + 2 );
}


_direntry::_direntry( string&& fn ) :
	fHandle( nullptr ) // same as dHandle
	, filename( _filesystemController::replaceAssocDirs( move(fn) ) )
	, mimeType( _mime::text_plain )
	, mode( _direntryMode::closed )
{
	// Trim the filename
	trim( filename );
	
	// set Name (not filename!)
	bool hasLastDelim = this->filename.back() == '/';
	if( hasLastDelim ) // Remove the ending '/'
		this->filename.resize( this->filename.length() - 1 );
	
	_u64 posLast;
	if( ( posLast = this->filename.rfind('/') ) != string::npos ) // Cut everything before and including the last occouring slash ('/')
		this->name = this->filename.substr( posLast + 1 );
	else
		this->name = this->filename;
	
	// Reset stat
	stat_buf.st_mode = 0;
	
	// Fill Stat-Struct
	if( hasLastDelim && this->filename.empty() ) // must be the root
		this->exists = !stat( "/" , &this->stat_buf );
	else
		this->exists = !stat( this->filename.c_str() , &this->stat_buf );
	
	if( !this->exists )
	{
		if( hasLastDelim ) // Is a filename that, if created, would lead to a directory?
			stat_buf.st_mode = _IFDIR; // Make it a directory
		else
			stat_buf.st_mode = 0;
	}
	
	// Set Mime-Type
	if( this->isDirectory() ){
		this->filename.push_back('/');
		this->mimeType = _mime::directory;
	}
	else if( this->name.back() != '.' ) // Must be a file
	{
		// Set Extension
		_u64 lastDot = this->name.rfind('.');
		
		if( lastDot != string::npos ){
			this->extension = this->name.substr( lastDot + 1 );
			this->name.resize( lastDot );
		}
		
		this->mimeType = _mimeType::fromExtension( this->getExtension() );
	}
}

_direntry& _direntry::operator=( _direntry&& other )
{
	this->close();
	this->filename = move( other.filename );
	this->name = move( other.name );
	this->extension = move( other.extension );
	this->stat_buf = move( other.stat_buf );
	this->mimeType = move( other.mimeType );
	this->mode = move( other.mode );
	this->exists = move( other.exists );
	this->fHandle = move( other.fHandle );
	other.fHandle = nullptr;
	other.mode = _direntryMode::closed;
	
	return *this;
}


_direntry& _direntry::operator=( const _direntry& other )
{
	this->close();
	this->filename = other.filename;
	this->name = other.name;
	this->extension = other.extension;
	this->stat_buf = other.stat_buf;
	this->mimeType = other.mimeType;
	this->mode = _direntryMode::closed;
	this->exists = other.exists;
	this->fHandle = nullptr;
	
	return *this;
}


bool _direntry::close() const
{
	if( !_filesystemController::isFatReady() || !this->exists )
		return false;
	
	// Returnm, if everything is closed already
	if( this->mode == _direntryMode::closed || !this->fHandle ) // same as dHandle
		return true;
	
	// Close Method, depending on whether it's a directory
	if( !this->isDirectory() )
	{
		if( fclose( this->fHandle ) != 0 )
			return false;
		
		this->flush();
	}
	else
	{
		if( closedir( this->dHandle ) != 0 )
			return false;
	}
	// Reset
	this->fHandle = nullptr; // same as dHandle = nullptr
	this->mode = _direntryMode::closed;
	return true;
}


// Get the definition of libfat
PARTITION* _FAT_partition_getPartitionFromPath (const char* path);

bool _direntry::flush() const 
{
	if( !_filesystemController::isFatReady() || !this->exists || this->isDirectory() )
		return false;
	
	// Get Partition
	PARTITION *partition = _FAT_partition_getPartitionFromPath( this->filename.c_str() );

	// Check Validity
	if( !partition )
		return false;
	
	// Flush all sectors in teh cache of this specific partition
	if ( !_FAT_cache_flush( partition->cache ) )
		return false;

	return true;
}


bool _direntry::setAttrs( _direntryAttributes attrs )
{	
	if( !_filesystemController::isFatReady() )
		return false;
	return FAT_setAttr( this->filename.c_str() , (_u8)attrs ) == 0;
}


_direntryAttributes _direntry::getAttrs() const
{	
	if( !_filesystemController::isFatReady() )
		return 0;
	return FAT_getAttr( this->filename.c_str() );
}


bool _direntry::open( string mode ) const
{
	if( !_filesystemController::isFatReady() || !this->exists )
		return false;
	
	//! Close the previous Handle
	if( !this->close() )
		return false;
	
	//! Open the File/Directory
	if( this->isDirectory() )
		this->dHandle = opendir( this->filename.c_str() );
	else
		this->fHandle = fopen( this->filename.c_str() , mode.c_str() );
	
	if( this->dHandle ) // dHandle and fHandle are the same
	{
		this->mode = mode[0] == 'w' || mode[0] == 'a' ? _direntryMode::write : _direntryMode::read;
		return true;
	}
	
	return false;
}

string _direntry::getDisplayName( bool forceRealName ) const
{
	if( !forceRealName )
	{
		_mimeType mime = this->getMimeType();
		switch( mime )
		{
			case _mime::application_octet_stream:
			case _mime::application_microsoft_installer:
			case _mime::application_x_lua_bytecode:
			{
				_program* prog = _program::fromFile( this->getFileName() );
				if( prog )
				{
					// Get Header of the program
					_programHeader& header = prog->getHeader();
					
					string name = "";
					if( header.fileName != nullptr )
						name = move(*header.fileName);
					
					// Delete the program again
					delete prog;
					
					if( !name.empty() )	
						return move(name);
				}
			}
			default:
				break;
		}
		
		// Certain Files do not have an .extension
		if( !_guiController::showFileExtension() )
			return this->name;
	}
	
	const string& ext = this->getExtension();
	
	if( ext.empty() )
		return this->name;
	
	return this->name + "." + ext;
}


bool _direntry::openread() const {
	return this->open( "rb" ); // Open for read, do not create
}


bool _direntry::openwrite( bool eraseOld )
{
	if( this->exists )
		return this->open( eraseOld ? "wb" : "ab" ); // Open for read & write, do not create if already existing
	return false;
}


string _direntry::getParentDirectory() const {
	return dirname(this->filename);
}


bool _direntry::create()
{
	if( !_filesystemController::isFatReady() )
		return false;
	
	if( this->exists || this->filename.empty() ) // Return if the file does already exist
		return true;
	
	string parent = this->getParentDirectory();
	
	// If we cannot make our way 'upstairs' the tree, abort
	if( parent == this->filename )
		return false;
	
	// Build everything "upstairs" (recursively!)
	_direntry parentEntry = _direntry( move(parent) );
	
	if( !parentEntry.create() )
		return false;
	
	// Then create the file/directory
	if( this->isDirectory() )
	{
		// Remove slash, because mkdir doesn't like a terminating '/'
		_char* filenameWithoutSlash = const_cast<_char*>(this->filename.data());
		_u32 length = this->filename.length();
		_char end = filenameWithoutSlash[length-1]; // Backup
		filenameWithoutSlash[length-1] = 0; // Shorten the string by one '/'
		
		// Create the directory
		bool result = mkdir( filenameWithoutSlash , S_IRWXU | S_IRWXG | S_IRWXO ) == 0;
		
		// Restore the '/'
		filenameWithoutSlash[length-1] = end;
		
		return result && ( this->exists = !stat( this->filename.c_str() , &this->stat_buf ) );
	}
	else
		return ( this->fHandle = fopen( this->filename.c_str() , "w" ) ) && ( this->exists = !stat( this->filename.c_str() , &this->stat_buf ) ) && this->close();
}


bool _direntry::read( void* dest , _optValue<_u32> size , _u32* numBytes ) const
{	
	if( _filesystemController::isFatReady() && !this->isDirectory() )
	{
		if( !size.isValid() )
			size = this->getSize();
		
		_direntryMode modePrev = this->mode;
		
		if( this->mode == _direntryMode::closed && !this->openread() )
			return false;
		
		//! Set Iterator to beginning
		rewind( this->fHandle );
		
		//! Read bytes
		_u32 numBytesRead = fread( dest , 1 , size , this->fHandle );
		if( numBytes )
			*numBytes = numBytesRead;
		
		if( modePrev == _direntryMode::closed )
			this->close();
		
		return 0 == ferror( this->fHandle ); // Check if there was an error	
	}
	
	return false;
}


bool _direntry::readChild( _literal& child , _fileExtensionList* allowedExtensions ) const
{
	if( _filesystemController::isFatReady() && this->exists && this->isDirectory() )
	{
		// Open the Directory if necesary
		if( this->mode == _direntryMode::closed && this->openread() == false )
			return false;
		if( !this->dHandle )
			return false;
		
		struct dirent *dir;
		
		while( ( dir = readdir( this->dHandle ) ) != nullptr )
		{
			if( dir->d_name[0] == '.' 
				&& (
					dir->d_name[1] == 0
					|| ( dir->d_name[1] == '.' && dir->d_name[2] == 0 )
				)
			)
				continue;
				
			// Check whether we have to check for a specific file extension (only if the currently processed file is not a directory)
			if( allowedExtensions && dir->d_type != DT_DIR )
			{
				_u32 nameLen = strlen( dir->d_name );
				_literal nameEnd = dir->d_name + nameLen - 1;
				
				for( const string& ext : *allowedExtensions ) // Iterate over valid extensions
				{
					_u32 extLen = ext.length();
					
					if( !extLen ) // Skip empty extensions
						continue;
					if( extLen == 1 && ext[0] == '*' )
						goto _found;
					
					_u32 i = 0;
					_u32 maxLen = min( extLen , nameLen );
					while( i < maxLen )
					{
						if( ext[extLen-1-i] != *(nameEnd-i) ) // Extension of the entry is not what we need, check the next extension
							goto _continue;
						i++;
					}
					if( i == extLen && *(nameEnd-extLen) == '.' ) // We found an extension that fits the current file!
						goto _found;
					_continue:;
				}
				continue; // File does not match any extension
			}
			
			_found:
			
			// Write Name to destination and return, that we have found a valid next directory entry
			child = dir->d_name;
			return true;
		}
	}
	
	// We have not found any entry!
	child = nullptr;
	return false;
}

bool _direntry::readChildFolderOnly( _literal& child ) const
{
	if( _filesystemController::isFatReady() && this->exists && this->isDirectory() )
	{
		// Open the Directory if necesary
		if( this->mode == _direntryMode::closed && this->openread() == false )
			return false;
		if( !this->dHandle )
			return false;
		
		struct dirent *dir;
		
		while( ( dir = readdir( this->dHandle ) ) != nullptr )
		{
			if(
				dir->d_type != DT_DIR
				||
				(
					dir->d_name[0] == '.' 
					&& (
						dir->d_name[1] == 0
						|| ( dir->d_name[1] == '.' && dir->d_name[2] == 0 )
					)
				)
			)
				continue;
			
			// Write Name to destination and return, that we have found the next directory
			child = dir->d_name;
			return true;
		}
	}
	
	// We have not found any entry!
	child = nullptr;
	return false;
}


bool _direntry::rewindChildren() const
{
	if( _filesystemController::isFatReady() && this->exists && this->isDirectory() )
	{
		// Open the Directory if necesary
		if( this->mode == _direntryMode::closed )
			return true;
		
		if( !this->dHandle )
			return false;
			
		rewinddir( this->dHandle );
		
		return true;
	}
	return false;
}


bool _direntry::write( void* src , _u32 size )
{
	if( !_filesystemController::isFatReady() || this->isDirectory() )
		return false;
	
	_direntryMode modePrev = this->mode; // Save old state
	
	if( this->mode == _direntryMode::closed && !this->openwrite() )
		return false;
	else if( this->mode == _direntryMode::read || !this->fHandle )
		return false;
	
	int result = fwrite( src , size , 1 , this->fHandle );
	
	if( modePrev == _direntryMode::closed )
		this->close();
	
	return result == 0;
}


bool _direntry::writeString( string str )
{
	if( !_filesystemController::isFatReady() || this->isDirectory() )
		return false;
	
	_direntryMode modePrev = this->mode; // Save old state
	
	if( this->mode == _direntryMode::closed && !this->openwrite() )
		return false;
	else if( this->mode == _direntryMode::read || !this->fHandle )
		return false;
	
	int result = fputs( str.c_str() , this->fHandle );
	
	if( modePrev == _direntryMode::closed )
		this->close();
	
	return result >= 0;
}


string _direntry::readString( _optValue<_u32> size ) const
{
	if( !size.isValid() )
		size = this->getSize();
	
	_direntryMode modePrev = this->mode;
	
	if( this->mode == _direntryMode::closed && !this->openread() )
		return "";
	if( size <= 0 || !this->fHandle )
		return "";
	
	//! Output
	string out;
	
	// Temp...
	_char* text = new _char[(_u32)size];
	
	// Read the Contents
	_u32 actualSize = fread( text , sizeof(_char) , size , this->fHandle );
	
	//! Copy 'text' into 'out'
	out = text;
	
	out.resize( actualSize );
	
	// Free temorary storage
	delete[] text;
	
	if( modePrev == _direntryMode::closed )
		this->close();
	
	return move(out);
}


_u32 _direntry::getSize() const
{
	if( !_filesystemController::isFatReady() || !this->exists || this->isDirectory() )
		return 0;
	
	_u32 size = 0;
	
	//! Is the File already opened
	if( this->mode != _direntryMode::closed && this->fHandle != nullptr )
	{
		fpos_t lastpos; // Save last position of iterator
		fgetpos( this->fHandle , &lastpos );
		
		//! "Tell" Size
		rewind( this->fHandle );
		fseek ( this->fHandle , 0 , SEEK_END );
		size = ftell( this->fHandle );
		
		//! Restore iterator
		fsetpos( this->fHandle , &lastpos );
	}
	//! Open the file
	else if( this->openread() )
	{
		//! "Tell" Size
		fseek ( this->fHandle , 0 , SEEK_END );
		size = ftell( this->fHandle );
		
		//! Close file again
		this->close();
	}
	
	return size;
}

bool _direntry::execute( _programArgs args )
{
	//if( _windows::getHardwareType() != _hardwareType::emulator && !_filesystemController::isFatReady() )
	//	return false;
	
	if( this->isDirectory() )
		return false;
	
	switch( this->mimeType )
	{
		case _mime::application_octet_stream:
		case _mime::application_microsoft_installer:
		case _mime::application_x_lua_bytecode:
		{
			_program* prog = _program::fromFile( this->getFileName() );
			if( prog )
				prog->execute( move( args ) );
			else
				return false;
			return true;
		}
		case _mime::application_x_bat:
			return _windows::executeCommand( this->readString() );
		
		// Choose the right filetype handler
		default:
			// Fetch handler expression
			const string& command = _registryController::getFileTypeHandler( this->getExtension() );
			if( command.empty() )
				break;
			
			string commandCopy = command; // Work on copy
			_filesystemController::replaceAssocDirs( commandCopy ); // Replace Associative directories
			
			size_t pos = 0;
			// Replace all "$F" with the full file path
			while( (pos = commandCopy.find( "$F" , pos )) != string::npos )
				commandCopy.replace( pos , sizeof("$F") , this->getFileName() );
			
			pos = 0;
			// Replace all "$N" with the file name without extension
			while( (pos = commandCopy.find( "$N" , pos )) != string::npos )
				commandCopy.replace( pos , sizeof("$N") , this->getName() );
			
			// Execute the command
			return _windows::executeCommand( move(commandCopy) );
	}
	return false;
}

_bitmap _direntry::getFileImage() const
{
	string		extension = this->getExtension();
	_mimeType	mime = this->getMimeType();
	
	switch( mime )
	{
		// Give 
		case _mime::directory:
			extension = "%dir%";
			break;
		case _mime::application_octet_stream:
		case _mime::application_microsoft_installer:
		case _mime::application_x_lua_bytecode:
		{
			_program* prog = _program::fromFile( this->getFileName() );
			if( prog ){
				_bitmap bmp = prog->getFileImage();
				delete prog;
				if( bmp.isValid() )
					return move(bmp);
			}
		}
		default:
			break;
	}
	
	return _registryController::getFileTypeImage( extension , mime );
}

bool _direntry::unlink( bool removeContents )
{
	if( !this->exists )
		return false;
	
	if( this->mode != _direntryMode::closed && !this->close() )
		return false; // Cannot close the file!
	
	if( ( !this->isDirectory() || std::remove( (this->filename + "/*").c_str() ) == 0 ) && std::remove( this->filename.c_str() ) == 0 )
	{
		this->exists = false;
		return true;
	}
	
	return false;
}

bool _direntry::rename( string newName )
{
	if( this->mode != _direntryMode::closed )
		this->close();
	
	// Replace associative directory names
	_filesystemController::replaceAssocDirs( newName );
	
	if( std::rename( this->filename.c_str() , newName.c_str() ) == 0 ){
		*this = _direntry( newName.c_str() );
		return true;
	}
	return false;
}

bool _direntry::setHidden( bool hidden ){
	_direntryAttributes attrs = this->getAttrs();
	if( attrs.hidden == hidden )
		return true;
	attrs.hidden = hidden;
	return this->setAttrs( move(attrs) );
}

bool _direntry::setSystemFile( bool isSystem ){
	_direntryAttributes attrs = this->getAttrs();
	if( attrs.system == isSystem )
		return true;
	attrs.system = isSystem;
	return this->setAttrs( move(attrs) );
}

bool _direntry::setReadOnly( bool readOnly ){
	_direntryAttributes attrs = this->getAttrs();
	if( attrs.readonly == readOnly )
		return true;
	attrs.readonly = readOnly;
	return this->setAttrs( move(attrs) );
}

// Root Directory
_direntry _diskRoot_ = _direntry("/");