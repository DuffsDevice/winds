#include "_type/type.mime.h"

map<string,_mime> string2mimeType = {
	{ "directory" , directory } ,
	{ "image/jpeg" , image_jpeg } ,
	{ "image/png" , image_png } ,
	{ "image/gif" , image_gif } ,
	{ "text/plain" , text_plain } ,
	{ "text/html" , text_html } ,
	{ "text/xml" , text_xml } ,
	{ "application/microsoft-installer" , application_microsoft_installer } ,
	{ "application/octet-stream" , application_octet_stream } ,
	{ "application/x-lua-bytecode" , application_x_lua_bytecode } ,
	{ "application/x-ms-shortcut" , application_x_ms_shortcut } ,
	{ "application/x-internet-shortcut" , application_x_internet_shortcut } ,
};

// File Extensions#
map<string,_mime> extension2mimeType = {
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
	{ "msi" , application_microsoft_installer } ,
	{ "lnk" , application_x_ms_shortcut } ,
	{ "url" , application_x_internet_shortcut }
};

map<_mime,string> mimeType2string = {
	{ directory , "directory" } , 
	{ image_jpeg , "image/jpeg" } , 
	{ image_png, "image/png" } ,
	{ image_gif, "image/gif" } ,
	{ text_plain, "text/plain" } ,
	{ text_html, "text/html" } ,
	{ text_xml , "text/xml" } ,
	{ application_octet_stream , "application/octet-stream" } ,
	{ application_microsoft_installer , "application/microsoft-installer" } ,
	{ application_x_lua_bytecode , "application/x-lua-bytecode" } ,
	{ application_x_ms_shortcut , "application/x-ms-shortcut" } ,
	{ application_x_internet_shortcut , "application/x-internet-shortcut" } 
};