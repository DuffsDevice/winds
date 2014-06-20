#ifndef _WIN_D_FILEOPEN_
#define _WIN_D_FILEOPEN_

#include "_type/type.dialog.h"
#include "_type/type.shortstring.h"
#include "_gadget/gadget.button.h"
#include "_dialog/dialog.file.save.h"
#include "_gadget/gadget.button.action.h"
#include "_gadget/gadget.button.image.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.window.dialog.h"
#include "_gadget/gadget.fileview.h"
#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.select.h"

class _fileOpenDialog : public _dialog
{
	private:
		
		_button*		openButton;
		_button*		cancelButton;
		_label*			fileNameLabel;
		_textBox*		fileNameBox;
		_window*		window;
		_select*		fileTypeChooser;
		_fileView*		fileView;
		_textBox*		fileViewAddress;
		_actionButton*	gotoButton;
		_imageButton*	folderUpButton;
		_fileTypeList	fileTypes;
		
		_callbackReturn eventHandler( _event );
		
		void executeInternal();
		void cleanupInternal();
		
		const _menuEntryList generateMenuList();
		_fileExtensionList getFileMask( _int value ) const ;
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_fileOpenDialog( _fileTypeList possibleFileExtensions , string initialFilePath = "" , _int initialFileExtension = 0 , _optValue<string> openLabel = ignore , _optValue<string> windowLabel = ignore );
		
		
		//! Get Index of the selected entry in 'possibleFileExtensions'
		_int getFileType(){
			return this->fileTypeChooser->getIntValue();
		}
		
		//! Get selected Mime-Type
		_mimeType getMimeType(){
			return _mimeType::fromExtension( std::get<1>(this->fileTypes[this->getFileType()]) );
		}
		
		//! Get Selected filename
		string getFileName(){
			return _direntry( this->fileView->getPath() ).getFileName() + this->fileNameBox->getStrValue();
		}
		
		//! Dtor
		~_fileOpenDialog(){
			delete this->openButton;
			delete this->cancelButton;
			delete this->fileNameLabel;
			delete this->fileNameBox;
			delete this->window;
			delete this->fileTypeChooser;
			delete this->fileView;
			delete this->fileViewAddress;
			delete this->gotoButton;
			delete this->folderUpButton;
		}
};

#endif