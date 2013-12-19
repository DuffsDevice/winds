#include "_type/type.sound.h"
#include <stdio.h>
#include <nds/fifocommon.h>

_sound*	_sound::globalChannels[16] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };

void _sound::play( _u8 volume , _s8 panning )
{
	if( this->playing )
		return;
	
	if( this->channelId < 0 )
		this->channelId = this->prepareChannel( volume , panning );
	
	// Check if we now have a channel
	if( this->channelId < 0 )
		return;
	
	// Start Playing
	this->sendChannelCommand( _soundCommand::channelPlay );
	
	// Organisatory stuff..
	this->playing = true;
	_sound::globalChannels[this->channelId] = this;
}

void _sound::stop()
{
	if( this->channelId < 0 )
		return;
	
	// Destroy the channel through arm7
	this->sendChannelCommand( _soundCommand::channelKill );
	
	// Indicate that this sound isn't running
	this->playing = false;
	
	// Derived classes can do something here...
	this->cleanupChannel( this->channelId );
	
	// Indicate that we don't use any channel
	_sound::globalChannels[this->channelId] = nullptr;
	this->channelId = -1;
}

void _sound::pause()
{
	if( !this->playing )
		return;
	
	this->sendChannelCommand( _soundCommand::channelStop );
	this->playing = false;
}

_u32 _sound::sendChannelCommand( _soundCommand cmd , _s16 value )
{
	// Abort if we don't own a channel
	if( this->channelId < 0 )
		return 0;
	
	// Generate the message
	_soundCommandMsg msg = _soundCommandMsg( cmd , this->channelId , value );
	
	// Send the command
	bool success = fifoSendValue32( FIFO_SOUND , msg.sum );
	
	if( !success )
		return 0;
	
	// Wait for the answer to be transfered
	while( !fifoCheckValue32( FIFO_SOUND ) );
	
	// Lossless conversion from _u32 to _s32 (big numbers get turned negative)
	_soundCommandResult result;
	result.sum = fifoGetValue32( FIFO_SOUND );
	
	return result.value;
}

_u32 _sound::sendCommand( _soundCommand cmd , _s16 value )
{
	// Generate the message
	_soundCommandMsg msg = _soundCommandMsg( cmd , -1 , value );
	
	// Send the command
	bool success = fifoSendValue32( FIFO_SOUND , msg.sum );
	
	if( !success )
		return 0;
	
	// Wait for the answer to be transfered
	while( !fifoCheckValue32( FIFO_SOUND ) );
	
	// Lossless conversion from _u32 to _s32 (big numbers get turned negative)
	_soundCommandResult result;
	result.sum = fifoGetValue32( FIFO_SOUND );
	
	return result.value;
}

void _sound::soundDataHandler( _int bytes , void* userData )
{
	_soundDataMsg msg;
	
	// Receive DataMsg
	fifoGetDatamsg( FIFO_SOUND , bytes , (_u8*)&msg );

	if( msg.type == _soundDataMsgType::bufferFill )
	{
		_callback<_fillingRoutine>* ptr = msg.bufferFill.filler;
		
		if( ptr )
		{
			_callback<_fillingRoutine>& filler = *ptr;
			
			// Call the callback
			filler( msg.bufferFill.requestedSize , msg.bufferFill.destination , msg.bufferFill.frequency , msg.bufferFill.format );
		}
	}
	else if( msg.type == _soundDataMsgType::channelStopped )
	{
		_s8 channel = msg.channelStopped.channelId;
		
		if( channel < 0 || channel > 15 )
			return;
		
		_sound*& snd = _sound::globalChannels[channel];
		if( snd )
		{
			snd->channelId = -1;
			snd->playing = false;
			snd = nullptr;
		}
	}
	else if( msg.type == _soundDataMsgType::integer )
	{
		static int i = 0;
		printf("Message: %d -    %d\n",msg.integer,i++);
	}
}

void _sound::enable(){
	fifoSetDatamsgHandler( FIFO_SOUND , _sound::soundDataHandler , nullptr );
	_sound::sendCommand( _soundCommand::soundEnable );
}
	