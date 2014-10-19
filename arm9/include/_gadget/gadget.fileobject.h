// Check if already included
#ifndef _WIN_G_FILEOBJECT_
#define _WIN_G_FILEOBJECT_

#include <_type/type.gadget.h>
#include <_type/type.direntry.h>
#include <_gadget/gadget.label.h>
#include <_gadget/gadget.textbox.h>
#include <_dialog/dialog.file.delete.h>

enum class _fileViewType : _u8{
	detail = 0,
	list = 1,
	symbol = 3
};

class _fileObject : public _gadget {
	
	private:
	
		// My Data...
		_direntry*								file;
		static _uniquePtr<_textBox>				renameTextBox;
		static _fileObject*						renamedFile;
		static _uniquePtr<_fileDeleteDialog>	deleteDialog;
		static _fileObject*						deletedFile;
		_fileViewType							viewType;
		
		// Handler for specific events
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn updateHandler( _event );
		static _callbackReturn keyHandler( _event );
		static _callbackReturn clickHandler( _event );
		static _callbackReturn blurHandler( _event );
		static void 			deleteDialogHandler( _dialogResult );
		
		// Default Menu to Open on right Click
		static _uniquePtr<_menu>	defaultMenu;
		static void		defaultMenuHandler( _u16 listIndex , _u16 entryIndex );
		void			initializeMenu();
		void			initRename();
		void			finishRename();
		bool			isRenamed(){ return _fileObject::renameTextBox && _fileObject::renamedFile == this; }
		
	public:
	
		//! Ctor
		_fileObject( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , const string& file , _fileViewType viewtype = _fileViewType::list , _style&& style = _style() );
		
		//! Dtor
		~_fileObject();
		
		//! Get the _direntry instance that this _filobject refers to
		const _direntry& getDirentry(){
			return *this->file;
		}
		
		//! Execute this file
		void execute( _programArgs args = _programArgs() , bool openInNewWindow = false );
};

#endif