#ifndef _WIN_T_SOUND_
#define _WIN_T_SOUND_

#include <_type/type.predefines.h>
#include <_type/type.sound.definitions.h>
#include <_type/type.sound.channel.h>

class _sound
{
	friend class _soundChannel;
	
	private:
		
		//! Attribute the indicates a previous call to enable()
		static bool soundEnabled;
		static bool soundMuted;
		
		//! Handler for FIFO-Messages
		static void soundDataHandler( _s32 bytes , void* userData );
		static void soundMsgHandler( _u32 command , void* userData );
		
		static _soundChannel* globalChannels[16];
	
	public:
	
		//! Method to process all currently running sounds
		static void runSounds();
		
		//! Enables Sound-Playback on the DS
		static void enable();
		
		//! Disable Sound on the DS
		static void disable();
		
		//! Check if sounds are enabled on the DS
		static bool isEnabled(){
			return _sound::soundEnabled;
		}
		
		//! Mute Sound on the DS
		static void mute();
		static void unMute();
		
		//! Check if Speakers are muted
		static bool isMuted(){
			return _sound::soundMuted;
		}
		
		//! Installs FIFO handlers
		static void installFIFO();
		
		//! Get an unused channel
		static _s8 getFreeChannel(){
			for( _u8 id = 0 ; id < 16 ; id++ )
				if( !_sound::globalChannels[id] )
					return id;
			return -1;
		}
		
		//! Get an unused noise channel
		static _s8 getFreeNoiseChannel(){
			for( _u8 id = 14 ; id < 16 ; id++ )
				if( !_sound::globalChannels[id] )
					return id;
			return -1;
		}
		
		//! Get an unused PSG channel
		static _s8 getFreePSGChannel(){
			for( _u8 id = 8 ; id < 14 ; id++ )
				if( !_sound::globalChannels[id] )
					return id;
			return -1;
		}
		
		//! Send an integer to arm9
		static void sendInteger( int val );
};

#endif