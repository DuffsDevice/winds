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
#include <_controller/controller.execution.h>
#include <_controller/controller.registry.h>
#include <_controller/controller.sound.h>
#include <_controller/controller.timer.h>
#include <_controller/controller.video.h>

// libnds headers
extern "C"{
	#undef unused
	#undef deprecated
	#include <nds/bios.h>
	#include <nds/system.h>
	#include <nds/fifocommon.h>
	#include <nds.h>
}

void _winds::init()
{
	if( _winds::active )
		_winds::end();
	
	//! Set initial Seed for the pseudo random number generator
	srand( time(NULL) );
	
	_winds::initMethods = {
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
		, { &_executionController::init , "program" }
		, { &_interruptController::init , "interrupt" }
	};
	
	_winds::mainMethods = {
		&_guiController::frame
		, &_timerController::frame
		, &_eventController::frame
		, &_executionController::frame
	};
	
	_winds::interruptMethods = {
		&_animationController::frame
		, &_inputController::frame
	};
	
	// These get executed in reverse order
	_winds::endMethods = {
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
		, &_executionController::end
		, &_interruptController::end
	};
	
	// Run Init Methods
	for( const _pair<_controllerInit*,string>& method : _winds::initMethods )
	{
		if( !(*method.first)() ){
			_debugController::debug("The %s-Controller failed to init!",method.second.c_str());
			_debugController::submit();
			return;
		}
	}
	
	_winds::active = true;
	_winds::terminateMain = false;
	
	fifoSetDatamsgHandler( FIFO_USER_01 , _winds::fifoDataHandler , nullptr );
}

void _winds::stop(){
	_winds::terminateMain = true;
}

void _winds::fifoDataHandler( _s32 bytes , void* userData )
{
	_systemDataMsg msg;
	
	// Receive DataMsg
	fifoGetDatamsg( FIFO_USER_01 , bytes , (_u8*)&msg );
	
	if( msg.type == _systemDataMsgType::cpuUsage )
		_winds::cpuUsageTempSub = msg.cpuUsage;
	else if( msg.type == _systemDataMsgType::debug )
		_debugController::debug( msg.debugMessage );
}


void _winds::main()
{
	while( !_winds::terminateMain )
	{
		_tempTime milliTime = _timerController::getMilliTime();
		
		// Run Main Methods
		for( _controllerFrame* method : _winds::mainMethods )
			(*method)();
		
		milliTime = _timerController::getMilliTime() - milliTime;
		milliTime = milliTime * 100 * 60 / 1000; // Time relative to 1/60s
		if( milliTime > 100 )
			milliTime = 100;
		
		// Compute System-Usage
		_winds::cpuUsageTemp = _winds::cpuUsageTemp * 15 + milliTime;
		_winds::cpuUsageTemp >>= 4;
		
		// Wait for VBlank
		_interruptController::waitForVerticalBlank( false );
	}
}

void _winds::interrupt()
{
	// Run all registered VBL-Methods
	for( _controllerFrame* method : _winds::interruptMethods )
		(*method)();
}

void _winds::end()
{
	if( !_winds::active )
		return;
	
	// Run all end()-methods, beginning at the end of the list
	for( auto iter = _winds::endMethods.rbegin() ; iter != _winds::endMethods.rend() ; iter++ )
		(**iter)();
	
	_winds::active = false;
}

const wstring& _winds::getDSUserName()
{
	static wstring name;
	if( name.empty() && PersonalData->nameLen > 0 )
	{
		for( int i = 0 ; i < PersonalData->nameLen ; i++ )
			name += wstring( 1 , (_wchar) PersonalData->name[i] );
	}
	return name;
}

_language _winds::getDSLanguage(){
	return id2language[PersonalData->language];
}

_hardwareType _winds::getHardwareType()
{
	if( swiIsDebugger() )
		return _hardwareType::emulator;
	else if( REG_DSIMODE )
		return _hardwareType::dsi;
	return _hardwareType::ds;
}

_programHandle _winds::execute( const string& cmd )
{
	if( cmd.empty() )
		return nullptr;
	
	_pair<string,string> commandData = _args::splitCommand( cmd );
	
	return _direntry( commandData.first ).execute( commandData.second.c_str() );
}

//! Conversion from _hardwareType to string
_toStr<_hardwareType> hardwareType2str = {
	{ _hardwareType::ds			, "ds" },
	{ _hardwareType::dsi		, "dsi" },
	{ _hardwareType::emulator	, "emulator" },
};

_vector<_pair<_controllerInit*,string>>	_winds::initMethods;
_vector<_controllerFrame*>				_winds::mainMethods;
_vector<_controllerFrame*>				_winds::interruptMethods;
_vector<_controllerEnd*>				_winds::endMethods;
_int									_winds::cpuUsageTemp;
_int									_winds::cpuUsageTempSub;
bool									_winds::active = false;
bool									_winds::terminateMain = false;