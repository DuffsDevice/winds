#ifndef _WIN_T_SOUND_STREAM_
#define _WIN_T_SOUND_STREAM_

#include "_type/type.h"
#include "_type/type.sound.h"
#include "_type/type.uniqueptr.h"

class _soundStream : public _sound
{
	private:
		
		_callback<_fillingRoutine>*	filler;
		_u16						frequency;
		_channelFormat				format;
		union{
			_u8*					buffer8;
			_u16*					buffer16;
		};
		_u32						length;
		
		//! Prepares the channel for use and returns its number
		_s8 prepareChannel( _u8 volume , _s8 panning ) override;
		
		//! Frees the allocated stream buffer
		void cleanupChannel( _u8 channel ) override;
		
	public:
		
		//! Ctor
		_soundStream( _channelFormat format , _u16 frequency , _u32 length , _paramAlloc<_callback<_fillingRoutine>> filler ) : 
			_sound( _channelType::stream )
			, filler( filler.get() )
			, frequency( frequency )
			, format( format )
			, buffer8( nullptr )
			, length( length )
		{}
		
		//! Dtor
		~_soundStream(){
			delete this->filler;
		}
		
		//! Make available from base class
		void setFrequency( _u16 freq );
		using _sound::getFrequency;
};

#endif