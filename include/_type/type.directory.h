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
		
		_directory( string filename ) : _file( filename ) {}
		
		bool open();
		
		bool readChildren();
		
		bool close();
		
		bool sortChildren();
		
		deque<_file*>::iterator begin(){ return this->children.begin(); }
		
		deque<_file*>::iterator end(){ return this->children.end(); }
};

extern _directory* _diskRoot_;

#endif