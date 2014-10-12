#include <_type/type.sound.h>
#include <_controller/controller.sound.h>

// Sound Controls
void _sound::play( _u8 volume , _s8 panning ){ _soundController::playSound( this ); }
void _sound::stop(){ _soundController::killSound( this ); }
void _sound::pause(){ _soundController::pauseSound( this ); }

// Prepares a channel using the _channelSetMsg and returns the new channel (returns -1 in case of fail)
_s8 _sound::sendChannelPrepareMessage( _channelSetMsg msg )
{
	// Send message
	bool success = _soundController::sendDataMessage( _soundDataMsg::channelSetMsg(msg) );
	
	if( !success )
		return -1;
	
	return _soundController::expectNumber(); // Wait for the channel id to be send from the arm7
}

// Get Channel ID
_s8				_sound::getChannel() const { return _soundController::getChannelOf( this ); }

// Getters
_u16			_sound::getFrequency() const { return _soundController::sendCommand( _soundCommand::getFrequency , this ); }
_psgDutyCycle	_sound::getDutyCycle() const { return (_psgDutyCycle) _soundController::sendCommand( _soundCommand::getDutyCycle , this ); }
_s8				_sound::getPanning() const { return _soundController::sendCommand( _soundCommand::getPanning , this ); }
_u8				_sound::getVolume() const { return _soundController::sendCommand( _soundCommand::getVolume , this ); }

// Setters
void			_sound::setFrequency( _u16 freq ){			_soundController::sendCommand( _soundCommand::setFrequency , this , freq ); }
void			_sound::setDutyCycle( _psgDutyCycle dC ){	_soundController::sendCommand( _soundCommand::setDutyCycle , this , (_u8)dC ); }
void			_sound::setPanning( _s8 panning ){			_soundController::sendCommand( _soundCommand::setPanning , this , panning ); }
void			_sound::setVolume( _u8 volume ){			_soundController::sendCommand( _soundCommand::setVolume , this , volume ); }
