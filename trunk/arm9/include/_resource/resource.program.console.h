#ifndef _WIN_PROG_CONSOLE_
#define _WIN_PROG_CONSOLE_

#include <_type/type.program.c.h>
#include <_gadget/gadget.codepointmatrix.h>

class PROG_Console : public _cProgram
{
	private:
		
		_codepointMatrix* codepointMatrix;
		
		void			main( _args args );
		
		_callbackReturn handler( _event event );
		
	public:
		
		PROG_Console();
};

#endif