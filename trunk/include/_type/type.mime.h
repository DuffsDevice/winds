#ifndef _WIN_T_MIMETYPE_
#define _WIN_T_MIMETYPE_

#include "_type/type.h"

typedef enum{
	plain = 0 ,
	directory,
	image_jpeg ,
	image_png ,
	image_gif ,
	image_bmp ,
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
	application_x_internet_shortcut,
	application_x_nintendo_ds_rom,
	application_x_nintendo_gba_rom,
	application_x_bat
}_mime;

extern _map<string,_mime> string2mimeType;
extern _map<string,_mime> extension2mimeType;
extern _map<_mime,string> mimeType2string;
	

class _mimeType{
	
	private:
	
		_mime type : 5;
		
	public:
	
		_mimeType( string str ) :
			type( string2mimeType[ str ] )
		{ }
		
		_mimeType( _mime type = _mime::plain ) :
			type( type )
		{ }
		
		static _mimeType fromExtension( string ext ){
			return extension2mimeType[ext];
		}
		
		operator string(){
			return mimeType2string[ this->type ];
		}
		
		operator _mime(){
			return this->type;
		}
} __attribute__(( packed ));
#endif