#ifndef _WIN_T_SOUND_STATIC_
#define _WIN_T_SOUND_STATIC_

#include <_type/type.h>
#include <_type/type.sound.h>

class _staticSound : public _sound
{
	private:
		
		_u16 frequency;
		_channelFormat format;
		const void* data;
		_u32 length;
		bool repeat;
		
		//! Prepares the channel for use and returns its number
		_s8 prepareChannel( _u8 volume , _s8 panning ) override;
		
	public:
		
		//! Ctor
		_staticSound( _channelFormat format , _u16 frequency , const void* data , _u32 length , bool repeat = false ) : 
			_sound( repeat ? _channelType::repeatstatic : _channelType::oneshotstatic )
			, frequency( frequency )
			, format( format )
			, data( data )
			, length( length )
			, repeat( repeat )
		{}
		
		//! Make available from base class
		void setFrequency( _u16 freq );
		using _sound::getFrequency;
};

#endif