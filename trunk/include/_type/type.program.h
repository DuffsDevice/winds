// Check if already included
#ifndef _WIN_T_PROGRAM_
#define _WIN_T_PROGRAM_

#include "_gadget/gadget.windows.h"
#include <memory>
using namespace std;

typedef enum{
	progLua,
	progC,
	progFile
} _programType;

class _program : public enable_shared_from_this<_program>{
	
	private:
		
		_programType	type;
		
	protected:
		
		_windows*		windowsInstance;
		
	public:
		
		bool 			autoDelete; // whether the program shall be deleted on finish
		
		_program( _programType type ) : type( type ) , autoDelete( true ) {}
		
		void 			execute( _cmdArgs args = _cmdArgs() );
		
		void 			init( _windows* w , _cmdArgs args  );
		
		virtual int		main( _cmdArgs args ) = 0;
		
		virtual void	init( _cmdArgs args ) = 0;
		
};

#endif