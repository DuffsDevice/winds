#include "_type/type.windows.soundbank.h"
#include "_type/type.sound.static.h"
#include <stdlib.h>

// Sound Files
#include "sound_alert_22_pcm.h"
#include "sound_click_44_pcm.h"
#include "sound_bubble_22_pcm.h"
#include "sound_error_22_pcm.h"
#include "sound_fatalerror_22_pcm.h"
#include "sound_logoff_22_pcm.h"
#include "sound_logon_22_pcm.h"
#include "sound_minimize_22_pcm.h"
#include "sound_navigation_44_pcm.h"
#include "sound_ping_22_pcm.h"
#include "sound_recycle_22_pcm.h"
#include "sound_restore_22_pcm.h"
#include "sound_startup_22_pcm.h"
#include "sound_shutdown_22_pcm.h"

void _windowsSoundBank::play( _windowsSound snd )
{
	switch( snd )
	{
		case _windowsSound::keyClick:{
			static _staticSound sound{ _channelFormat::bit8 , 44100 , sound_click_44_pcm , sound_click_44_pcm_size };
			sound.stop();
			sound.setFrequency( 36600 + rand() % 15000 );
			sound.play();
		}break;
		case _windowsSound::navigation:{
			static _staticSound sound{ _channelFormat::bit8 , 44100 , sound_navigation_44_pcm , sound_navigation_44_pcm_size };
			sound.stop();
			sound.play();
		}break;
		case _windowsSound::ping:{
			static _staticSound sound{ _channelFormat::bit8 , 22050 , sound_ping_22_pcm , sound_ping_22_pcm_size };
			sound.stop();
			sound.play();
		}break;
		case _windowsSound::startUp:{
			static _staticSound sound{ _channelFormat::bit8 , 22050 , sound_startup_22_pcm , sound_startup_22_pcm_size };
			sound.stop();
			sound.play();
		}break;
		case _windowsSound::shutDown:{
			static _staticSound sound{ _channelFormat::bit8 , 22050 , sound_shutdown_22_pcm , sound_shutdown_22_pcm_size };
			sound.stop();
			sound.play();
		}break;
		case _windowsSound::logOn:{
			static _staticSound sound{ _channelFormat::bit8 , 22050 , sound_logon_22_pcm , sound_logon_22_pcm_size };
			sound.stop();
			sound.play();
		}break;
		case _windowsSound::logOff:{
			static _staticSound sound{ _channelFormat::bit8 , 22050 , sound_logoff_22_pcm , sound_logoff_22_pcm_size };
			sound.stop();
			sound.play();
		}break;
		case _windowsSound::alert:{
			static _staticSound sound{ _channelFormat::bit8 , 22050 , sound_alert_22_pcm , sound_alert_22_pcm_size };
			sound.stop();
			sound.play();
		}break;
		case _windowsSound::error:{
			static _staticSound sound{ _channelFormat::bit8 , 22050 , sound_error_22_pcm , sound_error_22_pcm_size };
			sound.stop();
			sound.play();
		}break;
		case _windowsSound::fatalError:{
			static _staticSound sound{ _channelFormat::bit8 , 22050 , sound_fatalerror_22_pcm , sound_fatalerror_22_pcm_size };
			sound.stop();
			sound.play();
		}break;
		case _windowsSound::minimize:{
			static _staticSound sound{ _channelFormat::bit8 , 22050 , sound_minimize_22_pcm , sound_minimize_22_pcm_size };
			sound.stop();
			sound.play( 30 );
		}break;
		case _windowsSound::restore:{
			static _staticSound sound{ _channelFormat::bit8 , 22050 , sound_restore_22_pcm , sound_restore_22_pcm_size };
			sound.stop();
			sound.play( 30 );
		}break;
		case _windowsSound::recycle:{
			static _staticSound sound{ _channelFormat::bit8 , 22050 , sound_recycle_22_pcm , sound_recycle_22_pcm_size };
			sound.stop();
			sound.play( 30 );
		}break;
		case _windowsSound::bubble:{
			static _staticSound sound{ _channelFormat::bit8 , 22050 , sound_bubble_22_pcm , sound_bubble_22_pcm_size };
			sound.stop();
			sound.play();
		}break;
	}
}