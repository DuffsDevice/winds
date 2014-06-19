#include "_controller/controller.video.h"
#include "_controller/controller.timer.h"
#include "_type/type.color.h"

extern "C"{
	#undef unused
	#include <nds/arm9/video.h>
	#include <nds/arm9/background.h>
	#include <nds/arm9/console.h>
	#include <nds/system.h>
}


bool _videoController::init()
{
	//! Set the video mode on the main screen.
	videoSetMode( MODE_5_2D );
	
	//! Put the Main Screen to the bottom (the Touchscreen)
	lcdMainOnBottom();
	
	//! Set the video mode on the sub screen.
	videoSetModeSub( MODE_5_2D );
	
	//! Init Video RAMs
	vramSetBankA( VRAM_A_MAIN_BG );
	vramSetBankB( VRAM_B_MAIN_BG );
	vramSetBankC( VRAM_C_SUB_BG);
	vramSetBankD( VRAM_D_MAIN_BG );
	
	//! Init Backgrounds
	_videoController::bgIdKeyboard	= bgInit(2, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	_videoController::bgIdHost		= bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 8, 0);
	_videoController::bgIdTop		= bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	
	setBackdropColor( _color::black );
	setBackdropColorSub( _color::black );
	consoleDemoInit();
	
	return true;
}

void _videoController::fadeScreens( bool out , bool anim )
{
	_tempTime time = _timerController::getMilliTime();
	
	// Main Screen
	REG_BLDCNT = ( 1 << 3 ) | ( 1 << 2 ) // 3rd and 2nd Screen Layer
	| ( 3 << 6 ) ; // Set Blend Mode to fade to black ( 2 << 6 ) would be fading to white
	
	// Sub Screen
	REG_BLDCNT_SUB = ( 1 << 3 ) // 3rd Screen Layer
	| ( 3 << 6 ) ; // Set Blend Mode to fade to black ( 2 << 6 ) would be fading to white
	
	if( out )
	{
		while( anim && time + 200 > _timerController::getMilliTime() ){
			int curValue = 31 - float(_timerController::getMilliTime() - time )/(200)*31;
			REG_BLDY = curValue;
			REG_BLDCNT_SUB = curValue;
		}
		
		REG_BLDY = 0;
		REG_BLDY_SUB = 0;
	}
	else
	{
		while( anim && time + 200 > _timerController::getMilliTime() ){
			int curValue = float(_timerController::getMilliTime() - time )/(200)*31;
			REG_BLDY = curValue;
			REG_BLDCNT_SUB = curValue;
		}
		
		REG_BLDY = 31;
		REG_BLDY_SUB = 31;
	}
}

_int _videoController::bgIdKeyboard;
_int _videoController::bgIdHost;
_int _videoController::bgIdTop;