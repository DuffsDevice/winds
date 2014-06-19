#ifndef _WIN_T_DIRENTRY_
#define _WIN_T_DIRENTRY_

#include <stdio.h>
#include <sys/dir.h>

#include "_type/type.h"
#include "_type/type.bitmap.h"
#include "_type/type.mime.h"
#include "_type/type.ini.h"
#include "_type/type.program.args.h"

//! Statistics in bytes of the mounted device
struct _driveStats{
	_u64 blockSize;
	_u64 size;
	_u64 free;
	_u64 used;
};

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
	}PACKED;
	
	//! Ctor
	_direntryAttributes( _u8 val = 0 ) :
		sum( val )
	{}
	
	//! Convert to _u8
	operator _u8(){
		return this->sum;
	}
};

typedef _vector<string> _fileExtensionList;

class _direntry
{
	protected:
	
		//! Handle to File/Directory
		union{
			mutable FILE* 		fHandle;
			mutable DIR* 		dHandle;
		};
		
		//! Stat buffer to fill with stat(...)
		struct stat				stat_buf;
		
		//! Flag indicating if the file exists
		bool					exists;
		
		//! Filename
		string					filename;
		string					name;
		string					extension;
		
		//! MIME-Type
		_mimeType 				mimeType;
		
		//! The Mode the file was opened by
		mutable _direntryMode	mode;
		
		//! Flush a files contents to disk
		bool flush() const ;
		
		//! Set UNIX-Attributes of File
		bool setAttrs( _direntryAttributes attrs );
		
		//! Get UNIX-Attributes of File
		_direntryAttributes getAttrs() const ;
		
		//! Open a file. The Mode for opening is specified with mode
		virtual bool open( string mode ) const ;
		
	public:
		
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
		
		
		/////////////////////////
		// Direntry Attributes //
		/////////////////////////
		
		//! Set whether the _direntry is hidden, returns 'true' on success
		bool setHidden( bool hidden );
		
		//! Check, whether the file is hidden
		bool isHidden() const { return this->getAttrs().hidden; }
		
		//! Set whether the _direntry is a system file, returns 'true' on success
		bool setSystemFile( bool isSystem );
		
		//! Check whether the file is a system file
		bool isSystem() const { return this->getAttrs().system; }
		
		//! Set whether the _direntry implements read only access, returns 'true' on success
		bool setReadOnly( bool readOnly );
		
		//! Check whether the file is readOnly
		bool isReadOnly() const { return this->getAttrs().readonly; }
		
		//! Check if File is a Directory
		bool isDirectory() const { return S_ISDIR( this->stat_buf.st_mode ); }
		
		
		//////////////////////////
		// Basic Access Methods //
		//////////////////////////
		
		//! get mode (used to determine if the file is openend)
		_direntryMode getMode() const { return this->mode; }
		
		//! Open the file for reading
		virtual bool openread() const ;
		
		//! open the file for writing
		virtual bool openwrite( bool eraseOldContent = true );
		
		//! Create a file with the specified filename
		virtual bool create();
		
		//! Close the File
		virtual bool close() const ;
		
		//! Read the contents of the file into a block of memory
		//! and sets 'numBytes' to the number of bytes efficiently read (-1 for an error)
		virtual bool read( void* dest , _optValue<_u32> size = ignore , _u32* numBytes = nullptr ) const ;
		
		//! Write 'size' bytes of contents in 'src' to the file
		virtual bool write( void* src , _u32 size );
		
		//! If the _direntry is an directory, iterate through its children
		virtual bool readChild( _literal& child , _fileExtensionList* allowedExtensions = nullptr ) const ;
		virtual bool readChildFolderOnly( _literal& child ) const ;
		virtual bool rewindChildren() const ;
		
		//! Write an std::string to the end of the file
		bool writeString( string str );
		
		//! Read the contents of the file into std::string
		string readString( _optValue<_u32> size = ignore ) const ;
		
		//! Get Filename
		const string& getFileName() const { return this->filename; }
		
		//! get Name of the file (not "C:/Hello.txt", but "Hello")
		virtual const string& getName() const { return this->name; }
		
		//! Get the string that should be displayed if the direntry-name should be drawn
		string getDisplayName( bool forceRealName = false ) const ;
		
		//! get Extension of the file (not "C:/Hello.txt", but "txt")
		virtual const string& getExtension() const { return this->extension; }
		
		//! Get Mime Type
		_mimeType getMimeType() const { return this->mimeType; }
		
		//! get the size of a file (in bytes)
		_u32 getSize() const ;
		
		//! Get the Path of the parent directory
		string getParentDirectory() const ;
		
		//! Execute That File (arguments passed are only applied, if the file to execute is a program)
		virtual bool execute( _programArgs args = _programArgs() );
		
		//! Get File-Image
		virtual _bitmap getFileImage() const ;
		
		//! Check if the File at 'filename' exists
		virtual bool isExisting() const { return this->exists; }
		
		//! To remove the file/directory
		//! Pass 'true' to delete a non-empty directory
		bool unlink( bool removeContents = false );
		
		//! To rename the file
		bool rename( string newName );
};

extern _direntry _diskRoot_;

#endif