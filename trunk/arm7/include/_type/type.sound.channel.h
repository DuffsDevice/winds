#ifndef _WIN_T_SOUND_CHANNEL_
#define _WIN_T_SOUND_CHANNEL_

#include "_type/type.predefines.h"
#include "_type/type.sound.definitions.h"
#include "_type/type.sound.msg.channel.h"
#include "_type/type.callback.h"

// Note: 
// ~~~~~
// As the PSG-Squarewave is composed of 8 Samples,
// the actual frequency of the played tone is 1/8 of the sample rate

class _soundChannel
{
	friend class _sound;
	
	private:
	
		//! Attribute that holds information for the arm9,
		//! which filling routin should be called
		_callback<_fillingRoutine>* filler;
		
		_s32			bufferFilled;
		_u32			bufferSize;
		_u32			samplesPlayed;
		_tempTime		lastCheck;
		_u16			frequency;
		_s8				channelId;
		_channelFormat	format;
		_channelType	type;
		_psgDutyCycle	dutyCycle;
		_u8				volume;
		_s8				panning;
		bool			wasExecuted;
		union{
			_s8*	buffer8;
			_s16*	buffer16;
		};
		union{
			_s8*	curBuffer8;
			_s16*	curBuffer16;
		};
		union{
			_s8*	endBuffer8;
			_s16*	endBuffer16;
		};
	
		//! Calls the filling method
		_u32 callFiller( _u32 requestedSize , _s8* dest , _u16 frequency );
		
		//! Will be called on each channel if it is not 'nullptr'
		void process();
		
		//! Private Ctor
		_soundChannel(){}
	
	public:
		
		//! Dtor
		~_soundChannel();
		
		//! Stops Playback of this channel
		void stop();
		
		//! Starts Playback of this channel
		void play();
		
		//! Check if a channel is currently playing
		bool isPlaying();
		
		//! Check if a yet executed channel has finished playing
		bool hasFinished();
		
		//! Setters
		void setFrequency( _u16 );
		void setPanning( _s8 );
		void setVolume( _u8 );
		void setDutyCycle( _psgDutyCycle );
		
		//! Getters
		_u16 getFrequency(){ return this->frequency; }
		_s8 getPanning(){ return this->panning; }
		_u8 getVolume(){ return this->volume; }
		_psgDutyCycle getDutyCycle(){ return this->dutyCycle; }
		
		//! Creates a new channel by copying the properties in the message
		//! Returns -1 on fail and a number between 0 and 15 indicating the new channel used
		static _s8 fromMsg( _channelSetMsg msg );
};

#endif