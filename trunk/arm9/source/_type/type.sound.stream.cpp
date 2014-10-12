#include <_type/type.sound.stream.h>
#include <_type/type.callback.derives.h>
#include <nds/fifocommon.h>

void _soundStream::cleanupChannel( _u8 )
{
	if( this->buffer8 ){
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
	
	// Let the rest do the sound class
	return _sound::sendChannelPrepareMessage( cSM );
}

void _soundStream::setFrequency( _u16 freq )
{
	this->frequency = freq;
	_sound::setFrequency(freq);
}