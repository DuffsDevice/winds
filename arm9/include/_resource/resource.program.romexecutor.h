#ifndef _WIN_PROG_ROMEXECUTOR_
#define _WIN_PROG_ROMEXECUTOR_

#include <_type/type.program.c.h>

class PROG_RomExecutor : public _cProgram
{
	private:
		
		static string	fileToExecute;
		static void		shutdownCallback();
		
		void			main( _args args );
		
	public:
		
		PROG_RomExecutor();
};

#endif