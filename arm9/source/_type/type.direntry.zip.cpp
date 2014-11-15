#include <_type/type.direntry.zip.h>
#include <_controller/controller.filesystem.h>
#include <_controller/controller.debug.h>

#undef MINIZ_HEADER_FILE_ONLY
#include <_library/library.zip.h>

bool _zipArchive::open( string mode ) const
{
	_direntryMode newMode = mode[0] == 'w' || mode[0] == 'a' ? _direntryMode::write : _direntryMode::read;
	
	//! Check if change is neccessary
	if( this->mode == newMode )
		return true;
	
	bool status;
	
	//! Close the previous Handle
	if( !this->close() )
		return false;
	
	if( newMode == _direntryMode::read ){
		memset( &this->archive , 0 , sizeof(this->archive) );
		status = mz_zip_reader_init_file( &this->archive , _direntry::getFileName().c_str() , MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY );
	}
	else if( this->getSize() ){ // newMode == write
		if( this->mode == _direntryMode::closed ){
			memset( &this->archive , 0 , sizeof(this->archive) );
			status = mz_zip_reader_init_file( &this->archive , _direntry::getFileName().c_str() , MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY );
			if( !status )
				return false;
		}
		status = mz_zip_writer_init_from_reader( &this->archive , _direntry::getFileName().c_str() );
	}
	else
		status = mz_zip_writer_init_file( &this->archive , _direntry::getFileName().c_str() , 0 );
	
	if( !status )
		return false;
	
	this->mode = newMode;
	
	return true;
}

bool _zipArchive::create()
{
	if( this->mode != _direntryMode::closed || this->isExisting() )
		return false;
	
	// Create file
	fclose( fopen( this->getFileName().c_str() , "wb" ) );
	
	return updateStats();
}

bool _zipArchive::close() const
{
	// Return, if everything is closed already
	if( this->mode == _direntryMode::closed )
		return true;
	
	// Close the archive, freeing any resources it was using
	if( this->mode == _direntryMode::read )
		mz_zip_reader_end( &this->archive );
	else{
		mz_zip_writer_finalize_archive( &this->archive );
		mz_zip_writer_end( &this->archive );
	}
	
	this->mode = _direntryMode::closed;
	
	return true;
}

bool _zipArchive::getEntryByIndex( _s32 index , _zipEntry& dest ) const
{
	// Open the Directory if necesary
	if( this->mode != _direntryMode::read && this->openRead() == false )
		return false;
	
	_zipFileStat	statStruct;
	
	bool result = mz_zip_reader_file_stat( &this->archive , index , &statStruct );
	
	if( result )
		dest = _zipEntry(
			index
			, statStruct.m_filename
			, statStruct.m_comment
			, mz_zip_reader_is_file_a_directory( &this->archive, index )
			, statStruct.m_comp_size
			, statStruct.m_uncomp_size
			, statStruct.m_time
		);
	else
		dest = _zipEntry();
	
	return result;
}

bool _zipArchive::getEntryByPath( _literal entryName , _zipEntry& dest ) const
{
	// Open the Directory if necesary
	if( this->mode != _direntryMode::read && this->openRead() == false )
		return false;
	
	// Remove leading '/'
	if( entryName[0] == '/' )
		entryName++;
	
	_s32 index = mz_zip_reader_locate_file( &this->archive , entryName , nullptr , MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY );
	
	if( index < 0 ){
		dest = _zipEntry();
		return false;
	}
	
	return this->getEntryByIndex( index , dest );
}

bool _zipArchive::getNextFile( _zipEntry& dest ) const
{
	// Open the Directory if necesary
	if( this->mode != _direntryMode::read && this->openRead() == false )
		return false;
	
	_s32 maxFiles = getNumEntries();
	
	if( this->dirIterator < maxFiles )
		return this->getEntryByIndex( this->dirIterator++ , dest );
	
	return false;
}

_u32 _zipArchive::getNumEntries() const
{
	// Open the Directory if necesary
	if( this->mode != _direntryMode::read && this->openRead() == false )
		return 0;
	
	return (_u32) mz_zip_reader_get_num_files( &this->archive );
}

bool _zipArchive::readNextChild( _zipEntry& child , string path ) const
{
	// Open the Directory if necesary
	if( this->mode != _direntryMode::read && this->openRead() == false )
		return false;
	
	// Remove leading '/'
	if( path.front() == '/' )
		path = path.substr( 1 );
	
	// Add trailing '/' if path is not empty
	if( path.length() && path.back() != '/' )
		path += '/';
	
	_zipEntry entry;
	
	while( this->getNextFile( entry ) )
	{
		if( entry.filename.compare( 0 , path.length() , path ) == 0 )
		{
			// Cut off the first part of the path, since we are already at some relative location
			entry.filename = entry.filename.substr( path.length() );
			
			// Search for a '/'
			size_t occourence = entry.filename.find('/');
			
			// If the '/' is not at the very end of the filename
			if( occourence != string::npos && occourence != entry.filename.length() - 1 )
				continue;
			
			if( entry.filename.back() == '/' )
				entry.filename.resize( entry.filename.length() - 1 );
			
			child = move(entry);
			return true;
		}
	}
	
	// We have not found any entry!
	return false;
}

bool _zipArchive::rewindChildren() const
{
	this->dirIterator = 0;
	return true;
}

bool	_zipArchive::copyEntryToZip( const _zipEntry& entry , _zipArchive& other ) const
{
	if( entry.index < 0 )
		return false;
	
	if( this->mode != _direntryMode::read && this->openRead() == false )
		return false;
	
	if( other.mode != _direntryMode::write && other.openWrite() == false )
		return false;
	
	return mz_zip_writer_add_from_zip_reader( &other.archive , &this->archive , entry.index );
}

bool	_zipArchive::readChild( const _zipEntry& entry , void* dest , _u64* numBytes ) const
{
	if( entry.index < 0 )
		return false;
	
	// Open the Directory if necesary
	if( this->mode != _direntryMode::read && this->openRead() == false )
		return false;
	
	if( mz_zip_reader_extract_to_mem( &this->archive , entry.index , dest , entry.size , MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY ) ){
		*numBytes = 0;
		return false;
	}
	
	*numBytes = entry.size;
	return true;
}

bool	_zipArchive::readChild( _literal entryName , void* dest , _u64* numBytes ) const
{
	_zipEntry entry;
	if( !this->getEntryByPath( entryName , entry ) )
		return false;
	
	return readChild( entry , dest , numBytes );
}

string	_zipArchive::readChildString( const _zipEntry& entry ) const
{
	// Open the Directory if necesary
	if( entry.index < 0 || ( this->mode != _direntryMode::read && this->openRead() == false ) )
		return string();
	
	_u32	size;
	
	void* data = mz_zip_reader_extract_to_heap( &this->archive , entry.index , &size , MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY );
	if( !data )
		return string();
	
	string output{ (_literal)data };
	
	mz_free(data);
	
	// Make sure we only have baytes that we want
	output.resize( entry.size );
	
	return output;
}

string	_zipArchive::readChildString( _literal entryName ) const
{
	_zipEntry entry;
	this->getEntryByPath( entryName , entry );
	return readChildString( entry );
}

bool	_zipArchive::unlinkChild( _zipEntry& entry , bool removeContents )
{
	if( entry.index < 0 )
		return false;
	
	// Open the Directory if necesary
	if( this->mode != _direntryMode::read && this->openRead() == false )
		return false;
	
	// Temporary variables needed if 'removeContents' is true
	removeContents &= entry.isDirectory;
	_u32 baseNameLength = entry.filename.length();
	
	// Create temporary zip file
	_zipStruct	tempArchive;
	memset( &tempArchive , 0 , sizeof(tempArchive) );
	string tempName = _direntry::getFileName() + ".tmp";
	mz_zip_writer_init_file( &tempArchive , tempName.c_str() , 0 );
	
	// Iterate through all files
	_s32 numEntries = this->getNumEntries();
	for( _s32 i = 0 ; i < numEntries ; i++ )
	{
		if( i == entry.index ){ // This entry is to be skipped
			if( this->dirIterator > i ) // Adjust directory iterator
				this->dirIterator--;
		}
		else{
			if( removeContents ){ // Skip this entry, because it is a child of the entry to delete
				_zipEntry curEntry;
				this->getEntryByIndex( i , curEntry );
				if( curEntry.filename.compare( 0 , baseNameLength , entry.filename ) == 0 )
					continue;
			}
			if( !mz_zip_writer_add_from_zip_reader( &tempArchive , &this->archive , i ) )
				goto windUp;
		}
	}
	
	mz_zip_writer_finalize_archive( &tempArchive );
	mz_zip_writer_end( &tempArchive );
	
	// Close reader and delete old file
	if( !this->close() || !this->unlink() )
		return false;
	
	// Rename the temp file to the original name
	std::rename( tempName.c_str() , this->getFileName().c_str() );
	
	// Update information
	_direntry::operator=( _direntry( this->getFileName() ) );
	
	return true;
	
	// If anything goes wrong
	windUp:
		mz_zip_writer_finalize_archive( &tempArchive );
		mz_zip_writer_end( &tempArchive );
		_direntry(tempName).unlink();
	return false;
}

bool	_zipArchive::unlinkChild( _literal entryName , bool removeContents )
{
	_zipEntry entry;
	this->getEntryByPath( entryName , entry );
	return unlinkChild( entry , removeContents );
}

bool	_zipArchive::renameChild( _zipEntry& entry , string newName ){
	return false;
}

bool	_zipArchive::renameChild( const string& entryName , string newName ){
	return false;
}

bool	_zipArchive::writeChild( _literal entryName , void* src , _u64 size , _literal comment , _u32 commentSize , _zipComp compression )
{
	// Remove leading '/'
	if( entryName[0] == '/' )
		entryName++;
	
	if( !commentSize && comment )
		commentSize = strlen( comment );
	
	// Open the Directory if necesary
	if( this->mode != _direntryMode::write && this->openWrite() == false )
		return false;
	
	// Write to zip
	return mz_zip_writer_add_mem_ex( &this->archive , entryName , src , size , comment , commentSize , (_u32)compression , 0 , 0 );
}

bool	_zipArchive::addFile( const _direntry& file , string path , _literal comment , _u32 commentSize , bool overwrite , _zipComp compression )
{
	// Open the Directory if necesary
	if( this->mode != _direntryMode::write && this->openWrite() == false )
		return false;
	
	if( !commentSize && comment )
		commentSize = strlen( comment );
	
	// Remove leading '/'
	if( path.front() == '/' )
		path = path.substr( 1 );
	
	// Also make it possible to add whole directories
	if( file.isDirectory() )
	{
		if( path.length() && path.back() != '/' )
			path += '/';
		else
			path += file.getFullName( true ) + '/';
		
		//printf("~ %s\n",path.c_str());
		//_debugController::submit();
		
		// Add the directory to the zip
		if( !mz_zip_writer_add_mem_ex( &this->archive, path.c_str() , nullptr , 0 , comment , commentSize , (_u32)compression , 0 , 0 ) )
			return false;
		
		// Add all files
		for( _literal str ; file.readChild( str ) != false ; )
		{
			// Create name under which the file is inserted
			string newName = path + _direntry(str).getFullName( true );
			
			// Insert the file into the directory
			if( !this->addFile( _direntry( file.getFileName() + str ) , newName , comment , commentSize , true , compression ) )
				return false;
		}
		
		return true;
	}
	
	// Generate the filename under which the file is saved
	if( !path.length() || path.back() == '/' )
		path += file.getFullName( true );
	
	//printf("+ %s,%s\n",file.getFileName().c_str(),path.c_str());
	//_debugController::submit();
	
	// Check if the file is already present
	_zipEntry entry;
	if( !overwrite && this->getEntryByPath( path.c_str() , entry ) )
		return false;
	
	// Add to zip
	return mz_zip_writer_add_file( &this->archive , path.c_str() , file.getFileName().c_str() , comment , commentSize , (_u32)compression );
}

bool	_zipArchive::extractEntry( const _zipEntry& entry , string destinationPath , bool overwrite )
{
	if( entry.index < 0 )
		return false;
	
	// Open the Directory if necesary
	if( this->mode != _direntryMode::write && this->openWrite() == false )
		return false;
	
	_direntry destination = _direntry( destinationPath );
	if( destination.isDirectory() )
		destination = _direntry( destination.getFileName() + entry.filename );
	
	// Check if the file is already present
	if( !overwrite && destination.isExisting() )
		return false;
	
	// Add to zip
	return mz_zip_reader_extract_to_file( &this->archive , entry.index , destination.getFileName().c_str() , 0 );
}

bool	_zipArchive::extractEntry( _literal entryName , string destinationPath , bool overwrite )
{
	_zipEntry entry;
	this->getEntryByPath( entryName , entry );
	return extractEntry( entry , destinationPath , overwrite );
}