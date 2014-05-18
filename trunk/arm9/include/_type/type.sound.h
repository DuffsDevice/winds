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
		
		//! Flag, that indicates whether the sound is currently running
		bool			playing;
		
		//! Function that will prepare a channel for use and returns the channel number (overwritten by subclasses)
		virtual _s8		prepareChannel( _u8 volume , _s8 panning ) = 0;
		virtual void	cleanupChannel( _u8 channel ){};
		
		friend class _soundController;
		
	protected:
		
		//! Ctor
		_sound( _channelType type ) :
			type( type )
		{}
		
		//!	Allows subclasses to prepare thair channel
		_s8				sendChannelPrepareMessage( _channelSetMsg );
		
		//! Setters
		void			setFrequency( _u16 freq );
		void			setDutyCycle( _psgDutyCycle dC );
		
		//! Getters
		_u16			getFrequency() const ;
		_psgDutyCycle	getDutyCycle() const ;
		
		friend class _soundManager;
		
	public:
		
		//! Play the sound
		void play( _u8 volume = 127 , _s8 panning = 0 );
		
		//! pause the execution of a sound
		void pause();
		
		//! Terminate the execution of a sound
		void stop();
		
		//! Check if the sound is playing
		bool isPlaying() const {
			return this->playing;
		}
		
		//! Returns the channel id used by the sound
		_s8 getChannel() const ;
		
		//! Get the currently used volume & panning
		_u8 getVolume() const ;
		_s8 getPanning() const ;
		
		//! Set a new volume or panning value
		void setVolume( _u8 volume );
		void setPanning( _s8 panning );
		
		//! Dtor
		~_sound(){ this->stop(); }
};

#endif