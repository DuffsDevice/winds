#ifndef _WIN_D_FILESAVE_
#define _WIN_D_FILESAVE_

#include <_type/type.dialog.h>
#include <_gadget/gadget.button.h>
#include <_gadget/gadget.button.action.h>
#include <_gadget/gadget.button.image.h>
#include <_gadget/gadget.label.h>
#include <_gadget/gadget.window.dialog.h>
#include <_gadget/gadget.fileview.h>
#include <_gadget/gadget.textbox.h>
#include <_gadget/gadget.select.h>

typedef _assocVector<_int,_pair<wstring,string>> _fileTypeList;

class _fileSaveDialog : public _dialog
{
	private:
		
		_button*		saveButton;
		_button*		cancelButton;
		_label*			fileNameLabel;
		_label*			fileTypeLabel;
		_textBox*		fileNameBox;
		_dialogWindow*	window;
		_select*		fileTypeChooser;
		_fileView*		fileView;
		_textBox*		fileViewAddress;
		_actionButton*	gotoButton;
		_imageButton*	folderUpButton;
		string			initialName;
		_fileTypeList	fileTypes;
		
		_callbackReturn eventHandler( _event );
		
		void executeInternal();
		void cleanupInternal();
		
		const _menuEntryList generateMenuList(){
			_menuEntryList menuList;
			for( auto& value : fileTypes ){
				wstring& val = menuList[value.first].text;
				val.swap( value.second.first );
				val += " (.";
				val += value.second.second;
				val += ")";
			}
			fileTypes.clear();
			return move(menuList);
		}
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_fileSaveDialog( _fileTypeList possibleFileExtensions , _optValue<string> initialFileName = ignore , _int initialFileExtension = 0 , _optValue<wstring> saveLabel = ignore , _optValue<wstring> windowLabel = ignore );
		
		
		//! Get Index of the selected entry in 'possibleFileExtensions'
		_int getFileType() const {
			return this->fileTypeChooser->getIntValue();
		}
		
		//! Get selected Mime-Type
		_mimeType getMimeType() const {
			return _mimeType::fromExtension( std::get<1>(this->fileTypes[this->getFileType()]) );
		}
		
		//! Get Selected filename
		string getFileName() const {
			return _direntry( this->fileView->getPath() ).getFileName() + this->fileNameBox->getStrValue().cpp_str();
		}
		
		//! Dtor
		~_fileSaveDialog(){
			delete this->saveButton;
			delete this->cancelButton;
			delete this->fileNameLabel;
			delete this->fileTypeLabel;
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