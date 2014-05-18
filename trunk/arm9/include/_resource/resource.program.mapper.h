#ifndef _WIN_PROG_MAPPER_
#define _WIN_PROG_MAPPER_

#include "_type/type.program.c.h"
#include "_gadget/gadget.button.h"
#include "_gadget/gadget.image.h"
#include "_gadget/gadget.checkbox.h"
#include "_gadget/gadget.scrollarea.h"
#include "_dialog/dialog.file.open.h"

class PROG_Mapper : public _progC
{
	private:
		
		_label*				description;
		_label*				fileLabel;
		_button*			okButton;
		_button*			cancelButton;
		_button*			browseButton;
		_checkbox*			saveDescision;
		_label*				saveDescisionLabel;
		_scrollArea*		scrollArea;
		bool				chooseButNotOpen;
		string				fileToOpen;
		_fileOpenDialog*	openDialog;
		
		void				main( _programArgs args );
		void				destruct();
		
		_callbackReturn		handler( _event event );
		void				dialogHandler( _dialogResult event );
		
	public:
		
		//! Ctor
		PROG_Mapper();
};

#endif