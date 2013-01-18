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

#define FAT_EMULATOR_


#ifdef FAT_EMULATOR_
#include "program_bin.h"
#include "image_bin.h"
#endif

_bitmap icon_exe = BMP_ExeIcon();
_bitmap icon_lua = BMP_LuaIcon();
_bitmap icon_msi = BMP_FileIcon();
_bitmap icon_plain = BMP_FileIcon();
_bitmap icon_txt = BMP_TxtIcon();
_bitmap icon_xml = BMP_XmlIcon();
_bitmap icon_ini = BMP_IniIcon();
_bitmap icon_nds = BMP_NdsIcon();
_bitmap icon_gba = BMP_GbaIcon();
_bitmap icon_folder = BMP_FolderIcon();

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

bool _direntry::initFat(){
	//! libFat instantiation
	if( _direntry::fatInited == -1 )
		_direntry::fatInited = fatInitDefault();
	return _direntry::fatInited;
}

_direntry::_direntry( string fn ) :
	fHandle( nullptr )
	, dHandle( nullptr )
	, filename( replaceASSOCS( fn ) )
	, mode( _direntryMode::closed )
{
	initFat();
	
	// set Name (not filename!)
	this->name = this->filename.substr( this->filename.find_last_of("/") + 1 );
	this->name = this->name.substr( 0 , this->name.find_last_of(".") );
	
	// Fill Stat-Struct
	this->exists = !stat( this->filename.c_str() , &this->stat_buf );
	
	//printf("direntry: %d, %s\n",this->getSize(),this->filename.c_str());
	
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
	
	//if( this->filename.front() != '/' )
	//	this->filename.insert( 0 , 1 , '/' );
}

bool _direntry::close()
{
	if( !this->fatInited || !this->exists )
		return false;
	
	// Returnm, if everything is closed already
	if( this->mode == _direntryMode::closed && !this->fHandle && !this->dHandle )
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
	this->dHandle = nullptr;
	this->fHandle = nullptr;
	this->mode = _direntryMode::closed;
	return true;
}

// Get the definition of libfat
PARTITION* _FAT_partition_getPartitionFromPath (const char* path);

bool _direntry::flush()
{

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

bool _direntry::open( const char* mode )
{
	if( !this->fatInited || !this->exists )
		return false;
	
	//! Close the previous Handle
	if( !this->close() )
		return false;
	
	//! Open the File/Directory
	if( !this->isDirectory() )
		this->fHandle = fopen( this->filename.c_str() , mode );
	else
		this->dHandle = opendir( this->filename.c_str() );
	
	if( this->dHandle || this->fHandle )
		return true;
	return false;
}

inline bool _direntry::openread(){
	return this->open( "rb" ) && _u8( this->mode = _direntryMode::read ); // Open for read, do not create
}

inline bool _direntry::openwrite( bool erase ){
	if( !this->exists )
		return false;
	return this->open( erase ? "wb+" : "rb+" ) && _u8(this->mode = _direntryMode::write); // Open for read & write, do not create
}

inline bool _direntry::create()
{
	if( !this->fatInited )
		return false;
	
	if( this->exists || this->filename == "/") // Return if the file does already exist
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

#ifdef FAT_EMULATOR_
#include <string.h>
#endif

bool _direntry::read( void* dest , _u32 size )
{
	#ifdef FAT_EMULATOR_
	if( !size )
			size = this->getSize();
	
	memcpy( dest , program_bin , size );
	return true;
	#endif
	
	if( this->fatInited || !this->isDirectory() )
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
		if( this->mode == _direntryMode::closed )
			this->dHandle = opendir( this->filename.c_str() );
		
		if( !this->dHandle )
			return false;
		else
			this->mode = _direntryMode::read;
		
		struct dirent *dir;
		
		if( ( dir = readdir( this->dHandle ) ) == nullptr )
			return false;
		
		if( dir->d_name[0] == '.' && dir->d_name[1] == 0 )
			return this->readChild( child );
		else if( dir->d_name[0] == '.' && dir->d_name[1] == '.' && dir->d_name[2] == 0 )
			return this->readChild( child );
		child = this->filename + dir->d_name;
		
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
	if( !this->fatInited )
		return false;
	_direntryMode modePrev = this->mode;
	
	if( this->mode == _direntryMode::closed && !this->openwrite() )
		return false;
	else if( this->mode == _direntryMode::read )
		return false;
	
	fwrite( src , size , 1 , this->fHandle );
	
	if( modePrev == _direntryMode::closed )
		this->close();
	
	return true;
}

bool _direntry::writeString( string str )
{	
	if( this->fatInited && !this->isDirectory() )
	{
		_direntryMode modePrev = this->mode;
		
		if( this->mode == _direntryMode::closed && !this->openwrite() )
			return false;
		else if( this->mode == _direntryMode::read )
			return false;
		
		fputs( str.c_str() , this->fHandle );
		
		if( modePrev == _direntryMode::closed )
			this->close();
		
		return true;
	}
	return false;
}

string _direntry::readString( _u32 size )
{
	#ifdef FAT_EMULATOR_
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
	if( size <= 0 )
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

_mimeType _direntry::getMimeType(){
	return this->mimeType;
}

_u32 _direntry::getSize()
{
	#ifdef FAT_EMULATOR_
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
		
		//! Close file
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
	//printf("change dir to %s = %d",replaceASSOCS(dir).c_str(),chdir( replaceASSOCS(dir).c_str()) );
	unistd::chdir( replaceASSOCS(dir).c_str() );
}

bool _direntry::execute()
{
	//!TODO: remove Comment
	//if( !this->fatInited )
	//	return false;
	
	_mimeType mime = _mimeType::fromExtension( this->getExtension() );
	switch( mime )
	{
		case _mime::application_octet_stream:
		case _mime::application_x_lua_bytecode:{
			_program* prog = nullptr;
			#ifdef FAT_EMULATOR_
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

const _bitmap& _direntry::getFileImage()
{
	switch( this->getMimeType() ){
		case _mime::directory:
			return icon_folder;
		case _mime::application_octet_stream:
			return icon_exe;
		case _mime::application_x_lua_bytecode:
			return icon_lua;
		case _mime::application_microsoft_installer:
			return icon_msi;
		case _mime::text_plain:
			return icon_txt;
		case _mime::text_xml:
			return icon_xml;
		case _mime::text_x_ini:
			return icon_ini;
		case _mime::application_x_nintendo_ds_rom:
			return icon_nds;
		case _mime::application_x_nintendo_gba_rom:
			return icon_gba;
		default:
			break;
	}
	return icon_plain;
}

bool _direntry::unlink()
{
	if( this->mode != _direntryMode::closed )
		this->close();
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
		this->name = this->filename.substr( this->filename.find_last_of("/") + 1 );
		this->name = this->name.substr( 0 , this->name.find_last_of(".") );
		
		// Fill Stat-Struct
		this->exists = !stat( this->filename.c_str() , &this->stat_buf );
		
		//! Set Mime-Type
		if( this->isDirectory() )
		{
			this->mimeType = _mime::directory;
			if( *(this->filename.rbegin()+1) != '/' )
				this->filename += "/";
		}
		else
		{
			// Set Extension
			size_t pos = this->filename.find_last_of(".");
			size_t npos = this->filename.find_last_of("/");
			if( pos > npos && pos != this->filename.npos )
				this->extension = this->filename.substr( pos + 1 );
			
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