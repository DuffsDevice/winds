#include "_type/type.sound.static.h"
#include <nds/fifocommon.h>
#include <stdio.h>

_s8 _staticSound::prepareChannel( _u8 volume , _s8 panning )
{
	// Set up the message
	_channelSetMsg cSM = _channelSetMsg::staticChannel( this->format , this->length , this->frequency , this->data , this->repeat , volume , panning );
	_soundDataMsg msg = _soundDataMsg::channelSetMsg( cSM );
	
	// Send message
	bool success = fifoSendDatamsg( FIFO_SOUND , sizeof(_channelSetMsg) , (_u8*)&msg );
	
	if( !success )
		return -1;
	
	// Wait for an answer
	while( !fifoCheckValue32(FIFO_SOUND) );
	
	return fifoGetValue32(FIFO_SOUND);
}

void _staticSound::setFrequency( _u16 freq )
{
	this->frequency = freq;
	_sound::setFrequency(freq);
}