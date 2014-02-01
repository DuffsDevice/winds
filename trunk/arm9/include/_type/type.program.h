// Check if already included
#ifndef _WIN_T_PROGRAM_
#define _WIN_T_PROGRAM_

#include "_type/type.gadget.h"
#include "_type/type.h"
#include "_type/type.language.h"

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

struct _programHeader{
	flex_ptr<_bitmap>	fileIcon;
	flex_ptr<string>	author;
	flex_ptr<string>	version;
	flex_ptr<string>	description;
	flex_ptr<string>	copyright;
	_language			language;
};

typedef _vector<_pair<_program*,_programData>> _programList;

class _program
{
	friend class _system;
	friend class _systemController;
	
	private:
		
		//! Type of the program
		_programType	type;
		
		//! Parameters of the program
		_programHeader	header;
		
		//! path to the executeable
		string			path;
		
		// Static List of running programs
		static _programList globalPrograms;
		static _programList globalProgramsToExecute;
		static _constBitmap	standardFileImage;
		
		// Processes all programs
		static void		runPrograms();
		static void		terminateAllPrograms();
		
		//! Virtual main function to be overwritten in subclasses
		virtual void	internalMain( _cmdArgs args ) = 0;
		
		//! Called every 1/60s
		virtual	void	internalVbl(){}
		
		//! Main function to be called from _system
		void			main( _gadget* w , _cmdArgs args );
		
	protected:
		
		//! The current gadgetHost
		_gadget*		gadgetHost;
		
		//! Set Header
		void			setHeader( _programHeader header ){
			this->header = header;
		}
		
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
		
		//! Get the image of the program
		const _bitmap&	getFileImage(){ return *this->header.fileIcon; }
		
		//! Get Header Information of the program
		_programHeader&	getHeader(){ return this->header; }
		
		//! Virtual Dtor
		virtual 		~_program(){};
		
		//! Set Path to the executeable that program instance is currently running in
		_program& setPath( string path ){
			this->path = path;
			return *this;
		}
		
		//! Get path of the executeable
		const string& getPath() const { return this->path; }
		string& getPath(){ return this->path; }
		
		//! Get a list of all currently running programs
		static const _programList& getRunningPrograms(){
			return _program::globalPrograms;
		}
		
		//! Generate a _program instance from file
		static _program* fromFile( string filename );
};

#endif