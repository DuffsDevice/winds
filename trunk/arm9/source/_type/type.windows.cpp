#include <_type/type.windows.h>
#include <_type/type.system.msg.h>
#include <_type/type.language.h>
#include <_controller/controller.animation.h>
#include <_controller/controller.debug.h>
#include <_controller/controller.event.h>
#include <_controller/controller.filesystem.h>
#include <_controller/controller.font.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.input.h>
#include <_controller/controller.interrupt.h>
#include <_controller/controller.localization.h>
#include <_controller/controller.memory.h>
#include <_controller/controller.network.h>
#include <_controller/controller.power.h>
#include <_controller/controller.program.h>
#include <_controller/controller.registry.h>
#include <_controller/controller.sound.h>
#include <_controller/controller.timer.h>
#include <_controller/controller.video.h>

// libnds headers
extern "C"{
	#include <nds/bios.h>
	#include <nds/system.h>
	#include <nds/fifocommon.h>
}

void _windows::init()
{
	if( _windows::active )
		_windows::end();
	
	//! Set initial Seed for the pseudo random number generator
	srand( time(NULL) );
	
	_windows::initMethods = {
		{ &_powerController::init , "power" }
		, { &_memoryController::init , "memory" }
		, { &_filesystemController::init , "filesystem" }
		, { &_networkController::init , "network" }
		, { &_registryController::init , "registry" }
		, { &_videoController::init , "video" }
		, { &_soundController::init , "sound" }
		, { &_inputController::init , "input" }
		, { &_timerController::init , "timer" }
		, { &_animationController::init , "animation" }
		, { &_localizationController::init , "localization" }
		, { &_fontController::init , "font" }
		, { &_eventController::init , "event" }
		, { &_guiController::init , "gui" }
		, { &_programController::init , "program" }
		, { &_interruptController::init , "interrupt" }
	};
	
	_windows::mainMethods = {
		&_guiController::frame
		, &_timerController::frame
		, &_eventController::frame
		, &_programController::frame
	};
	
	_windows::interruptMethods = {
		&_animationController::frame
		, &_inputController::frame
	};
	
	// These get executed in reverse order
	_windows::endMethods = {
		&_powerController::end
		, &_memoryController::end
		, &_filesystemController::end
		, &_networkController::end
		, &_registryController::end
		, &_videoController::end
		, &_soundController::end
		, &_inputController::end
		, &_timerController::end
		, &_animationController::end
		, &_localizationController::end
		, &_fontController::end
		, &_eventController::end
		, &_guiController::end
		, &_programController::end
		, &_interruptController::end
	};
	
	// Run Init Methods
	for( const _pair<_controllerInit*,string>& method : _windows::initMethods )
	{
		if( !(*method.first)() ){
			_debugController::debug("The %s-Controller failed to init!",method.second.c_str());
			_debugController::submit();
			return;
		}
	}
	
	_windows::active = true;
	_windows::terminateMain = false;
	
	fifoSetDatamsgHandler( FIFO_USER_01 , _windows::fifoDataHandler , nullptr );
}

void _windows::stop(){
	_windows::terminateMain = true;
}

void _windows::fifoDataHandler( _s32 bytes , void* userData )
{
	_systemDataMsg msg;
	
	// Receive DataMsg
	fifoGetDatamsg( FIFO_USER_01 , bytes , (_u8*)&msg );
	
	if( msg.type == _systemDataMsgType::cpuUsage )
		_windows::cpuUsageTempSub = msg.cpuUsage;
}


void _windows::main()
{
	while( !_windows::terminateMain )
	{
		_tempTime milliTime = _timerController::getMilliTime();
		
		// Run Main Methods
		for( _controllerFrame* method : _windows::mainMethods )
			(*method)();
		
		milliTime = _timerController::getMilliTime() - milliTime;
		milliTime = milliTime * 100 * 60 / 1000; // Time relative to 1/60s
		if( milliTime > 100 )
			milliTime = 100;
		
		// Compute System-Usage
		_windows::cpuUsageTemp = _windows::cpuUsageTemp * 15 + milliTime;
		_windows::cpuUsageTemp >>= 4;
		
		// Wait for VBlank
		_interruptController::waitForVerticalBlank( false );
	}
}

void _windows::interrupt()
{
	// Run all registered VBL-Methods
	for( _controllerFrame* method : _windows::interruptMethods )
		(*method)();
}

void _windows::end()
{
	if( !_windows::active )
		return;
	
	// Run all end()-methods, beginning at the end of the list
	for( auto iter = _windows::endMethods.rbegin() ; iter != _windows::endMethods.rend() ; iter++ )
		(**iter)();
	
	_windows::active = true;
}

const string& _windows::getDSUserName()
{
	static string name;
	if( name.empty() && PersonalData->nameLen > 0 )
	{
		for( int i = 0 ; i < PersonalData->nameLen ; i++ )
			name += PersonalData->name[i];
	}
	return name;
}

_language _windows::getDSLanguage(){
	return id2language[PersonalData->language];
}

_hardwareType _windows::getHardwareType()
{
	if( swiIsDebugger() )
		return _hardwareType::emulator;
	else if( REG_DSIMODE )
		return _hardwareType::dsi;
	return _hardwareType::ds;
}

bool _windows::executeCommand( const string& cmd )
{
	if( cmd.empty() )
		return false;
	
	_pair<string,string> commandData = _programArgs::splitCommand( cmd );
	
	return _direntry( commandData.first ).execute( commandData.second.c_str() );
}

//! Conversion from _hardwareType to string
_toStr<_hardwareType> hardwareType2str = {
	{ _hardwareType::ds			, "ds" },
	{ _hardwareType::dsi		, "dsi" },
	{ _hardwareType::emulator	, "emulator" },
};

_vector<_pair<_controllerInit*,string>>	_windows::initMethods;
_vector<_controllerFrame*>				_windows::mainMethods;
_vector<_controllerFrame*>				_windows::interruptMethods;
_vector<_controllerEnd*>				_windows::endMethods;
_int									_windows::cpuUsageTemp;
_int									_windows::cpuUsageTempSub;
bool									_windows::active = false;
bool									_windows::terminateMain = false;