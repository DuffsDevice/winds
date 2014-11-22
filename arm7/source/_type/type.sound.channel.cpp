#include <_type/type.windows.h>
#include <_type/type.sound.h>
#include <_type/type.sound.channel.h>
#include <_type/type.sound.msg.h>
#include <_type/type.sound.definitions.h>

#include <nds/timers.h>
#include <nds/system.h>
#include <nds/interrupts.h>
#include <nds/arm7/audio.h>
#include <nds/ipc.h>
#include <nds/fifocommon.h>
#include <nds/fifomessages.h>

template<typename T>
constexpr T min( T v1 , T v2 ){
	return v1 < v2 ? v1 : v2;
}

_s8 _soundChannel::fromMsg( _channelSetMsg msg )
{
	_s8 newChannel = -1;
	switch( msg.type )
	{
		case _channelType::psg:
			newChannel = _sound::getFreePSGChannel();
			break;
		case _channelType::noise:
			newChannel = _sound::getFreeNoiseChannel();
			break;
		default:
			newChannel = _sound::getFreeChannel();
	}
	
	if( newChannel < 0 )
		return -1;
	
	_soundChannel* chn = new _soundChannel();
	
	// Set Attributes
	chn->channelId = newChannel;
	chn->bufferFilled = 0;
	chn->wasExecuted = false;
	chn->bufferSize = msg.bufferSize;
	chn->frequency = msg.frequency;
	chn->format = msg.format;
	chn->filler = msg.filler;
	chn->type = msg.type;
	chn->volume = msg.volume;
	chn->panning = msg.panning + 63;
	chn->buffer8 = (_s8*)msg.buffer;
	chn->curBuffer8 = chn->buffer8;
	chn->dutyCycle = msg.dutyCycle;
	
	// Push it into the list
	_sound::globalChannels[newChannel] = chn;
	
	switch( chn->type )
	{
		case _channelType::psg:
			SCHANNEL_CR(newChannel) = SOUND_VOL(chn->volume) | SOUND_PAN(chn->panning) | (3 << 29) | (_u32(chn->dutyCycle) << 24);
			break;
		
		case _channelType::noise:
			SCHANNEL_CR(newChannel) = SOUND_VOL(chn->volume) | SOUND_PAN(chn->panning) | (3 << 29);
			break;
		
		case _channelType::oneshotstatic:
		case _channelType::repeatstatic:
			
			// Set source and Length
			SCHANNEL_SOURCE(newChannel) = (_u32)chn->buffer8;
			if( chn->format == _channelFormat::bit16 )
				SCHANNEL_LENGTH(newChannel) = chn->bufferSize >> 1;
			else
				SCHANNEL_LENGTH(newChannel) = chn->bufferSize >> 2;
			
			// Loop point will always be 0
			SCHANNEL_REPEAT_POINT(newChannel) = 0;
			
			// Set Control Register
			SCHANNEL_CR(newChannel) =
				SOUND_VOL(chn->volume)
				| SOUND_PAN(chn->panning)
				| (_u32(chn->format) << 29)
				| (chn->type == _channelType::repeatstatic ? SOUND_REPEAT : SOUND_ONE_SHOT);
			
			break;
		case _channelType::stream:
			chn->lastCheck = _windows::getBUSTime();
			
			SCHANNEL_SOURCE(newChannel) = (_u32)chn->buffer8;
			SCHANNEL_REPEAT_POINT(newChannel) = 0;
			
			if( chn->format == _channelFormat::bit16 ){
				chn->endBuffer16 = chn->buffer16 + chn->bufferSize;
				SCHANNEL_LENGTH(newChannel) = chn->bufferSize >> 1;
			}
			else{
				chn->endBuffer8 = chn->buffer8 + chn->bufferSize;
				SCHANNEL_LENGTH(newChannel) = chn->bufferSize >> 2;
			}
			SCHANNEL_CR(newChannel) = SOUND_VOL(chn->volume) | SOUND_PAN(chn->panning) | (_u32(chn->format) << 29) | SOUND_REPEAT;
			break;
	}
	
	// Set Frequency
	SCHANNEL_TIMER(newChannel) = SOUND_FREQ( chn->frequency );
	
	return newChannel;
}

_soundChannel::~_soundChannel(){
	this->stop();
	_sound::globalChannels[this->channelId] = nullptr;
}

void _soundChannel::stop()
{
	if( !this->isPlaying() )
		return;
	
	if( this->type == _channelType::stream )
	{
		_tempTime time = _windows::getBUSTime();
		
		_s32 elapsedTicks = time - this->lastCheck; // Ticks of Timer0 that have elapsed since the last call to process()
		_s32 elapsedSamples = elapsedTicks * this->frequency / _windows::ticksPerSecond;
		
		// Indicate that we have check a few samples
		this->lastCheck = elapsedSamples * _windows::ticksPerSecond / this->frequency - this->lastCheck;
	}
	
	this->wasExecuted = false;
	SCHANNEL_CR(this->channelId) &= ~SCHANNEL_ENABLE;
}

void _soundChannel::play(){
	if( this->isPlaying() )
		return;
	
	if( this->type == _channelType::stream )
		this->lastCheck = _windows::getBUSTime() - this->lastCheck;
	
	this->wasExecuted = true;
	SCHANNEL_CR(this->channelId) |= SCHANNEL_ENABLE;
}

optimized void _soundChannel::process()
{
	if( this->type == _channelType::stream )
	{
		_tempTime time = _windows::getBUSTime();
		
		if( time < this->lastCheck ){
			this->lastCheck = time;
			return;
		}
		
		_u32 elapsedTime = time - this->lastCheck; // Ticks of Timer that have elapsed since the last call to process()
		_u32 elapsedSamples = elapsedTime * this->frequency / _windows::ticksPerSecond;
		
		// Indicate that we have checked a few samples
		this->lastCheck += elapsedSamples * _windows::ticksPerSecond / this->frequency;
		
		// disadvance the still Filled buffer with the samples that have elapsed
		this->bufferFilled -= elapsedSamples;
		this->samplesPlayed += elapsedSamples;
		
		// Check if the end of the buffer would be reached in about 1/4 second
		const _s32 criticalLowRun = min<_u32>( this->frequency , this->bufferSize ) >> 1;
		
		while( this->bufferFilled <= criticalLowRun )
		{
			_u32 samplesBeforeEnd;
			
			if( this->format == _channelFormat::bit8 )
				samplesBeforeEnd = min( this->curBuffer8 + criticalLowRun , this->endBuffer8 ) - this->curBuffer8;
			else
				samplesBeforeEnd = min( this->curBuffer16 + criticalLowRun , this->endBuffer16 ) - this->curBuffer16;
			
			// Call Filler...
			this->callFiller( samplesBeforeEnd , this->curBuffer8 , this->frequency );
			
			_s32 filledSamples = samplesBeforeEnd;
			
			// and advance the number of valid stream bytes
			this->bufferFilled += filledSamples;
			
			if( this->format == _channelFormat::bit8 )
			{
				if( ( this->curBuffer8 += filledSamples ) >= this->endBuffer8 )
					this->curBuffer8 = this->buffer8; // We reached the end of the buffer -> start new!
			}
			else
			{
				// Increase two bytes if sound format is 16bits
				if( ( this->curBuffer16 += filledSamples ) >= this->endBuffer16 )
					this->curBuffer16 = this->buffer16; // We reached the end of the buffer -> start new!
			}
		}
	}
	else if( this->type == _channelType::oneshotstatic && this->hasFinished() )
	{
		// Tell the arm9!
		_soundDataMsg msg = _soundDataMsg::channelStoppedMsg( { this->channelId } );
		fifoSendDatamsg( FIFO_SOUND , sizeof(_soundDataMsg) , (_u8*)&msg );
		
		delete this;
	}
}

_u32 _soundChannel::callFiller( _u32 requestedSize , _s8* destination , _u16 frequency )
{
	_fillingParams fP;
	
	fP.filler = this->filler;
	fP.requestedSize = requestedSize;
	fP.destination = destination;
	fP.frequency = frequency;
	fP.format = this->format;
	
	_soundDataMsg msg = _soundDataMsg::bufferFillMsg( fP );
	fifoSendDatamsg( FIFO_SOUND , sizeof(_soundDataMsg) , (_u8*)&msg );
	
	return requestedSize;
}

void _soundChannel::setVolume( _u8 volume ){
	SCHANNEL_CR(this->channelId) &= ~0xFF;
	SCHANNEL_CR(this->channelId) |= volume;
	this->volume = volume;
}

void _soundChannel::setPanning( _s8 panning ){
	SCHANNEL_CR(this->channelId) &= ~SOUND_PAN(0xFF);
	SCHANNEL_CR(this->channelId) |= SOUND_PAN(panning);
	this->panning = panning;
}

void _soundChannel::setFrequency( _u16 freq ){
	SCHANNEL_TIMER(this->channelId) = SOUND_FREQ(freq);
	this->frequency = freq;
}

bool _soundChannel::isPlaying(){
	return wasExecuted;
}

bool _soundChannel::hasFinished(){
	if( this->wasExecuted && !(SCHANNEL_CR(this->channelId) & SCHANNEL_ENABLE) )
	{
		this->wasExecuted = false;
		return true;
	}
	return false;
}

void _soundChannel::setDutyCycle( _psgDutyCycle dC ){
	if( this->type != _channelType::psg )
		return;
	SCHANNEL_CR(this->channelId) &=	 ~(7 << 24);
	SCHANNEL_CR(this->channelId) |=	_u32(dC) << 24;
	this->dutyCycle = dC;
}