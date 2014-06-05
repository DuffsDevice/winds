#include "_controller/controller.sound.h"
#include "_controller/controller.debug.h"

extern "C"{
	#include <nds/fifocommon.h>
}

bool _soundController::init(){
	fifoSetDatamsgHandler( FIFO_SOUND , _soundController::soundDataHandler , nullptr );
	_soundController::enable();
	return true;
}

void _soundController::end(){
	for( _sound* snd : channels )
		_soundController::killSound( snd );
}

_u32 _soundController::sendCommand( _soundCommand cmd , _u8 channelId )
{
	// Generate the message
	_soundCommandMsg msg = _soundCommandMsg( cmd , channelId );
	
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

_u32 _soundController::sendCommand( _soundCommand cmd , const _sound* subject , _s16 value )
{
	_s8 channelId = subject ? _soundController::getChannelOf( subject ) : -1;
	
	// Abort if the supplied sound doesn't have a channel connected to it
	if( subject && channelId < 0 )
		return 0;
	
	// Generate the message
	_soundCommandMsg msg = _soundCommandMsg( cmd , channelId , value );
	
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

_s32 _soundController::expectNumber(){
	while( !fifoCheckValue32(FIFO_SOUND) ); // Wait for that number
	return fifoGetValue32(FIFO_SOUND); // Return it
}


bool _soundController::sendDataMessage( _soundDataMsg msg ){
	return fifoSendDatamsg( FIFO_SOUND , sizeof(_soundDataMsg) , (_u8*)&msg );
}


void _soundController::enable(){
	_soundController::sendCommand( _soundCommand::soundEnable );
}


void _soundController::playSound( _sound* snd , _u8 volume , _s8 panning )
{
	if( !snd || snd->playing )
		return;
	
	_s8 channelId = _soundController::getChannelOf( snd );
	
	// Check if the sound needs a new channel
	if( channelId < 0 )
	{
		channelId = snd->prepareChannel( volume , panning );
		
		// Make sure we now have a channel
		if( channelId < 0 )
			return;
	}
	
	// Start Playing
	_soundController::sendCommand( _soundCommand::channelPlay , channelId );
	
	// Organisatory stuff..
	snd->playing = true;
	_soundController::channels[channelId] = snd;
}


void _soundController::killSound( _sound* snd )
{
	if( !snd )
		return;
	
	_s8 channelId = _soundController::getChannelOf( snd );
	
	if( channelId < 0 )
		return;
	
	// Destroy the channel through arm7
	_soundController::sendCommand( _soundCommand::channelKill );
	
	// Indicate that this sound isn't running
	snd->playing = false;
	
	// Derived classes can do something here...
	snd->cleanupChannel( channelId );
	
	// Indicate that we don't use any channel
	_soundController::channels[channelId] = nullptr;
}

void _soundController::soundDataHandler( _int bytes , void* userData )
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
		
		_sound*& snd = _soundController::channels[channel];
		if( snd ){
			snd->playing = false;
			snd = nullptr;
		}
	}
	else if( msg.type == _soundDataMsgType::debug )
	{
		static int i = 0;
		_debugController::debug("Message: %d -    %d\n",msg.debugInteger,i++);
	}
}


void _soundController::pauseSound( _sound* snd )
{
	if( !snd->playing )
		return;
	
	_soundController::sendCommand( _soundCommand::channelStop , _soundController::getChannelOf( snd ) );
	snd->playing = false;
}

_sound*	_soundController::channels[16] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };