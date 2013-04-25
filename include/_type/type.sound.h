#ifndef _WIN_T_SOUND_
#define _WIN_T_SOUND_

#include "_type/type.h"
#include <maxmod9.h>


enum class _soundEncoding
{
	mono8 = 0 ,
	stereo8 ,
	mono16 ,
	stereo16
};

class _sound
{
	private:
		
		const _u8*		data;
		_u32 			length;
		_soundEncoding	encoding;
		_u16			frequency;
		bool			runs;
		
		mm_word handler( _length length , void* dest );
		
		static mm_word soundDispatcher( mm_word length , mm_addr dest , mm_stream_formats format );
		
	public:
	
		//! Ctor
		_sound( const _u8* data , _u32 length , _soundEncoding enc , _u16 freq = 44100 );
		
		//! Play the sound at a specific volume and panning
		void play( _u8 volume = 127 , _s8 pan = 0 /* -64 <= pan <= +64 */ );
		
		//! Termiate the execution of a sound
		void terminate();
		
		//! Enable/Disable Sound Functionality
		static void enable();
		static void disable();
};

#endif