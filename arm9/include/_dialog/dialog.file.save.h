#ifndef _WIN_D_ENTERTEXT_
#define _WIN_D_ENTERTEXT_

#include "_type/type.dialog.h"
#include "_type/type.shortstring.h"
#include "_gadget/gadget.button.h"
#include "_gadget/gadget.button.action.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.window.h"
#include "_gadget/gadget.fileview.h"
#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.select.h"

typedef _assocVector<_int,_tuple<string,ssstring>> _fileTypeList;

class _fileSaveDialog : public _dialog
{
	private:
		
		_button*		saveButton;
		_button*		cancelButton;
		_label*			fileNameLabel;
		_label*			fileTypeLabel;
		_textBox*		fileNameBox;
		_window*		window;
		_select*		fileTypeChooser;
		_fileView*		fileView;
		_textBox*		fileViewAddress;
		_actionButton*	gotoButton;
		string			initialName;
		_fileTypeList	fileTypes;
		
		_callbackReturn eventHandler( _event );
		
		void executeInternal();
		void cleanupInternal();
		
		const _menuEntryList generateMenuList(){
			_menuEntryList menuList;
			for( auto& value : fileTypes ){
				string& val = menuList[value.first];
				val.swap( std::get<0>(value.second) );
				val += " (*.";
				val += std::get<1>(value.second).c_str();
				val += ")";
			}
			return move(menuList);
		}
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_fileSaveDialog( _fileTypeList possibleFileExtensions , _optValue<string> initialName = ignore , _int initialFileExtension = 0 , _optValue<string> saveLabel = ignore );
		
		
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
			return this->fileNameBox->getStrValue();
		}
		
		//! Get a direntry to directly write to
		_direntry getDirentry(){
			return _direntry( this->fileView->getPath() + this->getFileName() );
		}
		
		//! Dtor
		~_fileSaveDialog(){
			delete this->saveButton;
			delete this->cancelButton;
			delete this->fileNameLabel;
			delete this->fileTypeLabel;
			delete this->fileNameBox;
			delete this->fileTypeChooser;
			delete this->window;
		}
};

#endif