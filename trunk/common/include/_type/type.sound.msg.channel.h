#ifndef _WIN_T_SOUND_CHANNEL_MSG_
#define _WIN_T_SOUND_CHANNEL_MSG_

#include <_type/type.predefines.h>
#include <_type/type.sound.definitions.h>
#include <_type/type.callback.h>

class _channelSetMsg
{
	friend class _soundChannel;
	friend class _sound;
	friend class _staticSound;
	
	private:
	
		//! Attribute that holds information for the arm9,
		//! which filling routin should be called
		_callback<_fillingRoutine>* filler;
		
		_u32				bufferSize;
		void*				buffer;
		_u16				frequency;
		_channelType		type;
		_u8					volume;
		_s8					panning;
		_channelFormat		format;
		_psgDutyCycle		dutyCycle;
		
		_u32				dummy;
		
		//! Private Ctor
		_channelSetMsg( _callback<_fillingRoutine>* filler , _channelType type , _u32 bufferSize , _u16 freq , _psgDutyCycle dC , _u8 volume = 127 , _s8 panning = 0 ) :
			filler( filler )	, bufferSize( bufferSize )
			, frequency( freq )	, type( type )
			, volume( volume )	, panning( panning )
			, dutyCycle( dC )
		{}
		
		_channelSetMsg( _callback<_fillingRoutine>* filler , _channelFormat format , _channelType type , _u32 bufferSize , _u16 freq , void* buffer , _u8 volume = 127 , _s8 panning = 0 ) :
			filler( filler )	, bufferSize( bufferSize )
			, buffer( buffer )	, frequency( freq )
			, type( type )		, volume( volume )
			, panning( panning ), format( format )
		{}
	
	public:
		
		//! Default Ctor
		_channelSetMsg(){}
		
		//! Named Ctors
		static _channelSetMsg streamChannel( _callback<_fillingRoutine>* filler
			, _channelFormat format , _u32 bufferSize , _u16 freq , void* buffer , _u8 volume = 127 , _s8 panning = 0 )
		{
			return _channelSetMsg( filler , format , _channelType::stream , bufferSize , freq , buffer , volume , panning );
		}
		
		static _channelSetMsg staticChannel( _channelFormat format
			, _u32 bufferSize , _u16 freq , const void* buffer , bool loop = false , _u8 volume = 127 , _s8 panning = 0 )
		{
			return _channelSetMsg( nullptr , format , loop ? _channelType::repeatstatic : _channelType::oneshotstatic , bufferSize , freq , const_cast<void*>(buffer) , volume , panning );
		}
		
		static _channelSetMsg psgChannel( _u16 freq , _psgDutyCycle dC , _u8 volume = 127 , _s8 panning = 0 ){
			return _channelSetMsg( nullptr , _channelType::psg , 0 , freq , dC , volume , panning );
		}
		
		static _channelSetMsg noiseChannel( _u16 freq , _u8 volume = 127 , _s8 panning = 0 ){
			return _channelSetMsg( nullptr , _channelType::noise , 0 , freq , _psgDutyCycle::ignore , volume , panning );
		}
};

struct _channelStoppedMsg
{
	_s8 channelId;
};

#endif