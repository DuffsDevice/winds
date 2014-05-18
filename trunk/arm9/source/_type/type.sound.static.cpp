#include "_type/type.sound.static.h"
#include <nds/fifocommon.h>
#include <stdio.h>

_s8 _staticSound::prepareChannel( _u8 volume , _s8 panning )
{
	// Set up the message
	_channelSetMsg cSM = _channelSetMsg::staticChannel( this->format , this->length , this->frequency , this->data , this->repeat , volume , panning );
	
	// Let the rest do the sound class
	return _sound::sendChannelPrepareMessage( cSM );
}

void _staticSound::setFrequency( _u16 freq )
{
	this->frequency = freq;
	_sound::setFrequency(freq);
}