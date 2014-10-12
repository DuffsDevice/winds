// Check if already included
#ifndef _WIN_G_WINDOW_MAINFRAME_
#define _WIN_G_WINDOW_MAINFRAME_

#include <_gadget/gadget.window.h>
#include <_type/type.program.handle.h>

class _mainFrame : public _window
{
	private:
		
		_programHandle	programHandle;
		string			title;
		
		//! handler to handle onClose-events triggered by the base class _window
		static _callbackReturn closeHandler( _event );
		
		//! Get the String that should be displayed as the title of the window
		static string	getDisplayName( _programHandle handle );
		static _bitmap	getIcon( _programHandle handle );
		
	public:
		
		//! Set Title of window
		void setTitle( string title );
		
		//! Get the title
		string getTitle() const { return this->title; }
		
		//! Ctor
		_mainFrame( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _programHandle progHandle , _style&& style = _style() );
};
#endif