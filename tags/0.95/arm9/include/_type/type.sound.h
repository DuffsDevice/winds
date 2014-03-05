#ifndef _WIN_T_SOUND_
#define _WIN_T_SOUND_

#include "_type/type.h"
#include "_type/type.sound.definitions.h"
#include "_type/type.sound.msg.h"

class _sound
{
	private:
		
		//! Indicates the type of this sound
		_channelType	type;
		
		//! Holds the number of the channel used (-1 stands for 'no channel')
		_s8				channelId;
		
		//! Flag, that indicates whether the sound is currently running
		bool			playing;
		
		
		//! Send a command to the arm7 that is dependnent of this channel
		_u32 sendChannelCommand( _soundCommand cmd , _s16 value = 0 );
		
		//! Send a command to the arm7
		static _u32 sendCommand( _soundCommand cmd , _s16 value = 0 );
		
		//! Handler that handles data messages from the arm7
		static void soundDataHandler( _int bytes , void* userData );
		
		//! Array that holds pointers to all used channels
		static _sound* globalChannels[16];
		
	protected:
		
		//! Ctor
		_sound( _channelType type ) :
			type( type )
			, channelId( -1 )
		{}
		
		//! Setters
		void setFrequency( _u16 freq ){
			this->sendChannelCommand( _soundCommand::setFrequency , freq );
		}
		void setDutyCycle( _psgDutyCycle dC ){
			this->sendChannelCommand( _soundCommand::setDutyCycle , (_u8)dC );
		}
		
		//! Getters
		_u16 getFrequency(){
			return this->sendChannelCommand( _soundCommand::getFrequency );
		}
		_psgDutyCycle getDutyCycle(){
			return (_psgDutyCycle) this->sendChannelCommand( _soundCommand::getDutyCycle );
		}
		
		
		//! Function that will prepare a channel for use and returns the channel number
		virtual _s8 prepareChannel( _u8 volume , _s8 panning  ) = 0;
		virtual void cleanupChannel( _s8 channel ){};
		
	public:
		
		//! Play the sound
		void play( _u8 volume = 127 , _s8 panning = 0 );
		
		//! pause the execution of a sound
		void pause();
		
		//! Terminate the execution of a sound
		void stop();
		
		//! Check if the sound is playing
		bool isPlaying(){
			return this->playing;
		}
		
		//! Get the currently used volume & panning
		_u8 getVolume(){
			return this->sendChannelCommand( _soundCommand::getVolume );
		}
		_s8 getPanning(){
			return this->sendChannelCommand( _soundCommand::getPanning );
		}
		
		//! Set a new volume or panning value
		void setVolume( _u8 volume ){
			this->sendChannelCommand( _soundCommand::setVolume , volume );
		}
		void setPanning( _s8 panning ){
			this->sendChannelCommand( _soundCommand::setPanning , panning );
		}
		
		//! Get the Channel-id of this sound
		_s8 getChannel(){ return this->channelId; }
		
		//! Dtor
		~_sound(){
			this->stop();
		}
		
		
		//! Enable/Disable Sound Functionality
		static void enable();
		static void disable(){
			_sound::sendCommand( _soundCommand::soundEnable );
		}
		
		//! Mute/unmute Sound Playback on the DS
		static void mute();
		static void unMute();
};

#endif