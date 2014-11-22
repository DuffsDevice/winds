#include <_resource/resource.program.romexecutor.h>
#include <_resource/resource.icon.executor.h>
#include <_controller/controller.power.h>
#include <_controller/controller.debug.h>
#include <_controller/controller.gui.h>
#include <_library/library.rom.h>

PROG_RomExecutor::PROG_RomExecutor()
{
	_programHeader header;
	header.fileIcon = BMP_ExecutorIcon();
	header.name = "ROM_exec";
	header.fileName = "ROM_exec";
	header.author = string("WinDS");
	header.description = string("Executes a .nds file");
	this->setHeader( move(header) );
}

void PROG_RomExecutor::main( _programArgs args )
{
	if( !args.size() ){
		_debugController::debug( "Please pass a valid .nds" );
		this->terminate();
		return;
	}
	_direntry rom = _direntry(args[0]);
	if( !rom.isExisting() ){
		_debugController::debug( "Please pass an existing .nds" );
		this->terminate();
		return;
	}
	
	// Set File to execute
	PROG_RomExecutor::fileToExecute = rom.getFileName();
	_powerController::addShutdownCallback( make_callback( &PROG_RomExecutor::shutdownCallback ) );
	
	// Shut down WinDS
	_guiController::changeState( _guiState::shutdown );
}

void PROG_RomExecutor::shutdownCallback(){
	if( PROG_RomExecutor::fileToExecute.empty() )
		return;
	runNdsFile( PROG_RomExecutor::fileToExecute.c_str() , 0 , 0 );
}

string PROG_RomExecutor::fileToExecute;