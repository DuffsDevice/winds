#include <_type/type.sound.stream.file.h>
#include <nds/fifocommon.h>

void _fileSoundStream::cleanupChannel( _u8 )
{
	if( this->buffer8 ){
		if( this->format == _channelFormat::bit8 )
			delete[] this->buffer8;
		else
			delete[] this->buffer16;
	}
}

_s8 _fileSoundStream::prepareChannel( _u8 volume , _s8 panning )
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

void _fileSoundStream::setFrequency( _u16 freq )
{
	this->frequency = freq;
	_sound::setFrequency(freq);
}

_u32 _fileSoundStreamFiller::executor( _u32 requestedSize , _s8* destination , _u16 frequency , _channelFormat format ) const
{
	_u64 numBytes;
	
	if( format == _channelFormat::bit16 )
		requestedSize <<= 1;
	
	if( !this->reference.source.read( destination , requestedSize , ignore , &numBytes ) )
		this->reference.stop();
	
	return requestedSize;
}