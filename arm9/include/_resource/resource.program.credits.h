#ifndef _WIN_PROG_CREDITS_
#define _WIN_PROG_CREDITS_

#include <_type/type.program.c.h>
#include <_gadget/gadget.button.h>
#include <_gadget/gadget.image.h>

class PROG_Credits : public _cProgram
{
	private:
		
		_uniquePtr<_imageGadget>	winLogo;
		_uniquePtr<_button>			okButton;
		
		_uniquePtr<_label>			heading1;
		_uniquePtr<_label>			value1_1;
		_uniquePtr<_label>			value1_2;
		_uniquePtr<_label>			value1_3;
		_uniquePtr<_label>			value1_4;
		
		_uniquePtr<_label>			heading2;
		_uniquePtr<_label>			value2_1;
		_uniquePtr<_label>			value2_2;
		_uniquePtr<_label>			value2_3;
		_uniquePtr<_label>			value2_4;
		
		_uniquePtr<_label>			heading3;
		_uniquePtr<_label>			value3_1;
		
		void			main( _args args );
		
		_callbackReturn handler( _event );
		
	public:
		
		PROG_Credits();
};

#endif