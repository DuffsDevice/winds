#ifndef _WIN_T_SOUND_OSCILLATOR_
#define _WIN_T_SOUND_OSCILLATOR_

#include <_type/type.h>
#include <_type/type.sound.h>
#include <_type/type.sound.note.h>


class _soundOscillator : public _sound
{
	private:
		
		_u16			frequency;
		_psgDutyCycle	dutyCycle;
		
		//! Prepares the channel for use and returns its number
		_s8 prepareChannel( _u8 volume , _s8 panning ) override;
		
	public:
		
		//! Ctors
		_soundOscillator( _u16 frequency , _psgDutyCycle dC = _psgDutyCycle::dc37 , _u8 volume = 127 , _s8 panning = 0 ) : 
			_sound( _channelType::psg )
			, frequency( frequency * 8 )
			, dutyCycle( dC )
		{}
		_soundOscillator( _note note , _psgDutyCycle dC = _psgDutyCycle::dc37 , _u8 volume = 127 , _s8 panning = 0 ) : 
			_sound( _channelType::psg )
			, frequency( (_u16)note )
			, dutyCycle( dC )
		{}
		
		//! Make available from base class
		void setFrequency( _u16 freq );
		using _sound::getFrequency;
		void setDutyCycle( _psgDutyCycle dC );
		using _sound::getDutyCycle;
};

#endif