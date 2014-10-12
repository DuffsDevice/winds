#ifndef _WIN_C_VIDEO_
#define _WIN_C_VIDEO_

#include <_type/type.h>
#include <_type/type.controller.h>

class _videoController : public _controller
{
	private:
		
		static _int bgIdKeyboard;
		static _int bgIdHost; // Id for the gadget-host
		static _int bgIdTop; // Id for top screen
		
	public:
		
		//! Routine to initialize all video hardware for appropriate use
		static bool init();
		
		//! Get the Background ID of the top Screen Layer
		static _int getBgIdTop(){ return _videoController::bgIdTop; }
		
		//! Get the Background ID of the Keyboard Screen Layer
		static _int getBgIdKeyboard(){ return _videoController::bgIdKeyboard; }
		
		//! Get the Background ID of the Gadget-Host Screen Layer
		static _int getBgIdHost(){ return _videoController::bgIdHost; }
		
		//! Screen-Fade In/Out
		static void fadeScreens( bool out , bool anim = true );
};

#endif