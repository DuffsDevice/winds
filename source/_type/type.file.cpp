#include "_type/type.file.h"
#include "_type/type.directory.h"
#include "_type/type.mime.h"
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

_bitmap* icon_exe = new BMP_ExeIcon();
_bitmap* icon_lua = new BMP_LuaIcon();
_bitmap* icon_msi = new BMP_FileIcon();
_bitmap* icon_plain = new BMP_FileIcon();
_bitmap* icon_txt = new BMP_TxtIcon();
_bitmap* icon_xml = new BMP_XmlIcon();



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


bool _file::fatInited = false;
_u8 _file::openedFiles = 0;

_file::_file( string fn ) :
	handle( nullptr )
	, filename( replaceASSOCS( fn ) )
	, mode( closed )
{ 
	if( !this->fatInited )
		this->fatInited = fatInitDefault();
	
	// Set Extension
	_coord pos = this->filename.find_last_of(".");
	_coord npos = this->filename.find_last_of("/");
	if( pos > npos && pos != this->filename.npos )
		this->extension = this->filename.substr( pos + 1 );
	
	// set Name (not filename!)
	this->name = this->filename.substr( this->filename.find_last_of("/") + 1 );
	this->name = this->name.substr( 0 , this->name.find_last_of(".") );
}

_file::~_file(){
	this->close();
}

bool _file::close(){
	if( this->mode != _fileModeOpen::closed || this->handle != nullptr ){
		if( fclose( this->handle ) != 0 )
			return false;
		this->handle = nullptr;
		this->mode = _fileModeOpen::closed;
	}
	this->openedFiles--;
	return true;
}

int _file::setAttrs( _fileAttributes attrs )
{	
	return FAT_setAttr( this->filename.c_str() , attrs );
}

bool _file::exists(){
	struct stat temp;
	return stat( this->filename.c_str() , &temp ) == 0;
}

bool _file::open( const char* mode ){

	//! Close the previous Handle
	if( !this->close() )
		return false;
	
	//! Open the File
	this->handle = fopen( this->filename.c_str() , mode );
	
	if( this->handle )
	{
		this->openedFiles++;
		return true;
	}
	return false;
}

inline bool _file::openread(){
	return this->open( "rb" ) && (this->mode = _fileModeOpen::read ); // Open for read, do not create
}

inline bool _file::openwrite(){
	return this->open( "rb+" ) && (this->mode = _fileModeOpen::write); // Open for read & write, do not create
}

inline bool _file::create( bool fullCreate ){
	if( this->exists() ) // Return if the file does already exist
		return false;
	if( fullCreate )
		return this->_createPath( this->filename , false );
	return this->open( "w" ) && this->close(); // Create and close
}

bool _file::_createPath( string path , bool isDir ){
	_file newFile = _file( path );
	
	printf("%s\n",path.c_str() );
	// If this directory already exists, return
	// TODO: Allow fat:/... syntax!
	//
	// path.substr( max( (int)path.find_first_of(":") , 0 ) , string::npos ) == "/"
	if( path == "/" || newFile.exists() ){
		printf("Returned!");
		return true;
	}
	else{
		//_system_->submit();
		_u32 delimiter = max( (int)path.find_last_of("/") , 1 );
		string fileName = path.substr( delimiter , string::npos );
		string leftPath = path.substr( 0 , delimiter );
		
		// Build everything "upstairs" (recursively!)
		if( !_file::_createPath( leftPath ) )
			return false;
		
		// Then create the file/directory
		
		_file* file = nullptr;
		if( !isDir )
			file = new _file( path );
		else
			file = new _directory( path );
		
		bool ret = file->create();
		
		delete file;
		
		return ret;
	}
		
}

bool _file::read( void* dest , _u32 size )
{
	if( size == -1 )
		size = this->getSize();
	
	_fileModeOpen modePrev = this->mode;
	
	if( this->mode == _fileModeOpen::closed && !this->openread() )
		return false;
	
	//! Set Iterator to beginning
	rewind( this->handle );
	fread( dest , 1 , size , this->handle );
	
	if( modePrev == closed )
		this->close();
	
	return 0 == ferror( this->handle ); // Check if there was an error	
}

bool _file::write( void* src , _u32 size )
{	
	_fileModeOpen modePrev = this->mode;
	
	if( this->mode == _fileModeOpen::closed && !this->openwrite() )
		return false;
	else if( this->mode == _fileModeOpen::read )
		return false;
	
	fwrite( src , size , 1 , this->handle );
	
	if( modePrev == closed )
		this->close();
	
	return true;
}

bool _file::writeString( string str )
{	
	_fileModeOpen modePrev = this->mode;
	
	if( this->mode == closed && !this->openwrite() )
		return false;
	else if( this->mode == _fileModeOpen::read )
		return false;
	
	fputs( str.c_str() , this->handle );
	
	if( modePrev == closed )
		this->close();
	
	return true;
}

string _file::readString( _u32 size ){
	
	if( size == -1 )
		size = this->getSize();
	
	_fileModeOpen modePrev = this->mode;
	
	if( this->mode == _fileModeOpen::closed && !this->openread() )
		return "";
	
	if( size <= 0 )
		return "";
	
	//! Output
	string out;
	
	// Temp...
	char* text = new char[size];
	
	// Read the Contents
	_u32 actualSize = fread( text , sizeof(_char) , size , this->handle );
	
	//! Copy 'text' into 'out'
	out = text;
	
	out.resize( actualSize );
	
	// Free temorary storage
	delete[] text;
	
	if( modePrev == _fileModeOpen::closed )
		this->close();
	
	return out;
}

_mimeType _file::getMimeType(){
	if( this->mimeType )
		return this->mimeType;
	return this->mimeType = _mimeType( this->getExtension() );
}

_u32 _file::getSize(){
	
	_u32 size = 0;
	
	//! Is the File already opened
	if( this->mode != _fileModeOpen::closed && this->handle != nullptr )
	{
		fpos_t lastpos; // Save last position of iterator
		fgetpos( this->handle , &lastpos );
		
		//! "Tell" Size
		rewind( this->handle );
		fseek ( this->handle , 0 , SEEK_END );
		size = ftell( this->handle );
		
		//! Restore iterator
		fsetpos( this->handle , &lastpos );
	}
	//! Open the file
	else if( this->openread() )
	{
		//! "Tell" Size
		fseek ( this->handle , 0 , SEEK_END );
		size = ftell( this->handle );
		
		//! Close file
		this->close();
	}
	return size;
}



bool _file::execute(){
	
	_mimeType mime = { this->getExtension() };
	switch( mime ){
		case _mime::application_octet_stream:
		case _mime::application_x_lua_bytecode:
			_system_->addProgram( new _progLua( this->readString() ) );
			break;
		default:
			return false;
	}
	return true;
}

const _bitmap* _file::getFileImage()
{
	switch( this->getMimeType() ){
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
			return icon_plain;
			break;
	}
}