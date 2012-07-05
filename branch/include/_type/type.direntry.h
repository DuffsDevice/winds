#ifndef _WIN_T_DIRENTRY_
#define _WIN_T_DIRENTRY_

#include <string.h>
#include <stdio.h>
#include <list>
#include <sys/dir.h>
using namespace std;

#include "fat.h"
#include "_type/type.h"
#include "_type/type.bitmap.h"
#include "_type/type.mime.h"

// Libfat
extern "C"{
#include "_library/_fat/partition.h"
#include "_library/_fat/fatdir.h"
}

#define FAT_EMULATORm

typedef enum{
	mode_closed = 0,
	mode_read = 1,
	mode_write = 2
}_direntryMode;

union _direntryAttributes{
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
	
	_direntryAttributes( _u8 val ) : sum( val ){}
	_direntryAttributes() : sum( 0 ){}
	operator _u8(){
		return this->sum;
	}
};


class _direntry{
	
	protected:
	
		//! Handle to File/Directory
		FILE* 		fHandle;
		DIR* 		dHandle;
		
		struct stat stat_buf;
		
		bool		exists;
		
		static int	fatInited;
		
		//! My Filename
		string		filename;
		string		name;
		string		extension;
		
		_mimeType 	mimeType;
		
		//! The Mode the file was opened by
		_direntryMode mode;
		
		// Get PARTITION and DIR_ENTRY
		bool getEntry( PARTITION* &partition , DIR_ENTRY* &dirEntry );
		
	public:
	
		// Check if File is Direct
		inline bool isDirectory() const { return S_ISDIR( this->stat_buf.st_mode ); }
		
		//! Ctor...
		_direntry( string path );
		
		//! Dtor...
		~_direntry(){
			this->close();
		}
		
		//! get mode (used to determine if the file is openend)
		_direntryMode getMode(){ return this->mode; }
		
		//! Open a file. The Mode for opening is specified with mode
		virtual bool open( const char* mode );
		
		//! open the file for reading
		virtual bool openread();
		
		//! open the file for writing
		virtual bool openwrite( bool eraseContent = false );
		
		//! Create a file with the specified filename
		virtual bool create();
		
		//! Close the File
		virtual bool close();
		
		//! Read the contents of the file into a block of memory
		virtual bool read( void* dest , _u32 size = -1 );
		virtual bool write( void* src , _u32 size );
		
		//! Hide the File
		/*bool hide(){ return this->setAttrs( []( _direntryAttributes attr )->_direntryAttributes{ attr.attr.hidden = true; return attr; } ); }
		bool unhide(){ return this->setAttrs( []( _direntryAttributes attr )->_direntryAttributes{ attr.attr.hidden = false; return attr; } ); }
		
		//! Set The File to Syste-File
		bool setSystem(){ return this->setAttrs( []( _direntryAttributes attr )->_direntryAttributes{ attr.attr.system = true; return attr; } ); }
		bool unsetSystem(){ return this->setAttrs( []( _direntryAttributes attr )->_direntryAttributes{ attr.attr.system = false; return attr; } ); }*/
		
		//! If the _direntry is an directory, iterate through its children
		virtual bool readChild( string& str );
		virtual bool rewindChildren();
		
		//! Set Attributes of File
		int setAttrs( _direntryAttributes attrs );
		
		//! Write an std::string to the end of the file
		bool writeString( string str );
		
		//! Read the contents of the file into std::string
		string readString( _u32 size = 0 );
		
		//! Get Filename
		string getFileName(){ return this->filename; }
		
		// get Name of the file (not "C:/Hello.txt", but "Hello")
		virtual string getName(){ return this->name; }
		
		// get Extension of the file (not "C:/Hello.txt", but "txt")
		virtual string getExtension(){ return this->extension; }
		
		virtual _mimeType getMimeType();
		
		//! get the size of a file (in bytes)
		virtual _u32 getSize();
		
		//! Execute That File
		virtual bool execute();
		
		//! Get File-Image
		virtual const _bitmap* getFileImage();
		
		//! Check if the File at 'filename' exists
		virtual bool isExisting(){ return this->exists; }
		
		_direntry& operator=( _direntry other ){
			this->close();
			this->filename = other.filename;
			this->name = other.name;
			this->extension = other.extension;
			this->stat_buf = other.stat_buf;
			this->mimeType = other.mimeType;
			this->mode = _direntryMode::mode_closed;
			return *this;
		}
		
		//! Get cwd
		static string getWorkingDirectory();
		
		//! Set cwd
		static void setWorkingDirectory( string dir );
		
		//! To remove the file
		virtual bool unlink();
		
		//! To rename the file
		virtual bool rename( string newName );
};

extern _direntry _diskRoot_;

#endif