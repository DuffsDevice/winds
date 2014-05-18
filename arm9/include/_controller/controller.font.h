#ifndef _WIN_S_FONT_
#define _WIN_S_FONT_

#include "_type/type.controller.h"
#include "_type/type.font.h"
#include "_type/type.font.handle.h"
#include "_type/type.uniqueptr.h"
#include <algorithm>

using _fontList = _list<_uniquePtr<_font>>;

class _fontController : public _controller
{
	private:
		
		static _fontList	registeredFonts;
		static _fontPtr		standardFont;
		static _u8			standardFontSize;
		
	public:
		
		//! Inits the font controller
		static bool				init();
		
		//! Shuts down the font controller
		static void				end();
		
		//! Give the _font-Manager the full control over the supplied font (that includes destruction)
		static void				registerFont( _uniquePtr<_font> font );
		
		//! Check if the supplied font pointer is still valid
		static bool				isExistent( _fontPtr font );
		static bool				isExistent( const string& fontName );
		
		//! Get the one font in the font-manager which name euqlas the supplied string
		static _fontPtr			getFont( _literal fontName );
		static _fontPtr			getFont( const string& fontName );
		
		//! Get the standard system font
		static _fontPtr			getStandardFont(){
			return _fontController::standardFont;
		}
		
		//! Set the standard system font
		static void				setStandardFont( _fontPtr font );
		
		//! Get the standard font-size
		static _u8				getStandardFontSize(){ return standardFontSize; }
		
		//! Set the standard font size
		static void				setStandardFontSize( _u8 fontSize ){ standardFontSize = fontSize; }
		
		//! Delete the supplied font in the font-manager
		static void				deleteFont( _fontPtr font );
		
		//! Get a list of all currently registered fonts
		static const _fontList&	getRegisteredFonts(){ return registeredFonts; }
};

#endif