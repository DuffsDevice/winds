#ifndef _WIN_T_DIRECTORY_
#define _WIN_T_DIRECTORY_

#include "_type/type.file.h"
#include <deque>
#include <sys/dir.h>
using namespace std;

// A Directory is a File(Entry), that has children
class _directory : public _file{
	
	private:
	
		deque<_file*>	children;
		
		DIR* handle;
		
	public:
	
		// Overwrite 'isDirectory' of _file
		bool isDirectory(){ return true; }
		
		// Constructor
		_directory( string filename ) : _file( filename ) {}
		
		// Open the directory connection
		bool open();
		
		// create the directory
		bool create( bool fullCreate = false );
		
		// fill 'children'-vector
		bool readChildren();
		
		// Close handle
		bool close();
		
		// Sort children Alphabetical
		bool sortChildren();
		
		// Get Iterators
		deque<_file*>::iterator begin(){ return this->children.begin(); }
		
		// Get Iterators
		deque<_file*>::iterator end(){ return this->children.end(); }
		
		// ""
		string getExtension();
		
		//! Get File-Image
		const _bitmap* getFileImage();
};

extern _directory* _diskRoot_;

#endif