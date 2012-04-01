#include "_type/type.file.h"
#include "_type/type.mime.h"
#include "_type/type.system.h"
extern "C"{
#include "_file/_fat/partition.h"
#include "_file/_fat/fatfile.h"
#include "_file/_fat/file_allocation_table.h"
}

bool _file::fatInited = false;
_u8 _file::openedFiles = 0;

_file::_file( string fn ) :
	handle( nullptr )
	, filename( fn )
	, mode( closed )
{ 
	if( !this->fatInited )
		this->fatInited = fatInitDefault();
}

_file::~_file(){
	this->close();
}

bool _file::close(){
	if( this->mode != closed || this->handle != nullptr ){
		if( fclose( this->handle ) != 0 )
			return false;
		this->handle = nullptr;
		this->mode = closed;
	}
	this->openedFiles--;
	return true;
}

int _file::setAttrs( _fileAttributes attrs )
{	
	return FAT_setAttr( this->filename.c_str() , attrs );
}

bool _file::exists(){
	return this->open( "rb" ) && this->close();
}

bool _file::open( const char* mode ){

	//! Close the previous Handle
	if( !this->close() )
		return false;
	
	//! Open the File
	this->handle = fopen( this->filename.c_str() , mode );
	
	if( this->handle ){
		this->openedFiles++;
		return true;
	}
	return false;
}

inline bool _file::openread(){
	return this->open( "rb" ) && (this->mode = read ); // Open for read, do not create
}

inline bool _file::openwrite(){
	return this->open( "rb+" ) && (this->mode = write); // Open for read & write, do not create
}

inline bool _file::create(){
	if( this->exists() ) // Return if the file does already exist
		return false;
	return this->open( "w" ) && this->close(); // Create and close
}

bool _file::readTo( void* dest , _u32 size )
{
	if( size == -1 )
		size = this->getSize();
	
	_fileModeOpen modePrev = this->mode;
	
	if( this->mode == closed && !this->openread() )
		return false;
	
	//! Set Iterator to beginning
	rewind( this->handle );
	fread( dest , 1 , size , this->handle );
	
	if( modePrev == closed )
		this->close();
	
	return 0 == ferror( this->handle ); // Check if there was an error	
}


string _file::readString( _u32 size ){
	
	if( size == -1 )
		size = this->getSize();
	
	_fileModeOpen modePrev = this->mode;
	
	if( this->mode == closed && !this->openread() )
		return "";
	
	if( size <= 0 )
		return "";
	
	//! Output
	string out;
	
	// Temp...
	char* text = new char[size];
	
	if( text == nullptr )
		return "";
	
	// Read the Contents
	_u32 actualSize = fread( text , sizeof(_char) , size , this->handle );
	
	//! Copy 'text' into 'out'
	out = text;
	
	out.resize( actualSize );
	
	// Free temorary storage
	delete[] text;
	
	if( modePrev == closed )
		this->close();
	
	return out;
}

_u32 _file::getSize(){
	
	_u32 size = 0;
	
	//! Is the File already opened
	if( this->mode != closed && this->handle != nullptr )
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

string _file::getName()
{ 
	this->name = this->filename.substr( this->filename.find_last_of("/") + 1 );
	this->name = this->name.substr( 0 , this->name.find_last_of(".") );
	return this->name;
}

string _file::getExtension()
{ 
	_coord pos = this->filename.find_last_of(".");
	_coord npos = this->filename.find_last_of("/");
	
	if( pos > npos && pos != this->filename.npos )
		return this->extension = this->filename.substr( pos + 1 );
	return "";
}



void _file::execute(){
	
	_mimeType mime = { this->getExtension() };
	switch( mime ){
		case _mime::application_octet_stream:
			_system_->addProgram( new _program( this->readString() ) );
			break;
		default:
			break;
	}
}