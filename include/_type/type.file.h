#ifndef _WIN_T_FILE_
#define _WIN_T_FILE_

#include "_type/type.h"
#include <string.h>
#include <stdio.h>
#include <functional>
using namespace std;
#include "fat.h"

// Libfat
extern "C"{
#include "_file/_fat/partition.h"
#include "_file/_fat/fatdir.h"
}

enum _fileModeOpen{
	closed = 0,
	read = 1,
	write = 2
};

union _fileAttributes{
	_u8 sum;	
	// Bitset
	struct{
		public:
			_u8	readonly 	:1;
			_u8 hidden 		:1;
			_u8 system		:1;
			_u8 volume		:1;
		private:
			_u8 directory	:1;
		public:
			_u8 archive		:1;
	} __attribute__(( packed )) attr;
	
	_fileAttributes( _u8 val ) : sum( val ){}
	_fileAttributes() : sum( 0 ){}
	operator _u8(){
		return this->sum;
	}
};


class _file{
	
	protected:
	
		//! handle to File
		FILE* handle;
		
		//! My Filename
		string filename;
		string name;
		string extension;
		
		//! The Mode the file was opened by
		_fileModeOpen mode;
		
		static bool fatInited;
		
		static _u8 	openedFiles;
		
		// Get PARTITION and DIR_ENTRY
		bool getEntry( PARTITION* &partition , DIR_ENTRY* &dirEntry );
		
	public:
	
		// Check if File is Direct
		virtual bool isDirectory(){ return false; }
		
		//! Ctor...
		_file( string path );
		
		//! Dtor...
		~_file();
		
		//! Open a file. The Mode for opening is specified with mode
		virtual bool open( const char* mode );
		
		//! open the file for reading
		virtual bool openread();
		
		//! open the file for writing
		virtual bool openwrite();
		
		//! Create a file with the specified filename
		virtual bool create();
		
		//! Check if the File at 'filename' exists
		virtual bool exists();
		
		//! Close the File
		virtual bool close();
		
		//! Read the contents of the file into a block of memory
		virtual bool readTo( void* dest , _u32 size = -1 );
		
		//! Hide the File
		/*bool hide(){ return this->setAttrs( []( _fileAttributes attr )->_fileAttributes{ attr.attr.hidden = true; return attr; } ); }
		bool unhide(){ return this->setAttrs( []( _fileAttributes attr )->_fileAttributes{ attr.attr.hidden = false; return attr; } ); }
		
		//! Set The File to Syste-File
		bool setSystem(){ return this->setAttrs( []( _fileAttributes attr )->_fileAttributes{ attr.attr.system = true; return attr; } ); }
		bool unsetSystem(){ return this->setAttrs( []( _fileAttributes attr )->_fileAttributes{ attr.attr.system = false; return attr; } ); }*/
		
		//! Set Attributes of File
		int setAttrs( _fileAttributes attrs );
		
		//! Read the contents of the file into std::string
		virtual string readString( _u32 size = -1 );
		
		// get Name of the file (not "C:/Hello.txt", but "Hello")
		virtual string getName();
		
		// get Extension of the file (not "C:/Hello.txt", but "txt")
		virtual string getExtension();
		
		//! get the size of a file (in bytes)
		virtual _u32 getSize();
		
		//! Execute That File
		virtual void execute();
};

#endif