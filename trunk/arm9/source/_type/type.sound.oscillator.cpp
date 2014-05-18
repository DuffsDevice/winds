#include "_type/type.sound.oscillator.h"
#include <nds/fifocommon.h>

_s8 _soundOscillator::prepareChannel( _u8 volume , _s8 panning )
{
	// Set up the message
	_channelSetMsg cSM = _channelSetMsg::psgChannel( this->frequency , this->dutyCycle , volume , panning );
	
	// Let the rest do the sound class
	return _sound::sendChannelPrepareMessage( cSM );
}

void _soundOscillator::setFrequency( _u16 freq )
{
	this->frequency = freq;
	_sound::setFrequency(freq);
}

void _soundOscillator::setDutyCycle( _psgDutyCycle dC )
{
	this->dutyCycle = dC;
	_sound::setDutyCycle(dC);
}