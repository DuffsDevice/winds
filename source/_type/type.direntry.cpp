#include "_type/type.direntry.h"
#include "_type/type.system.h"
#include "_type/type.progLua.h"

extern "C"{
#include "_library/_fat/partition.h"
#include "_library/_fat/fatfile.h"
#include "_library/_fat/file_allocation_table.h"
}

namespace unistd{
#include <unistd.h>
}

#include "_resource/BMP_FileIcon.h"
#include "_resource/BMP_ExeIcon.h"
#include "_resource/BMP_TxtIcon.h"
#include "_resource/BMP_XmlIcon.h"
#include "_resource/BMP_NdsIcon.h"
#include "_resource/BMP_GbaIcon.h"
#include "_resource/BMP_IniIcon.h"
#include "_resource/BMP_LuaIcon.h"
#include "_resource/BMP_FolderIcon.h"

//#define FAT_EMULATOR


#ifdef FAT_EMULATOR
#include "program_bin.h"
#include "image_bin.h"
#endif

int _direntry::fatInited = -1;

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

string replaceASSOCS( string path )
{
	for( auto& assoc : _system::_runtimeAttributes_->assocDirectories )
	{
		size_t pos = path.find( assoc.first );
		if ( pos != string::npos )
		{
		   path.erase( pos, assoc.first.size() );
		   path.insert( pos, assoc.second );
		}
	}
	return path;
}

bool _direntry::initFat()
{
	//! libFat instantiation
	if( _direntry::fatInited == -1 )
		_direntry::fatInited = fatInitDefault();
	
	return _direntry::fatInited;
}

_direntry::_direntry( string fn ) :
	fHandle( nullptr ) // same as dHandle
	, filename( replaceASSOCS( fn ) )
	, mode( _direntryMode::closed )
{
	initFat();
	
	// set Name (not filename!)
	_u64 posLast = this->filename.find_last_of("/");
	if( posLast != string::npos )
		this->name = this->filename.substr( posLast + 1 );
	else
		this->name = this->filename;
	this->name = this->name.substr( 0 , this->name.find_last_of(".") );
	
	// Fill Stat-Struct
	this->exists = !stat( this->filename.c_str() , &this->stat_buf );
	
	//! Set Mime-Type
	if( this->isDirectory() )
	{
		this->mimeType = _mime::directory;
		if( *( this->filename.rbegin() + 1 ) != '/' )
			this->filename += "/";
	}
	else
	{
		// Set Extension
		_u64 lastDot = this->filename.find_last_of(".");
		_u64 lastSlash = this->filename.find_last_of("/");
		
		if( ( lastSlash == string::npos || lastDot > lastSlash ) && lastDot != string::npos ) // Valid filename with extension?
			this->extension = this->filename.substr( lastDot + 1 );
		
		this->mimeType = _mimeType::fromExtension( this->getExtension() );
	}
	
	//! Removed, because it makes Working Directories not work :D
	//if( this->filename.front() != '/' )
	//	this->filename.insert( 0 , 1 , '/' );
}

_direntry& _direntry::operator=( _direntry other )
{
	this->close();
	this->filename = other.filename;
	this->name = other.name;
	this->extension = other.extension;
	this->stat_buf = other.stat_buf;
	this->mimeType = other.mimeType;
	this->mode = _direntryMode::closed;
	
	return *this;
}

bool _direntry::close()
{
	if( !this->fatInited || !this->exists )
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

bool _direntry::flush()
{
	if( this->isDirectory() )
		return false;
	
	// Defines...
	PARTITION *partition = NULL;

	// Get Partition
	partition = _FAT_partition_getPartitionFromPath( this->filename.c_str() );

	// Check Partition
	if( !partition )
		return false;
	
	// Flush any sectors in the disc cache
	if ( !_FAT_cache_flush( partition->cache ) )
		return false;

	return true;
}

int _direntry::setAttrs( _direntryAttributes attrs )
{	
	if( !this->fatInited )
		return 0;
	return FAT_setAttr( this->filename.c_str() , attrs );
}

_direntryAttributes _direntry::getAttrs()
{	
	if( !this->fatInited )
		return 0;
	return FAT_getAttr( this->filename.c_str() );
}

bool _direntry::open( string mode )
{
	if( !this->fatInited || !this->exists )
		return false;
	
	//! Close the previous Handle
	if( !this->close() )
		return false;
	
	//! Open the File/Directory
	if( !this->isDirectory() )
		this->fHandle = fopen( this->filename.c_str() , mode.c_str() );
	else
		this->dHandle = opendir( this->filename.c_str() );
	
	if( this->dHandle ) // dHandle and fHandle are the same
	{
		this->mode = mode[0] == 'r' ? _direntryMode::read : _direntryMode::write;
		return true;
	}
	
	return false;
}

bool _direntry::openread(){
	return this->open( "rb" ); // Open for read, do not create
}

bool _direntry::openwrite( bool eraseOld )
{
	return !this->exists && this->open( eraseOld ? "wb+" : "rb+" ); // Open for read & write, do not create if already existing
}

bool _direntry::create()
{
	if( !this->fatInited )
		return false;
	
	if( this->exists || this->filename == "/" ) // Return if the file does already exist
		return true;
	
	// Build everything "upstairs" (recursively!)
	_direntry parentEntry = _direntry( dirname( this->filename ) );
	
	if( !parentEntry.create() )
		return false;
	
	// Then create the file/directory
	if( this->filename.back() == '/' )
		return mkdir( this->filename.substr( 0 , this->filename.length() - 1 ).c_str() , S_IRWXU | S_IRWXG | S_IRWXO ) == 0 && ( this->exists = !stat( this->filename.c_str() , &this->stat_buf ) );
	else
		return ( this->fHandle = fopen( this->filename.c_str() , "w" ) ) && ( this->exists = !stat( this->filename.c_str() , &this->stat_buf ) ) && this->close();
}

#ifdef FAT_EMULATOR
#include <string.h>
#endif

bool _direntry::read( void* dest , _u32 size )
{
	#ifdef FAT_EMULATOR
	if( !size )
			size = this->getSize();
	
	memcpy( dest , program_bin , size );
	return true;
	#endif
	
	if( this->fatInited && !this->isDirectory() )
	{
		if( !size )
			size = this->getSize();
		
		_direntryMode modePrev = this->mode;
		
		if( this->mode == _direntryMode::closed && !this->openread() )
			return false;
		
		//! Set Iterator to beginning
		rewind( this->fHandle );
		fread( dest , 1 , size , this->fHandle );
		
		if( modePrev == _direntryMode::closed )
			this->close();
		
		return 0 == ferror( this->fHandle ); // Check if there was an error	
	}
	
	return false;
}

bool _direntry::readChild( string& child )
{
	if( this->fatInited && this->isDirectory() )
	{
		// Open the Directory if necesary
		if( this->mode == _direntryMode::closed && this->open() == false )
			return false;
		if( !this->dHandle )
			return false;
		
		struct dirent *dir;
		
		if( ( dir = readdir( this->dHandle ) ) == nullptr )
			return false;
		
		if( dir->d_name[0] == '.' && dir->d_name[1] == 0 )
			return this->readChild( child );
		else if( dir->d_name[0] == '.' && dir->d_name[1] == '.' && dir->d_name[2] == 0 )
			return this->readChild( child );
		
		// Write Name to destination
		child = dir->d_name;
		
		return true;
	}
	
	return false;
}

bool _direntry::rewindChildren()
{
	if( this->fatInited && this->isDirectory() )
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
	if( !this->fatInited || this->isDirectory() )
		return false;
	
	_direntryMode modePrev = this->mode; // Save old state
	
	if( this->mode == _direntryMode::closed && !this->openwrite() )
		return false;
	else if( this->mode == _direntryMode::read || !this->fHandle )
		return false;
	
	fwrite( src , size , 1 , this->fHandle );
	
	if( modePrev == _direntryMode::closed )
		this->close();
	
	return true;
}

bool _direntry::writeString( string str )
{	
	if( !this->fatInited || this->isDirectory() )
		return false;
	
	_direntryMode modePrev = this->mode; // Save old state
	
	if( this->mode == _direntryMode::closed && !this->openwrite() )
		return false;
	else if( this->mode == _direntryMode::read || !this->fHandle )
		return false;
	
	fputs( str.c_str() , this->fHandle );
	
	if( modePrev == _direntryMode::closed )
		this->close();
	
	return true;
}

string _direntry::readString( _u32 size )
{
	#ifdef FAT_EMULATOR
	string str = (const char*)program_bin;
	return str;
	#else
	if( !this->fatInited || this->isDirectory() )
		return "";
	#endif
	
	if( !size )
		size = this->getSize();
	
	_direntryMode modePrev = this->mode;
	
	if( this->mode == _direntryMode::closed && !this->openread() )
		return "";
	if( size <= 0 || !this->fHandle )
		return "";
	
	//! Output
	string out;
	
	// Temp...
	char* text = new char[size];
	
	// Read the Contents
	_u32 actualSize = fread( text , sizeof(_char) , size , this->fHandle );
	
	//! Copy 'text' into 'out'
	out = text;
	
	out.resize( actualSize );
	
	// Free temorary storage
	delete[] text;
	
	if( modePrev == _direntryMode::closed )
		this->close();
	
	return out;
}

_u32 _direntry::getSize()
{
	#ifdef FAT_EMULATOR
	return program_bin_size;
	#endif
	
	if( !this->fatInited || !this->exists || this->isDirectory() )
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


string _direntry::getWorkingDirectory()
{
	char val[PATH_MAX];
	
	// Get working directory
	unistd::getcwd( val , PATH_MAX );
	
	return val;
}

void _direntry::setWorkingDirectory( string dir )
{
	unistd::chdir( replaceASSOCS(dir).c_str() );
}

bool _direntry::execute()
{
	#ifndef FAT_EMULATOR
	if( !this->fatInited )
		return false;
	#endif
	
	if( this->isDirectory() )
		return false;
	
	switch( this->mimeType )
	{
		case _mime::application_octet_stream:
		case _mime::application_x_lua_bytecode:{
			_program* prog = nullptr;
			#ifdef FAT_EMULATOR
				string pro = (const char*)program_bin;
				pro.resize( program_bin_size );
				prog = new _progLua( pro );
			#else
				prog = new _progLua( this->readString() );
			#endif
			prog->execute();
			break;
		}
		case _mime::directory:
			_system::getBuiltInProgram( "explorer.exe" )->execute({{"path",this->filename}});
			break;
		default:
			return false;
	}
	return true;
}

_bitmap _direntry::getFileImage()
{
	switch( this->getMimeType() )
	{
		case _mime::directory:
			return BMP_FolderIcon();
			
		case _mime::application_octet_stream:
			return BMP_ExeIcon();
			
		case _mime::application_x_lua_bytecode:
			return BMP_LuaIcon();
			
		case _mime::application_microsoft_installer:
			return BMP_FileIcon();
			
		case _mime::text_plain:
			return BMP_TxtIcon();
			
		case _mime::text_xml:
			return BMP_XmlIcon();
			
		case _mime::text_x_ini:
			return BMP_IniIcon();
			
		case _mime::application_x_nintendo_ds_rom:
			return BMP_NdsIcon();
			
		case _mime::application_x_nintendo_gba_rom:
			return BMP_GbaIcon();
			
		default:
			break;
	}
	return BMP_FileIcon();
}

bool _direntry::unlink()
{
	if( !this->exists )
		return false;
	
	if( this->mode != _direntryMode::closed && !this->close() )
		return false; // Cannot close the file!
	
	if( std::remove( this->filename.c_str() ) == 0 )
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
	newName = replaceASSOCS( newName );
	
	if( std::rename( this->filename.c_str() , newName.c_str() ) == 0 )
	{
		this->filename = newName;
		this->extension = "";
		
		// set Name (not filename!)
		_u64 posLast = this->filename.find_last_of("/");
		if( posLast != string::npos )
			this->name = this->filename.substr( posLast + 1 );
		else
			this->name = this->filename;
		this->name = this->name.substr( 0 , this->name.find_last_of(".") );
		
		// Fill Stat-Struct
		this->exists = !stat( this->filename.c_str() , &this->stat_buf );
		
		//! Set Mime-Type
		if( this->isDirectory() )
		{
			this->mimeType = _mime::directory;
			if( *( this->filename.rbegin() + 1 ) != '/' )
				this->filename += "/";
		}
		else
		{
			// Set Extension
			_u64 lastDot = this->filename.find_last_of(".");
			_u64 lastSlash = this->filename.find_last_of("/");
			
			if( ( lastSlash == string::npos || lastDot > lastSlash ) && lastDot != string::npos ) // Valid filename with extension?
				this->extension = this->filename.substr( lastDot + 1 );
			
			this->mimeType = _mimeType::fromExtension( this->getExtension() );
		}
		
		if( this->filename.front() != '/' )
			this->filename.insert( 0 , 1 , '/' );
		
		return true;
	}
	return false;
}

/// Root Directory
_direntry _diskRoot_ = _direntry("/");