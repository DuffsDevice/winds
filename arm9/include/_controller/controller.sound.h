#ifndef _WIN_T_SOUND_HARDWARE_
#define _WIN_T_SOUND_HARDWARE_

#include <_type/type.h>
#include <_type/type.sound.h>
#include <_type/type.controller.h>

class _soundController : public _controller
{
	private:
		
		//! Array that holds pointers to all channels
		static _sound* channels[16];
		
		
		//! Send a command to the arm7 that is dependnent of this channel
		static _u32 sendCommand( _soundCommand cmd , _u8 channelId );
		static _u32 sendCommand( _soundCommand cmd , const _sound* subject , _s16 value );
		static _u32 sendCommand( _soundCommand cmd , const _sound* subject = nullptr ){
			return _soundController::sendCommand( cmd , subject , 0 );
		}
		
		//! Send a Data-Message to the arm7 and returns true, if that succeeded
		static bool sendDataMessage( _soundDataMsg msg );
		
		//! Waits until the arm9 receives a number from the arm7
		static _s32 expectNumber();
		
		//! Handler that handles data messages from the arm7
		static void soundDataHandler( _int bytes , void* userData );
		
		
		// API for the _sound class
		static void playSound( _sound* snd , _u8 volume = 127 , _s8 panning = 0 );
		static void killSound( _sound* snd );
		static void pauseSound( _sound* snd );
		
		friend class _sound;
		
	public:
		
		//! Initialization routine
		static bool init();
		
		//! Shuts down the sound controller
		static void end();
		
		//! Enable/Disable Sound Functionality
		static void enable();
		static void disable(){
			_soundController::sendCommand( _soundCommand::soundEnable );
		}
		
		//! get the channel of the supplied sound object (returns -1, if it hasn't one)
		static _s8 getChannelOf( const _sound* snd ){
			for( _u8 i = 0 ; i < 16 ; i++ )
				if( channels[i] == snd )
					return i;
			return -1;
		}
		
		//! Mute/unmute Sound Playback on the DS
		static void mute();
		static void unMute();
};

#endif