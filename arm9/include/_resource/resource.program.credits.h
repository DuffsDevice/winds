#ifndef _WIN_PROG_CREDITS_
#define _WIN_PROG_CREDITS_

#include "_type/type.system.h"
#include "_type/type.program.c.h"
#include "_gadget/gadget.button.h"
#include "_gadget/gadget.image.h"
#include "_gadget/gadget.window.h"

class PROG_Credits : public _progC
{
	private:
		
		_window*		window;
		_imageGadget*	winLogo;
		_button*		okButton;
		
		_label*			heading1;
		_label*			value1_1;
		_label*			value1_2;
		_label*			value1_3;
		_label*			value1_4;
		
		_label*			heading2;
		_label*			value2_1;
		_label*			value2_2;
		_label*			value2_3;
		_label*			value2_4;
		
		_label*			heading3;
		_label*			value3_1;
		
		void			main( _programArgs args );
		void			destruct();
		
		_callbackReturn handler( _event event );
		
	public:
		
		PROG_Credits();
};

#endif