#ifndef _WIN_T_DIRENTRY_
#define _WIN_T_DIRENTRY_

#include <stdio.h>
#include <list>
#include <sys/dir.h>

#include "fat.h"
#include "_type/type.h"
#include "_type/type.shortstring.h"
#include "_type/type.bitmap.h"
#include "_type/type.mime.h"

// Libfat
extern "C"{
#include "_library/_fat/partition.h"
#include "_library/_fat/fatdir.h"
}

enum class _direntryMode : _u8{
	closed = 0,
	read = 1,
	write = 2
};

union _direntryAttributes
{
	_u8 sum;
	
	// Bitset
	struct{
		_u8	readonly 	:1;
		_u8 hidden 		:1;
		_u8 system		:1;
		_u8 volume		:1;
		_u8 directory	:1; // Do not change this value!
		_u8 archive		:1;
	} __attribute__(( packed ));
	
	//! Ctor
	_direntryAttributes( _u8 val = 0 ) :
		sum( val ){}
	
	//! Convert to _u8
	operator _u8(){
		return this->sum;
	}
};


class _direntry{
	
	protected:
	
		//! Handle to File/Directory
		mutable union{
			FILE* 		fHandle;
			DIR* 		dHandle;
		};
		
		//! Stat buffer to fill with stat(...)
		struct stat			stat_buf;
		
		//! Flag indicating if the file exists
		bool				exists;
		
		//! Falg indicating if the FAT32-System is ready to be used
		static int			fatInited;
		
		//! My Filename
		string				filename;
		string				name;
		ssstring			extension;
		
		_mimeType 			mimeType;
		
		//! The Mode the file was opened by
		_direntryMode		mode;
		
		//! Get PARTITION and DIR_ENTRY
		bool getEntry( PARTITION* &partition , DIR_ENTRY* &dirEntry );
		
		//! Flush a files contents to disk
		bool flush();
		
	public:
	
		//! Manually init Fat
		static bool initFat();
		
		
		//! Check if File is Direct
		bool isDirectory() const { return S_ISDIR( this->stat_buf.st_mode ); }
		
		
		//! Ctor...
		_direntry( string&& path );
		_direntry( const string& path ) :
			_direntry( string( path ) )
		{ }
		
		//! Move Ctor...
		_direntry( _direntry&& other ){
			*this = move( other );
		}
		
		//! Move Ctor...
		_direntry( const _direntry& other ){
			*this = other;
		}
		
		//! Copy and move operators
		_direntry& operator=( const _direntry& other );
		_direntry& operator=( _direntry&& other );
		
		//! Dtor...
		virtual ~_direntry(){ this->close(); }
		
		
		//! get mode (used to determine if the file is openend)
		_direntryMode getMode() const { return this->mode; }
		
		
		//! Open a file. The Mode for opening is specified with mode
		virtual bool open( string mode = "" );
		
		
		//! open the file for reading
		virtual bool openread();
		
		
		//! open the file for writing
		virtual bool openwrite( bool eraseOldContent = false );
		
		
		//! Create a file with the specified filename
		virtual bool create();
		
		
		//! Close the File
		virtual bool close();
		
		
		//! Read the contents of the file into a block of memory
		virtual bool read( void* dest , _u32 size = -1 );
		virtual bool write( void* src , _u32 size );
		
		//! Hide the File
		/*bool hide(){ return this->setAttrs( []( _direntryAttributes attr )->_direntryAttributes{ attr.attr.hidden = true; return attr; } ); }
		bool unhide(){ return this->setAttrs( []( _direntryAttributes attr )->_direntryAttributes{ attr.attr.hidden = false; return attr; } ); }*/
		
		//! Set The File to Syste-File
		/*bool setSystem(){ return this->setAttrs( []( _direntryAttributes attr )->_direntryAttributes{ attr.attr.system = true; return attr; } ); }
		bool unsetSystem(){ return this->setAttrs( []( _direntryAttributes attr )->_direntryAttributes{ attr.attr.system = false; return attr; } ); }*/
		
		//! If the _direntry is an directory, iterate through its children
		virtual bool readChild( string& str );
		virtual bool rewindChildren();
		
		
		//! Set UNIX-Attributes of File
		int setAttrs( _direntryAttributes attrs );
		
		
		//! Get UNIX-Attributes of File
		_direntryAttributes getAttrs() const ;
		
		
		//! Write an std::string to the end of the file
		bool writeString( string str );
		
		
		//! Read the contents of the file into std::string
		string readString( _u32 size = 0 );
		
		
		//! Get Filename
		const string& getFileName() const { return this->filename; }
		
		
		//! get Name of the file (not "C:/Hello.txt", but "Hello")
		virtual const string& getName() const { return this->name; }
		
		
		//! get the string that should be displayed if the direntry-name should be drawn
		string getDisplayName() const ;
		
		
		//! get Extension of the file (not "C:/Hello.txt", but "txt")
		virtual const ssstring& getExtension() const { return this->extension; }
		
		
		//! Get Mime Type
		virtual _mimeType getMimeType() const { return this->mimeType; }
		
		
		//! get the size of a file (in bytes)
		virtual _u32 getSize();
		
		
		//! Execute That File (arguments passed are only applied, if the file to execute is a program)
		virtual bool execute( _cmdArgs&& args = _cmdArgs() );
		bool execute( const _cmdArgs& args ){ return this->execute( _cmdArgs( args ) ); }
		
		
		//! Get File-Image
		virtual _bitmap getFileImage();
		
		
		//! Check if the File at 'filename' exists
		virtual bool isExisting() const { return this->exists; }		
		
		
		//! To remove the file/directory
		//! Pass 'true' to delete a non-empty directory
		virtual bool unlink( bool removeContents = false );
		
		
		//! To rename the file
		virtual bool rename( string newName );
		
		
		//! Replace associated filename-patterns
		static string replaceASSOCS( string&& path ){
			string path2 = move( path );
			replaceASSOCS( path2 );
			return path2;
		}
		static string replaceASSOCS( const string& path ){
			string path2 = path;
			replaceASSOCS( path2 );
			return path2;
		}
		static void replaceASSOCS( string& path );
};

extern _direntry _diskRoot_;

#endif