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
	friend class _system;
	
	private:
		
		//! Type of the program
		_programType	type;
		
	protected:
		
		//! The current gadgetHost
		_gadget*		gadgetHost;
		
		//! Virtual main function to be overwritten in subclasses
		virtual void	main( _cmdArgs&& args ) = 0;
		
		//! Main function to be called from _system
		void 			main( _gadget* w , _cmdArgs&& args  );
		void 			main( _gadget* w , const _cmdArgs& args  ){ this->main( w , _cmdArgs( args ) ); }
		
	public:
		
		//! Flag: whether the program shall be deleted in the next frame
		bool 			autoDelete;
		
		//! Ctor
		_program( _programType type ) :
			type( type ),
			autoDelete( false )
		{}
		
		//! Execute it! Means pushing it to _system's list of programs
		void 			execute( _cmdArgs&& args = _cmdArgs() );
		void 			execute( const _cmdArgs& args ){ this->execute( _cmdArgs( args ) ); }
		
		//! Terminate the program
		void 			terminate();
		
		//! get The gadgetHost
		_gadget*		getGadgetHost(){ return this->gadgetHost; }
		
		//! Virtual Dtor
		virtual 		~_program(){};
		
		
		static _program* fromFile( string filename );
};

#endif