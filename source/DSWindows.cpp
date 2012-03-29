#include "DSWindows.h"

namespace DSWindows
{	
	void memcpyHalfWords( _u16* src , _u16* dest , _u32 size ){
		while(size--)
			*src++ = *dest++;
	}
	
	void submit(){
		while( !(keysDown() & KEY_A) )
			scanKeys();
	}

	_pixel desktopBg;
	_pixel desktopBgX;
	_pixel desktopBgD;
	
	void initDisplay()
	{
		
		//! Power on everything 
		powerOn( POWER_ALL );
		
		//! Set the video mode on the main screen.
		videoSetMode( MODE_5_2D );
		
		//lcdMainOnTop();
		lcdMainOnBottom();
		
		//! Set the video mode on the sub screen.
		videoSetModeSub( MODE_5_2D );
		
		//! Init Video RAMs
		vramSetBankA( VRAM_A_MAIN_BG );
		vramSetBankC( VRAM_C_SUB_BG );
		
		//! Init Backgrounds
		bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
		bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
		
		consoleDemoInit();
		
	}
	
	void benchMarkStart(){
		cpuStartTiming(true);
	}
	
	void benchMarkStopPrint(){
		printf("Ticks:%d",cpuGetTiming() );
		cpuEndTiming();
		submit();
	}
	
	unsigned int i = 0;
	
}