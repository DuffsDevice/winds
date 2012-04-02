#include "_type/type.mime.h"
#include "_graphic/BMP_FileIcon.h"
#include "_graphic/BMP_ExeIcon.h"
#include "_graphic/BMP_TxtIcon.h"
#include "_graphic/BMP_XmlIcon.h"
#include "_graphic/BMP_FolderIcon.h"
#include "_graphic/BMP_LuaIcon.h"

_bitmap* icon_application_octet_stream = new BMP_ExeIcon();
_bitmap* icon_application_x_lua_bytecode = new BMP_LuaIcon();
_bitmap* icon_application_microsoft_installer = new BMP_FileIcon();
_bitmap* icon_plain = new BMP_FileIcon();
_bitmap* icon_folder = new BMP_FolderIcon();
_bitmap* icon_text_plain = new BMP_TxtIcon();
_bitmap* icon_text_xml = new BMP_XmlIcon();

map<string,_mime> string2mimeType = {
	{ "image/jpeg" , image_jpeg } ,
	{ "image/png" , image_png } ,
	{ "image/gif" , image_gif } ,
	{ "text/plain" , text_plain } ,
	{ "text/html" , text_html } ,
	{ "text/xml" , text_xml } ,
	{ "application/microsoft-installer" , application_microsoft_installer } ,
	{ "application/octet-stream" , application_octet_stream } ,
	{ "application/x-lua-bytecode" , application_x_lua_bytecode } ,
	
	// File Extensions
	{ "jpg" , image_jpeg } ,
	{ "jpeg" , image_jpeg } ,
	{ "png" , image_png } ,
	{ "gif" , image_gif } ,
	{ "txt" , text_plain } ,
	{ "html" , text_html } ,
	{ "htm" , text_html } ,
	{ "xml" , text_xml } ,
	{ "xhtml" , text_xml } ,
	{ "exe" , application_octet_stream } ,
	{ "lua" , application_x_lua_bytecode } ,
	{ "msi" , application_microsoft_installer }
};

map<_mime,string> mimeType2string = {
	{ image_jpeg , "image/jpeg" } , 
	{ image_png, "image/png" } ,
	{ image_gif, "image/gif" } ,
	{ text_plain, "text/plain" } ,
	{ text_html, "text/html" } ,
	{ text_xml , "text/xml" } ,
	{ application_octet_stream , "application/octet-stream" } ,
	{ application_microsoft_installer , "application/microsoft-installer" }  ,
	{ application_x_lua_bytecode , "application/x-lua-bytecode" } 
};

const _bitmap* _mimeType::getFolderImage() const {
	return icon_folder;
}

const _bitmap* _mimeType::getFileImage() const 
{
	switch( this->type ){
		case _mime::application_octet_stream:
			return icon_application_octet_stream;
			break;
		case _mime::application_x_lua_bytecode:
			return icon_application_x_lua_bytecode;
			break;
		case _mime::application_microsoft_installer:
			return icon_application_microsoft_installer;
			break;
		case _mime::text_plain:
			return icon_text_plain;
			break;
		case _mime::text_xml:
			return icon_text_xml;
			break;
		default:
			return icon_plain;
			break;
	}
}