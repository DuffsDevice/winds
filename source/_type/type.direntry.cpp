#include "_type/type.direntry.h"
#include "_type/type.system.h"
extern "C"{
#include "_file/_fat/partition.h"
#include "_file/_fat/fatfile.h"
#include "_file/_fat/file_allocation_table.h"
}

#include "_graphic/BMP_FileIcon.h"
#include "_graphic/BMP_ExeIcon.h"
#include "_graphic/BMP_TxtIcon.h"
#include "_graphic/BMP_XmlIcon.h"
#include "_graphic/BMP_LuaIcon.h"
#include "_graphic/BMP_FolderIcon.h"

_bitmap* icon_exe = new BMP_ExeIcon();
_bitmap* icon_lua = new BMP_LuaIcon();
_bitmap* icon_msi = new BMP_FileIcon();
_bitmap* icon_plain = new BMP_FileIcon();
_bitmap* icon_txt = new BMP_TxtIcon();
_bitmap* icon_xml = new BMP_XmlIcon();
_bitmap* icon_folder = new BMP_FolderIcon();

int _direntry::fatInited = -1;



string replaceASSOCS( string path )
{
	for( auto& assoc : _system_->_runtimeAttributes_->assocDirectories )
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

_direntry::_direntry( string fn ) :
	fHandle( nullptr )
	, dHandle( nullptr )
	, filename( replaceASSOCS( fn ) )
	, mode( closed )
{
	 //! libFat instantiation
	if( this->fatInited == -1 )
		this->fatInited = fatInitDefault();
	
	// set Name (not filename!)
	this->name = this->filename.substr( this->filename.find_last_of("/") + 1 );
	this->name = this->name.substr( 0 , this->name.find_last_of(".") );
	
	// Fill Stat-Struct
	this->exists = !stat( this->filename.c_str() , &this->stat_buf );
	
	//! Set Mime-Type
	if( this->isDirectory() )
	{
		//printf("directory:%s->%c\n",this->filename.c_str(),this->filename.back() );
		this->mimeType = _mime::directory;
		if( *(this->filename.end()-1) != '/' )
			this->filename += "/";
		//printf("directory:%s\n",this->filename.c_str() );
	}
	else
	{
		// Set Extension
		_coord pos = this->filename.find_last_of(".");
		_coord npos = this->filename.find_last_of("/");
		if( pos > npos && pos != this->filename.npos )
			this->extension = this->filename.substr( pos + 1 );
		
		this->mimeType = _mimeType::fromExtension( this->getExtension() );
	}
}

bool _direntry::close()
{
	if( !this->exists )
		return false;
	
	// Returnm, if everything is closed already
	if( this->mode == _direntryMode::closed && !this->fHandle && !this->dHandle )
		return true;
	
	// Close Method, depending on whether it's a directory
	if( !this->isDirectory() )
	{
		if( fclose( this->fHandle ) != 0 )
			return false;
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

int _direntry::setAttrs( _direntryAttributes attrs )
{	
	return FAT_setAttr( this->filename.c_str() , attrs );
}

bool _direntry::open( const char* mode )
{
	if( !this->exists )
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
	return this->open( "rb" ) && (this->mode = _direntryMode::read ); // Open for read, do not create
}

inline bool _direntry::openwrite(){
	return this->open( "rb+" ) && (this->mode = _direntryMode::write); // Open for read & write, do not create
}

inline bool _direntry::create()
{
	if( this->exists && this->filename == "/") // Return if the file does already exist
		return true;
	
	_u32 delimiter = max( (int)this->filename.find_last_of("/") , 1 );
	string fileName = this->filename.substr( delimiter , string::npos );
	string leftPath = this->filename.substr( 0 , delimiter );
	
	// Build everything "upstairs" (recursively!)
	_direntry parentEntry = _direntry( leftPath );
	
	if( !parentEntry.create() )
		return false;
	
	// Then create the file/directory
	if( !this->isDirectory() )
		return ( this->fHandle = fopen( this->filename.c_str() , "w" ) ) && this->close();
	else
		return mkdir( this->filename.c_str() , S_IRWXU | S_IRWXG | S_IRWXO ) == 0;
}

bool _direntry::read( void* dest , _u32 size )
{
	if( !this->isDirectory() )
	{
		if( !size )
			size = this->getSize();
		
		_direntryMode modePrev = this->mode;
		
		if( this->mode == _direntryMode::closed && !this->openread() )
			return false;
		
		//! Set Iterator to beginning
		rewind( this->fHandle );
		fread( dest , 1 , size , this->fHandle );
		
		if( modePrev == closed )
			this->close();
		
		return 0 == ferror( this->fHandle ); // Check if there was an error	
	}
	return false;
}

bool _direntry::readChild( string& child )
{
	if( this->isDirectory() )
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
	if( this->isDirectory() )
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
	_direntryMode modePrev = this->mode;
	
	if( this->mode == _direntryMode::closed && !this->openwrite() )
		return false;
	else if( this->mode == _direntryMode::read )
		return false;
	
	fwrite( src , size , 1 , this->fHandle );
	
	if( modePrev == closed )
		this->close();
	
	return true;
}

bool _direntry::writeString( string str )
{	
	if( !this->isDirectory() )
	{
		_direntryMode modePrev = this->mode;
		
		if( this->mode == closed && !this->openwrite() )
			return false;
		else if( this->mode == _direntryMode::read )
			return false;
		
		fputs( str.c_str() , this->fHandle );
		
		if( modePrev == closed )
			this->close();
		
		return true;
	}
	return false;
}

string _direntry::readString( _u32 size )
{
	if( this->isDirectory() )
		return "";
	
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

_u32 _direntry::getSize(){
	
	if( this->isDirectory() )
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



bool _direntry::execute()
{
	_mimeType mime = _mimeType::fromExtension( this->getExtension() );
	switch( mime )
	{
		case _mime::application_octet_stream:
		case _mime::application_x_lua_bytecode:{
			_program* prog = new _progLua( this->readString() );
			prog->execute();
			break;
		}
		default:
			return false;
	}
	return true;
}

const _bitmap* _direntry::getFileImage()
{
	switch( this->getMimeType() ){
		case _mime::directory:
			return icon_folder;
			break;
		case _mime::application_octet_stream:
			return icon_exe;
			break;
		case _mime::application_x_lua_bytecode:
			return icon_lua;
			break;
		case _mime::application_microsoft_installer:
			return icon_msi;
			break;
		case _mime::text_plain:
			return icon_txt;
			break;
		case _mime::text_xml:
			return icon_xml;
			break;
		default:
			break;
	}
	return icon_plain;
}

/// Root Directory
_direntry _diskRoot_ = _direntry("/");