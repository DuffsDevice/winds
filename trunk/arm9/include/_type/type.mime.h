#ifndef _WIN_T_MIMETYPE_
#define _WIN_T_MIMETYPE_

#include <_type/type.h>

enum class _mime : _u8{
	unknown,
	directory,
	image_jpeg ,
	image_png ,
	image_gif ,
	image_bmp ,
	image_ico ,
	text_plain ,
	text_html ,
	text_xml ,
	text_x_ini ,
	font_opentype,
	font_truetype,
	audio_mpeg,
	audio_wav,
	audio_x_aiff,
	audio_mid,
	audio_x_mpegurl,
	audio_x_mod,
	audio_x_midi,
	application_octet_stream ,
	application_microsoft_installer,
	application_x_lua_bytecode,
	application_x_ms_shortcut,
	application_x_ms_registry,
	application_x_internet_shortcut,
	application_x_nintendo_ds_rom,
	application_x_nintendo_gba_rom,
	application_x_bat,
	application_zip
};

extern _fromStr<_mime>	string2mimeType;
extern _fromStr<_mime>	extension2mimeType;
extern _toStr<_mime>	mimeType2string;
extern _toStr<_mime>	mimeType2name;
	

class _mimeType{
	
	private:
	
		_mime type;
		
	public:
	
		//! Mime-Type from string
		_mimeType( string str ) :
			type( string2mimeType[str] )
		{ }
		
		//! Ctor
		_mimeType( _mime type = _mime::unknown ) :
			type( type )
		{ }
		
		//! Create Mime Type from extension
		static _mimeType fromExtension( string ext ){
			return extension2mimeType[ext];
		}
		
		//! Convert to string
		operator _literal(){
			return mimeType2string[ this->type ];
		}
		
		//! Convert to raw type
		operator _mime(){
			return this->type;
		}
		
		//! Comparison operator
		bool operator==( _mime other ){
			return other == type;
		}
		
		//! Get the Name of the Mime Type
		_literal getName(){
			return mimeType2name[ this->type ];
		}
};
#endif