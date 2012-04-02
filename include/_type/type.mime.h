#ifndef _WIN_T_MIMETYPE_
#define _WIN_T_MIMETYPE_

#include "_type/type.h"
#include "_type/type.bitmap.h"

typedef enum{
	plain = 0 ,
	image_jpeg ,
	image_png ,
	image_gif ,
	text_plain ,
	text_html ,
	text_xml ,
	application_octet_stream ,
	application_microsoft_installer,
	application_x_lua_bytecode
}_mime;

extern map<string,_mime> string2mimeType;
extern map<_mime,string> mimeType2string;
	

class _mimeType{
	
	private:
	
		_mime type;
		
	public:
	
		_mimeType( string str ) :
			type( string2mimeType[ str ] )
		{ }
		
		_mimeType( _mime type ) :
			type( type )
		{ }
		
		operator string(){
			return mimeType2string[ this->type ];
		}
		
		operator _mime(){
			return this->type;
		}
		
		const _bitmap* getFileImage() const ;
		
		const _bitmap* getFolderImage() const ;
};
#endif