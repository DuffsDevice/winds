// Check if already included
#ifndef _WIN_T_PROGRAM_
#define _WIN_T_PROGRAM_

#include "_type/type.gadget.h"
#include "_type/type.h"

enum class _programType
{
	progLua,
	progC,
	progFile
};

struct _programData{
	bool		autoDelete;
	_tempTime	runningSince;
};

typedef _vector<_pair<flex_ptr<_program>,_programData>> _programList;

class _program
{
	friend class _system;
	friend class _systemController;
	
	private:
		
		//! Type of the program
		_programType	type;
		
		//! path to the executeable
		string			path;
		
		//! Virtual main function to be overwritten in subclasses
		virtual void	internalMain( _cmdArgs args ) = 0;
		
		//! Called every 1/60s
		virtual	void	internalVbl(){}
		
		//! Main function to be called from _system
		void 			main( _gadget* w , _cmdArgs args );
		
		// Static List of running programs
		static _programList globalPrograms;
		static _programList globalProgramsToExecute;
		
		// Processes all programs
		static void runPrograms();
		
	protected:
		
		//! The current gadgetHost
		_gadget*		gadgetHost;
		
	public:
		
		//! Ctor
		_program( _programType type ) :
			type( type )
		{}
		
		//! Execute it! Means pushing it to _system's list of programs
		void 			execute( _cmdArgs args = _cmdArgs() );
		
		//! Terminate the program
		void 			terminate();
		
		//! get The gadgetHost
		_gadget*		getGadgetHost(){ return this->gadgetHost; }
		
		//! Virtual Dtor
		virtual 		~_program(){};
		
		//! After execution set path of file
		_program& setPath( string path ){
			this->path = path;
			return *this;
		}
		
		//! Get a list of all currently running programs
		static const _programList& getRunningPrograms(){
			return _program::globalPrograms;
		}
		
		//! Generate a _program instance from file
		static _program* fromFile( string filename );
};

#endif