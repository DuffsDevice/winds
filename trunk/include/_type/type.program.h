// Check if already included
#ifndef _WIN_T_PROGRAM_
#define _WIN_T_PROGRAM_

#include "_type/type.gadget.h"

enum class _programType
{
	progLua,
	progC,
	progFile
};

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
		
		void 			terminate();
		
		void 			init( _gadget* w , _cmdArgs& args  );
		
		virtual int		main( _cmdArgs& args ) = 0;
		
		virtual void	init( _cmdArgs& args ) = 0;
		
		virtual 		~_program(){};
		
};

#endif