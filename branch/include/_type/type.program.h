// Check if already included
#ifndef _WIN_T_PROGRAM_
#define _WIN_T_PROGRAM_

#include "_type/type.gadget.h"
using namespace std;

typedef enum
{
	progLua,
	progC,
	progFile
} _programType;

class _program
{
	
	private:
		
		_programType	type;
		
	protected:
		
		_gadget*		gadgetHost;
		
	public:
		
		bool 			autoDelete; // whether the program shall be deleted on finish
		
		_program( _programType type ) : type( type ) , autoDelete( true ) {}
		
		void 			execute( _cmdArgs args = _cmdArgs() );
		
		void 			init( _gadget* w , _cmdArgs& args  );
		
		virtual int		main( _cmdArgs& args ) = 0;
		
		virtual void	init( _cmdArgs& args ) = 0;
		
};

#endif