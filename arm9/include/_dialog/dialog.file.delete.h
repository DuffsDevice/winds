#ifndef _WIN_D_FILE_DELETE_
#define _WIN_D_FILE_DELETE_

#include <_dialog/dialog.image.h>
#include <_type/type.direntry.h>

//! File Delete - Dialog
class _fileDeleteDialog : public _imageDialog
{
	private:
		
		string fileToDelete;
		
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_fileDeleteDialog( _direntry fileToDelete );
		
		//! Get the path of the file that is being asked for
		string getFileName(){ return fileToDelete; }
};

#endif