#include "_type/type.mime.h"

map<string,_mime> string2mimeType = {
	{ "directory" , directory } ,
	{ "image/jpeg" , image_jpeg } ,
	{ "image/png" , image_png } ,
	{ "image/gif" , image_gif } ,
	{ "image/bmp" , image_bmp } ,
	{ "text/plain" , text_plain } ,
	{ "text/html" , text_html } ,
	{ "text/xml" , text_xml } ,
	{ "text/x-ini" , text_x_ini } ,
	{ "application/microsoft-installer" , application_microsoft_installer } ,
	{ "application/octet-stream" , application_octet_stream } ,
	{ "application/x-lua-bytecode" , application_x_lua_bytecode } ,
	{ "application/x-ms-shortcut" , application_x_ms_shortcut } ,
	{ "application/x-internet-shortcut" , application_x_internet_shortcut } ,
	{ "application/x-nintendo-ds-rom" , application_x_nintendo_ds_rom } ,
	{ "application/x-nintendo-gba-rom" , application_x_nintendo_gba_rom }
};

// File Extensions#
map<string,_mime> extension2mimeType = {
	{ "jpg" , image_jpeg } ,
	{ "jpeg" , image_jpeg } ,
	{ "png" , image_png } ,
	{ "gif" , image_gif } ,
	{ "bmp" , image_bmp } ,
	{ "txt" , text_plain } ,
	{ "html" , text_html } ,
	{ "htm" , text_html } ,
	{ "xml" , text_xml } ,
	{ "xhtml" , text_xml } ,
	{ "ini" , text_x_ini } ,
	{ "sav" , text_x_ini } ,
	{ "opt" , text_x_ini } ,
	{ "exe" , application_octet_stream } ,
	{ "lua" , application_x_lua_bytecode } ,
	{ "msi" , application_microsoft_installer } ,
	{ "lnk" , application_x_ms_shortcut } ,
	{ "url" , application_x_internet_shortcut },
	{ "nds" , application_x_nintendo_ds_rom } ,
	{ "gba" , application_x_nintendo_gba_rom } ,
	{ "gbc" , application_x_nintendo_gba_rom }
};

map<_mime,string> mimeType2string = {
	{ directory , "directory" } , 
	{ image_jpeg , "image/jpeg" } , 
	{ image_png, "image/png" } ,
	{ image_gif, "image/gif" } ,
	{ image_bmp, "image/bmp" } ,
	{ text_plain, "text/plain" } ,
	{ text_html, "text/html" } ,
	{ text_xml , "text/xml" } ,
	{ text_x_ini , "text/x-ini" } ,
	{ application_octet_stream , "application/octet-stream" } ,
	{ application_microsoft_installer , "application/microsoft-installer" } ,
	{ application_x_lua_bytecode , "application/x-lua-bytecode" } ,
	{ application_x_ms_shortcut , "application/x-ms-shortcut" } ,
	{ application_x_internet_shortcut , "application/x-internet-shortcut" } ,
	{ application_x_nintendo_ds_rom , "application/x-nintendo-ds-rom" } ,
	{ application_x_nintendo_gba_rom , "application/x-nintendo-gba-rom" } 
};