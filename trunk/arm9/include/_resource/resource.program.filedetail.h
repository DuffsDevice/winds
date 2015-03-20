#ifndef _WIN_PROG_FILEDETAIL_
#define _WIN_PROG_FILEDETAIL_

#include <_type/type.program.c.h>
#include <_gadget/gadget.button.h>
#include <_gadget/gadget.image.h>
#include <_gadget/gadget.label.h>
#include <_gadget/gadget.checkbox.h>
#include <_gadget/gadget.textbox.h>
#include <_gadget/gadget.divider.h>

class PROG_FileDetail : public _cProgram
{
	private:
		
		_uniquePtr<_direntry>		file;
		
		_uniquePtr<_imageGadget>	fileIcon;
		_uniquePtr<_textBox>		fileNameTextbox;
		_uniquePtr<_button>			okButton;
		_uniquePtr<_button>			cancelButton;
		_uniquePtr<_button>			openedWithChangeButton;
		
		_uniquePtr<_checkBox>		systemCheckbox;
		_uniquePtr<_checkBox>		hiddenCheckbox;
		_uniquePtr<_checkBox>		readonlyCheckbox;
		
		_uniquePtr<_label>			fileTypeLabel;
		_uniquePtr<_label>			fileTypeValueLabel;
		_uniquePtr<_label>			openedWithLabel;
		_uniquePtr<_label>			openedWithValueLabel;
		_uniquePtr<_label>			locationLabel;
		_uniquePtr<_label>			locationValueLabel;
		_uniquePtr<_label>			sizeLabel;
		_uniquePtr<_label>			sizeValueLabel;
		_uniquePtr<_label>			dateCreatedLabel;
		_uniquePtr<_label>			dateCreatedValueLabel;
		_uniquePtr<_label>			dateLastChangeLabel;
		_uniquePtr<_label>			dateLastChangeValueLabel;
		_uniquePtr<_label>			dateLastAccessLabel;
		_uniquePtr<_label>			dateLastAccessValueLabel;
		_uniquePtr<_label>			attributesLabel;
		_uniquePtr<_label>			systemAttrLabel;
		_uniquePtr<_label>			hiddenAttrLabel;
		_uniquePtr<_label>			readonlyAttrLabel;
		
		_uniquePtr<_dividerGadget>	divider1;
		_uniquePtr<_dividerGadget>	divider2;
		_uniquePtr<_dividerGadget>	divider3;
		
		
		void			main( _args args );
		
		_callbackReturn handler( _event event );
		
	public:
		
		PROG_FileDetail();
};

#endif