#ifndef _WIN_D_FOLDERCHOOSE_
#define _WIN_D_FOLDERCHOOSE_

#include <_type/type.dialog.h>
#include <_gadget/gadget.button.h>
#include <_gadget/gadget.label.h>
#include <_gadget/gadget.window.dialog.h>
#include <_gadget/gadget.filetree.h>

class _folderChooseDialog : public _dialog
{
	private:
		
		_button*		okButton;
		_button*		cancelButton;
		_label*			descriptionLabel;
		_dialogWindow*	window;
		_fileTree*		fileTree;
		
		// Path to currently selected folder
		string			folderPath;
		
		_callbackReturn eventHandler( _event );
		
		void executeInternal();
		void cleanupInternal();
		
	
	public:
		
		//! Ctor
		//! @note if 'ignore'/nothing is passed as argument, the appropriate localized string is inserted instead
		_folderChooseDialog( _optValue<string> descriptionLabel = ignore , _optValue<string> okLabel = ignore , _optValue<string> windowLabel = ignore );
		
		//! Get Selected filename
		string getPath() const {
			return this->folderPath;
		}
		
		//! Dtor
		~_folderChooseDialog(){
			delete this->okButton;
			delete this->cancelButton;
			delete this->window;
			delete this->fileTree;
			if( this->descriptionLabel )
				delete this->descriptionLabel;
		}
};

#endif