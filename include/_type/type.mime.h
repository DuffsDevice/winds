#ifndef _WIN_T_MIMETYPE_
#define _WIN_T_MIMETYPE_

#include "_type/type.h"

typedef enum{
	plain = 0 ,
	directory,
	image_jpeg ,
	image_png ,
	image_gif ,
	text_plain ,
	text_html ,
	text_xml ,
	text_x_ini ,
	application_octet_stream ,
	application_microsoft_installer,
	application_x_lua_bytecode,
	application_x_ms_shortcut,
	application_x_internet_shortcut,
	application_x_nintendo_ds_rom,
	application_x_nintendo_gba_rom
}_mime;

extern map<string,_mime> string2mimeType;
extern map<string,_mime> extension2mimeType;
extern map<_mime,string> mimeType2string;
	

class _mimeType{
	
	private:
	
		_mime type;
		
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
};
#endif