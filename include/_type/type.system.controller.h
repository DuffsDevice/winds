#ifndef _WIN_T_SYSTEMSCENARIO_
#define _WIN_T_SYSTEMSCENARIO_

class _systemController
{
	private:
		
		enum class _systemState : _u8
		{
			empty,
			login,
			bootup,
			setup,
			desktop
		};
		
		static _systemState state;
		static bool			dirty;
		
		//! The current state - getter and setter
		static void changeState( _systemState state ){ dirty = _systemController::state != state; _systemController::state = state; }
		static _systemState getState(){ return state; }
		
		//! Different Scenarios
		static void loginPage();
		static void setupPage();
		static void bootupPage();
		static void desktopPage();
		
		//! Internal event-handlers
		static _callbackReturn loginHandler( _event e );
		static _callbackReturn setupHandler( _event e );
		static _callbackReturn bootupHandler( _event e );
		static _callbackReturn desktopHandler( _event e );
		
	public:
	
		static void main();
};

#endif