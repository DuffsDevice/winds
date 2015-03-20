#include <_type/type.program.h>
#include <_type/type.program.lua.h>
#include <_type/type.program.c.h>
#include <_type/type.tokenizer.h>
#include <_controller/controller.execution.h>
#include <_controller/controller.filesystem.h>
#include <_controller/controller.registry.h>
#include <_controller/controller.timer.h>

#include <_resource/resource.program.credits.h>
#include <_resource/resource.program.explorer.h>
#include <_resource/resource.program.mapper.h>
#include <_resource/resource.program.taskmanager.h>
#include <_resource/resource.program.filedetail.h>
#include <_resource/resource.program.romexecutor.h>
#include <_resource/resource.program.console.h>
#include <_resource/resource.icon.lua.h>
#include <_resource/resource.icon.exe.h>
#include <program_example_lua.h>
#include <program_pong_lua.h>
#include <program_paint_lua.h>
#include <program_calculator_lua.h>

void _program::prepareExecution( _args args )
{
	this->executionData.startTime = _timerController::getMilliTime();
	this->executionData.argumentsForMain = move(args);
}

bool _program::isRunning() const {
	return _executionController::isExistent( this );
}

void _program::terminate(){
	this->executionData.markedForTermination = true;
}

_program::~_program()
{
	if( this->mainFrame )
	{
		// Save Dimensions of the mainframe to the registry
		_mainFrame& frame = *this->mainFrame;
		
		string hash = this->getRegistryHash();
		
		// Abort writing dimensions, because the program has no distinct header attributes
		if( hash.empty() )
			return;
		
		// Convert current mainFrame-dimensions to a string
		string dimensionString = unTokenize(
			(_initializerList<string>){
				int2string( frame.getX() )
				, int2string( frame.getY() )
				, int2string( frame.getWidth() )
				, int2string( frame.getHeight() )
			}
			, ", "
		);
		
		// Write the resulting string to the registry
		_registryController::getSystemRegistry().writeIndex( hash , "$mainFrame" , move(dimensionString) );
	}
}

_mainFrame* _program::getMainFrame( _length width , _length height , bool forceSize , _style&& style )
{
	if( !this->mainFrame )
	{
		string hash = this->getRegistryHash();
		string dimensionString;
		
		if( !hash.empty() )
			dimensionString = _registryController::getSystemRegistry().readIndex( hash , "$mainFrame" ).cpp_str();
		
		// Read Dimensions of the mainframe from the registry
		_vector<string> dim = tokenize( dimensionString , ", " , true );
		
		if( dim.size() != 4 ){
			// Allocate mainFrame instance using supplied dimensions and center of the screen
			this->mainFrame = new _mainFrame( ( SCREEN_WIDTH - width ) >> 1 , ( SCREEN_HEIGHT - height ) >> 1 , width , height , this , move(style) );
		}else if( forceSize ){
			// Allocate mainFrame instance using supplied dimensions and saved position
			this->mainFrame = new _mainFrame( string2int( dim[0] ) , string2int( dim[1] ) , width , height , this , move(style) );
		}else{
			// Allocate mainFrame instance using saved dimensions/position
			this->mainFrame = new _mainFrame( string2int( dim[0] ) , string2int( dim[1] ) , string2int( dim[2] ) , string2int( dim[3] ) , this , move(style) );
		}
	}
	return this->mainFrame;
}

_uniquePtr<_program> _program::fromFile( string filename )
{
	_direntry d = _direntry( filename );
	string fn = d.getFileName();
	
	// Convert to lowerspace
	transform( fn.begin() , fn.end() , fn.begin() , ::tolower );
	
	// Result Variable
	_program* result = nullptr;
	
	// For the case that the program exists
	if( d.isExisting() )
		result = new _luaProgram( d.readString() );
	else if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/explorer.exe" ) )
		result = new PROG_Explorer();
	else if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/rom_exec.exe" ) )
		result = new PROG_RomExecutor();
	else if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/console.exe" ) )
		result = new PROG_Console();
	else if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/progmapper.exe" ) )
		result = new PROG_Mapper();
	else if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/filedetail.exe" ) )
		result = new PROG_FileDetail();
	else if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/taskmgr.exe" ) )
		result = new PROG_TaskManager();
	else if( fn == _filesystemController::replaceAssocDirs( "%SYSTEM%/credits.exe" ) )
		result = new PROG_Credits();
	else if( fn == _filesystemController::replaceAssocDirs( "%WINDIR%/accessories/exampleprogram.exe" ) ){
		string str = (_literal)program_example_lua;
		str.resize( program_example_lua_size );
		result = new _luaProgram( move(str) );
	}
	else if( fn == _filesystemController::replaceAssocDirs( "%WINDIR%/accessories/paint.exe" ) ){
		string str = (_literal)program_paint_lua;
		str.resize( program_paint_lua_size );
		result = new _luaProgram( move(str) );
	}
	else if( fn == _filesystemController::replaceAssocDirs( "%WINDIR%/accessories/calculator.exe" ) ){
		string str = (_literal)program_calculator_lua;
		str.resize( program_calculator_lua_size );
		result = new _luaProgram( move(str) );
	}
	else if( fn == _filesystemController::replaceAssocDirs( "%WINDIR%/games/pong.exe" ) ){
		string str = (_literal)program_pong_lua;
		str.resize( program_pong_lua_size );
		result = new _luaProgram( move(str) );
	}
	
	if( result ){
		result->setPath(fn);
		result->setCWD( _direntry(fn).getParentDirectory() );
	}
	
	return move(result);
}


string _program::getRegistryHash()
{
	// Hashing Function: djb2
		_s32 initialSeed = 5381;
		_s32 hashVal = initialSeed;
		_s32 curChar;
		_literal str;
		
		// Process Name
		if( this->header.name ){
			str = this->header.name->c_str();
			while (curChar = *str++)	hashVal = (hashVal * 33) ^ curChar;
		}
		
		// Process Author
		if( this->header.author ){
			str = this->header.author->c_str();
			while (curChar = *str++)	hashVal = (hashVal * 33) ^ curChar;
		}
		
		// Process Version
		if( this->header.version ){
			str = this->header.version->c_str();
			while (curChar = *str++)	hashVal = (hashVal * 33) ^ curChar;
		}
		
		// Fallback on path
		if( hashVal == initialSeed )
		{
			str = this->path.c_str();
			while (curChar = *str++)	hashVal = (hashVal * 33) ^ curChar;
		}
	// Hash end
	
	if( hashVal < 0 ) // Do not allow negative hashes
		hashVal = -hashVal;
	
	return string("program{") + int2string( hashVal , 0 , 16 ) + "}";
}