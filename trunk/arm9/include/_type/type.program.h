// Check if already included
#ifndef _WIN_T_PROGRAM_
#define _WIN_T_PROGRAM_

#include <_type/type.h>
#include <_type/type.gadget.h>
#include <_type/type.language.h>
#include <_type/type.arguments.h>
#include <_type/type.program.executiondata.h>
#include <_type/type.uniqueptr.h>
#include <_type/type.service.h>
#include <_gadget/gadget.window.mainframe.h>

// Describes the type of a program
enum class _programType{
	lua,
	c,
};

// Struct about a program's details
struct _programHeader
{
	_uniquePtr<_bitmap>	fileIcon;
	_uniquePtr<string>	fileName;
	_uniquePtr<_bitmap>	windowIcon;
	_uniquePtr<string>	windowName;
	_uniquePtr<string>	name;
	_uniquePtr<string>	author;
	_uniquePtr<string>	version;
	_uniquePtr<string>	description;
	_uniquePtr<string>	copyright;
	_language			language;
	
	//! Ctor
	_programHeader() : language( "--" ) {}
	
	//! Move/Copy Ctor
	_programHeader( _programHeader&& ) = default;
	_programHeader( const _programHeader& ) = delete;
	
	//! Move/Copy Assignment operator
	_programHeader& operator=( _programHeader&& ) = default;
	_programHeader& operator=( const _programHeader& ) = delete;
};

class _program
{
	friend class _executionController;
	
	private:
		
		//! Type of the program
		_programType			type;
		
		//! Parameters of the program
		_programHeader			header;
		_programExecutionData	executionData;
		
		//! Path to the executeable
		string					path;
		
		//! _mainFrame-Object
		_uniquePtr<_mainFrame>	mainFrame;
		
		//! This method creates a hash that serves as a key in the registry
		string					getRegistryHash();
		
		//! Main function to be called from _executionController
		virtual void			main( _args args ){};
		virtual void			dispatch( _serviceId id , _serviceTransfer args ){};
		virtual void			process( _serviceId id , _serviceState& state ){};
		virtual _serviceTransfer getServiceData( _serviceId id ){ return _serviceTransfer(); };
		
		/**
		 * Prepares the program for execution (called by _executionController)
		 */
		void					prepareExecution( _args args );
		
		//! Main function to be called from _executionController
		void					callMain(){
			if( this->executionData.mainCalled )
				return;
			this->main( move(this->executionData.argumentsForMain) );
			this->executionData.mainCalled = true;
		}
		
		//! Calls the 'dispatch' function to process a service request
		void					callDispatch( _serviceId id , _serviceTransfer arguments ){
			this->dispatch( id , move(arguments) );
		}
		
		//! Calls the 'process' function to process a service (every vbl)
		void					callProcess( _serviceId id , _serviceState& state ){
			this->process( id , state );
		}
		
		//! Called every 1/60s
		virtual	void			frame( _int numRunningPrograms ){}
		
	protected:
		
		//! Set Header
		void					setHeader( _programHeader header ){ this->header = move(header); }
		
	public:
		
		//! Ctor
		_program( _programType type ) :
			type( type )
			, mainFrame( nullptr )
		{}
		
		//! Dtor (made virtual to work for subclasses)
		virtual ~_program();
		
		
		//! Returns, whether this program instance is currently running
		bool					isRunning() const ;
		
		
		//! Terminates the program (only called by the _executionController)
		void 					terminate();
		
		
		//! Get the image of the program
		const _bitmap&			getFileImage(){ return *this->header.fileIcon; }
		
		
		//! Get a newly allocated _mainFrame-Gadget
		_mainFrame*				getMainFrame( _length width , _length height , bool forceSize = false , _style&& style = _style() );
		_mainFrame*				getMainFrame( _length width , _length height , _style&& style ){
			return getMainFrame( width , height , false , move(style) );
		}
		
		//! Assume, the mainFrame is already created
		_mainFrame*				getMainFrame() const { return this->mainFrame; }
		
		
		//! Get header information of the program
		_programHeader&			getHeader(){ return this->header; }
		const _programHeader&	getHeader() const { return this->header; }
		
		
		//! Get Details about the execution of a program
		const _programExecutionData&	getExecutionData() const { return this->executionData; }
		
		
		//! Set the File-Path at which the executeable was started
		_program&				setPath( string path ){
			this->path = path;
			return *this;
		}
		
		
		//! Get path of the executeable
		const string&			getPath() const { return this->path; }
		string&					getPath(){ return this->path; }
		
		//! Function to clean up the program (pendant to main)
		virtual	void			cleanUp(){};
		
		//! Generate a _program instance from file
		static _uniquePtr<_program>	fromFile( string filename );
};

#endif