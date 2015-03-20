#include <_type/type.sound.h>
#include <_type/type.sound.msg.h>
#include <_type/type.windows.h>

#include <nds/timers.h>
#include <nds/arm7/audio.h>
#include <nds/system.h>
#include <nds/interrupts.h>
#include <nds/arm7/audio.h>
#include <nds/ipc.h>
#include <nds/fifocommon.h>
#include <nds/fifomessages.h>

_soundChannel*	_sound::globalChannels[16] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
bool			_sound::soundMuted = false;
bool			_sound::soundEnabled = false;

void _sound::runSounds(){
	for( int i = 0 ; i < 16 ; i++ ){
		_soundChannel* chn = _sound::globalChannels[i];
		if( !chn )
			continue;
		chn->process();
	}
}

void _sound::enable(){
	powerOn(PM_SOUND_AMP);
	writePowerManagement(PM_CONTROL_REG, ( readPowerManagement(PM_CONTROL_REG) & ~PM_SOUND_MUTE ) | PM_SOUND_AMP );
	REG_SOUNDCNT = SOUND_ENABLE;
	_sound::unMute();
	_sound::soundEnabled = true;
}

void _sound::disable(){
	_sound::mute();
	REG_SOUNDCNT &= ~SOUND_ENABLE;
	writePowerManagement(PM_CONTROL_REG, ( readPowerManagement(PM_CONTROL_REG) & ~PM_SOUND_AMP ) | PM_SOUND_MUTE );
	powerOff(PM_SOUND_AMP);
	_sound::soundEnabled = false;
}

void _sound::mute(){
	REG_MASTER_VOLUME = 0;
	_sound::soundMuted = true;
}

void _sound::unMute(){
	REG_MASTER_VOLUME = 127;
	_sound::soundMuted = false;
}

void _sound::soundDataHandler( _s32 bytes , void* userData )
{
	_soundDataMsg msg;
	
	// Receive DataMsg
	fifoGetDatamsg( FIFO_SOUND , bytes , (_u8*)&msg );
	
	if( msg.type == _soundDataMsgType::channelSet )
	{
		// Allocates and writes itself into global
		_s8 channel = _soundChannel::fromMsg( msg.channelSet );
		
		fifoSendValue32( FIFO_SOUND , (_u32)channel );
	}
}

void _sound::soundMsgHandler( _u32 command , void* userData )
{
	_soundCommandMsg msg = _soundCommandMsg( command );
	_soundChannel* chn = _sound::globalChannels[msg.channel];
	
	//char text[30] = "Command Received: ";
	//text[18] = '0'+command;
	//text[19] = ',';
	//text[20] = '0'+msg.channel;
	//text[21] = 0;
	//_winds::debug( text );
	
	switch( msg.cmd )
	{
		case _soundCommand::soundEnable:
			_sound::enable();
			break;
		case _soundCommand::soundDisable:
			_sound::disable();
			break;
		case _soundCommand::isSoundEnabled:
			fifoSendValue32( FIFO_SOUND , _sound::isEnabled() );
			return;
		// ----------------
		case _soundCommand::setVolume:
			if( chn )
				chn->setVolume( msg.value );
			break;
		case _soundCommand::getVolume:
			if( chn )
				fifoSendValue32( FIFO_SOUND , chn->getVolume() );
			else
				fifoSendValue32( FIFO_SOUND , -1 );
			return;
		// ----------------
		case _soundCommand::setPanning:
			if( chn )
				chn->setPanning( msg.value );
			break;
		case _soundCommand::getPanning:
			if( chn )
				fifoSendValue32( FIFO_SOUND , chn->getPanning() );
			else
				fifoSendValue32( FIFO_SOUND , -1 );
			return;
		// ----------------
		case _soundCommand::setFrequency:
			if( chn )
				chn->setFrequency( msg.value );
			break;
		case _soundCommand::getFrequency:
			if( chn )
				fifoSendValue32( FIFO_SOUND , chn->getFrequency() );
			else
				fifoSendValue32( FIFO_SOUND , -1 );
			return;
		// ----------------
		case _soundCommand::setDutyCycle:
			if( chn )
				chn->setDutyCycle( (_psgDutyCycle)msg.value );
			break;
		case _soundCommand::getDutyCycle:
			if( chn )
				fifoSendValue32( FIFO_SOUND , (_s32)chn->getDutyCycle() );
			else
				fifoSendValue32( FIFO_SOUND , -1 );
			return;
		// ----------------
		case _soundCommand::channelKill:
			if( chn )
				delete chn;
			break;
		case _soundCommand::isChannelEnabled:
			fifoSendValue32( FIFO_SOUND , chn != nullptr );
			return;
		case _soundCommand::channelStop:
			if( chn )
				chn->stop();
			break;
		case _soundCommand::channelPlay:
			if( chn )
				chn->play();
			break;
		case _soundCommand::isChannelPlaying:
			fifoSendValue32( FIFO_SOUND , chn->isPlaying() );
			return;
		// ----------------
		case _soundCommand::micStop:
			micStopRecording();
			break;
		// ----------------
		case _soundCommand::soundMute:
			_sound::mute();
			break;
		case _soundCommand::soundUnMute:
			_sound::unMute();
			break;
		case _soundCommand::isSoundMuted:
			fifoSendValue32( FIFO_SOUND , _sound::isMuted() );
			return;
	}
	
	fifoSendValue32( FIFO_SOUND , msg.channel );
}

void _sound::installFIFO(){
	fifoSetDatamsgHandler( FIFO_SOUND , _sound::soundDataHandler , 0 );
	fifoSetValue32Handler( FIFO_SOUND , (FifoValue32HandlerFunc) &_sound::soundMsgHandler , 0 );
}