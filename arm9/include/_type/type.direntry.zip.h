#ifndef _WIN_T_DIRENTRY_ZIPFILE_
#define _WIN_T_DIRENTRY_ZIPFILE_

#include <_type/type.direntry.h>
#define MINIZ_HEADER_FILE_ONLY
#include <_library/library.zip.h>
#include <string.h>

using _zipFileStat = mz_zip_archive_file_stat;
using _zipStruct = mz_zip_archive;

enum class _zipComp : _u8 {
	none = 0 ,
	speed = 1,
	normal = 5,
	size = 9,
	uber = 10, // Not zlib compatible
};

struct _zipEntry
{
	string	filename;
	string	comment;
	bool	isDirectory;
	_u64	sizeComp;
	_u64	size;
	_time	time;
	_s32	index;
	
	//! Ctor
	_zipEntry( _s32 index = -1 , string filename = "" , string comment = "" ,
		bool isDirectory = false , _u64 sizeComp = 0 , _u64 size = 0 , _time time = _time() ) :
		filename( move(filename) )
		, comment( move(comment) )
		, isDirectory( isDirectory )
		, sizeComp( sizeComp )
		, size( size )
		, time( move(time) )
		, index( index )
	{}
};

class _zipArchive : public _direntry
{
	private:
		
		mutable _zipStruct	archive;
		mutable _s32		dirIterator;
		
		// Opens the zip file
		bool open( string mode ) const override;
		
		// Fills dest with the next file in the iteration
		bool getNextFile( _zipEntry& dest ) const ;
		
		// Copies a file inside the zip into another zip
		bool copyEntryToZip( const _zipEntry& entry , _zipArchive& other ) const ;
	
	public:
		
		//! Ctor
		_zipArchive( string path ) :
			_direntry( move(path) )
			, dirIterator( 0 )
		{}
		
		//! Dtor
		~_zipArchive(){
			this->close();
		}
		
		//! Iterate through its children
		bool	readNextChild( _zipEntry& child , string path = "/" ) const ;
		bool	rewindChildren() const ;
		
		//! Closes the zip file
		bool	close() const override ;
		
		//! Creates an empty zip file
		bool	create() override ;
		
		//////////////////
		// Child access //
		//////////////////
		
		//! Sets the content of the supplied zip entry (creates it if it doesn't exist)
		bool	writeChild( _literal entryName , void* src , _u64 size , _literal comment = nullptr , _u32 commentSize = 0 , _zipComp compression = _zipComp::normal );
		bool	writeChild( _literal entryName , void* src , _u64 size , string comment , _zipComp compression = _zipComp::normal ){
			return writeChild( entryName , src , size , comment.c_str() , comment.length() , compression );
		}
		bool	writeChild( const _zipEntry& entry , void* src , _u64 size , _literal comment = nullptr , _u32 commentSize = 0 , _zipComp compression = _zipComp::normal ){
			return writeChild( entry.filename.c_str() , src , size , comment , commentSize , compression );
		}
		bool	writeChild( const _zipEntry& entry , void* src , _u64 size , string comment , _zipComp compression = _zipComp::normal ){
			return writeChild( entry.filename.c_str() , src , size , comment.c_str() , comment.length() , compression );
		}
		
		//! Sets the content of the supplied zip entry (creates it if it doesn't exist) to the supplied string
		bool	writeChildString( const _zipEntry& entry , string str , _literal comment = nullptr , _u32 commentSize = 0 , _zipComp compression = _zipComp::normal  ){
			return writeChild( entry , (void*)str.data() , str.length() , comment , commentSize );
		}
		bool	writeChildString( const _zipEntry& entry , string str , string comment , _zipComp compression = _zipComp::normal ){
			return writeChild( entry , (void*)str.data() , str.length() , comment.c_str() , comment.length() , compression );
		}
		
		bool	writeChildString( _literal entryName , string str , _literal comment = nullptr , _u32 commentSize = 0 , _zipComp compression = _zipComp::normal ){
			return writeChild( entryName , (void*)str.data() , str.length() , comment , commentSize , compression );
		}
		bool	writeChildString( _literal entryName , string str , string comment , _zipComp compression = _zipComp::normal ){
			return writeChild( entryName , (void*)str.data() , str.length() , comment.c_str() , comment.length() , compression );
		}
		
		
		//! Reads the contents of the specified zip entry to a buffer
		bool	readChild( const _zipEntry& entry , void* dest , _u64* numBytes = nullptr ) const ;
		bool	readChild( _literal entryName , void* dest , _u64* numBytes = nullptr ) const ;
		string	readChildString( const _zipEntry& entry ) const ;
		string	readChildString( _literal entryName ) const ;
		
		
		//! Returns the number of files inside the zip file
		_u32	getNumEntries() const ;
		
		
		//! Removes a child (if the child is a folder and removeContents is true, it automatically deletes all files in that folder)
		bool	unlinkChild( _zipEntry& entry , bool removeContents = false );
		bool	unlinkChild( _literal entryName , bool removeContents = false );
		
		
		//! Renames a child
		bool	renameChild( _zipEntry& entry , string newName );
		bool	renameChild( const string& entryName , string newName );
		
		
		//! Fills dest with data
		bool	getEntryByIndex( int index , _zipEntry& dest ) const ;
		bool	getEntryByPath( _literal entryName , _zipEntry& dest ) const ;
		
		/////////////////////////////
		// Extract/Import features //
		/////////////////////////////
		
		//! Adds the supplied _direntry to the zip-file
		bool	addFile( const _direntry& file , string path = "/" , _literal comment = nullptr , _u32 commentSize = 0 , bool overwrite = false , _zipComp compression = _zipComp::normal );
		bool	addFile( const _direntry& file , string path , string comment , bool overwrite = false , _zipComp compression = _zipComp::normal ){
			return addFile( file , path , comment.c_str() , comment.length() , overwrite , compression );
		}
		
		//! Extracts the given zip entry to disk at the specified path
		bool	extractEntry( _literal entryName , string destinationPath , bool overwrite = false );
		bool	extractEntry( const _zipEntry& entry , string destinationPath , bool overwrite = false ); 
};

#endif