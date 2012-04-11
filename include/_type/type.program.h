// Check if already included
#ifndef _WIN_T_PROGRAM_
#define _WIN_T_PROGRAM_

#include "_gadget/gadget.windows.h"

typedef enum{
	progLua,
	progC,
	progFile
} _programType;

class _program{
	
	private:
		
		_programType	type;
		
	protected:
		
		_windows*		windowsInstance;
		
	public:
		
		_program( _programType type ) : type( type ){}
		
		void run( _windows* w ){ this->windowsInstance = w; this->run(); }
		
		virtual void run() = 0;
		
};

#endif