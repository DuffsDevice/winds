#include "_type/type.sound.oscillator.h"
#include <nds/fifocommon.h>

_s8 _soundOscillator::prepareChannel( _u8 volume , _s8 panning )
{
	// Set up the message
	_channelSetMsg cSM = _channelSetMsg::psgChannel( this->frequency , this->dutyCycle , volume , panning );
	_soundDataMsg msg = _soundDataMsg::channelSetMsg( cSM );
	
	// Send message
	bool success = fifoSendDatamsg( FIFO_SOUND , sizeof(_channelSetMsg) , (_u8*)&msg );
	
	if( !success )
		return -1;
	
	// Wait for an answer
	while( !fifoCheckValue32(FIFO_SOUND) );
	
	return fifoGetValue32(FIFO_SOUND);
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