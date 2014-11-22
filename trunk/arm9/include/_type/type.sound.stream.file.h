#ifndef _WIN_T_SOUND_FILE_STREAM_
#define _WIN_T_SOUND_FILE_STREAM_

#include <_type/type.h>
#include <_type/type.sound.h>
#include <_type/type.callback.derives.h>

class _fileSoundStream;

//! Class to forward any event to any other
class _fileSoundStreamFiller : public _dummyCallback<_fillingRoutine>
{
	private:
		_fileSoundStream&	reference;
		_u32				executor( _u32 requestedSize , _s8* destination , _u16 frequency , _channelFormat format ) const ;
	public:
		// Ctor
		_fileSoundStreamFiller( _fileSoundStream& ref ) :
			_dummyCallback<_fillingRoutine>( &_fileSoundStreamFiller::executor )
			, reference( ref )
		{}
};

class _fileSoundStream : public _sound
{
	private:
		
		_fileSoundStreamFiller*		filler;
		_u16						frequency;
		_channelFormat				format;
		union{
			_u8*					buffer8;
			_u16*					buffer16;
		};
		_u32						length;
		_direntry					source;
		
		// Friend the filler class
		friend class _fileSoundStreamFiller;
		
		//! Prepares the channel for use and returns its number
		_s8 prepareChannel( _u8 volume , _s8 panning ) override;
		
		//! Frees the allocated stream buffer
		void cleanupChannel( _u8 channel ) override;
		
		//! Checks, if the source file can be opened
		bool isChannelReady() override {
			return this->source.openRead();
		}
		
	public:
		
		//! Ctor
		_fileSoundStream( string filename , _channelFormat format , _u16 frequency , _u32 length = 8 * 1024 ) : 
			_sound( _channelType::stream )
			, filler( new _fileSoundStreamFiller(*this) )
			, frequency( frequency )
			, format( format )
			, buffer8( nullptr )
			, length( length )
			, source( filename )
		{}
		
		//! Make available from base class
		void setFrequency( _u16 freq );
		using _sound::getFrequency;
};

#endif