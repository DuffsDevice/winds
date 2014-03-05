#include "_type/type.sound.stream.h"
#include "_type/type.callback.derives.h"
#include <nds/fifocommon.h>

void _soundStream::cleanupChannel( _s8 channel )
{
	if( this->buffer8 )
	{
		if( this->format == _channelFormat::bit8 )
			delete[] this->buffer8;
		else
			delete[] this->buffer16;
	}
}

_s8 _soundStream::prepareChannel( _u8 volume , _s8 panning )
{
	if( !this->buffer8 )
	{
		if( this->format == _channelFormat::bit8 )
			this->buffer8 = new _u8[this->length];
		else
			this->buffer16 = new _u16[this->length];
	}
	
	// Set up the message
	_channelSetMsg cSM = _channelSetMsg::streamChannel( this->filler , this->format , this->length , this->frequency , this->buffer8 , volume , panning );
	_soundDataMsg msg = _soundDataMsg::channelSetMsg( cSM );
	
	// Send message
	bool success = fifoSendDatamsg( FIFO_SOUND , sizeof(_channelSetMsg) , (_u8*)&msg );
	
	if( !success )
		return -1;
	
	// Wait for an answer
	while( !fifoCheckValue32(FIFO_SOUND) );
	
	return fifoGetValue32(FIFO_SOUND);
}

void _soundStream::setFrequency( _u16 freq )
{
	this->frequency = freq;
	_sound::setFrequency(freq);
}