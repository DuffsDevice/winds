// Check if already included
#ifndef _WIN_G_WINDOW_
#define _WIN_G_WINDOW_

#include "_type/type.gadget.h"
#include "_type/type.bitmap.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.imagegadget.h"
#include "_gadget/gadget.window.button.h"


class _window : public _gadget {
	
	private:
		
		friend class PROG_Explorer;
		
		_label*			label;
		_imagegadget*	icon;
		
		_windowButton*	button[3];
		
		_rect*			normalDimensions; // Ptr where original dimensions are stored when maximized
		bool			minimizeable : 1;
		bool			minimized : 1;
		bool			closeable : 1;
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn focusHandler( _event event );
		static _callbackReturn dragHandler( _event event );
		_callbackReturn buttonHandler( _event event ); // Handler for _window-Buttons
		
		//! Will be called if the window is resized ->label will also be resized
		static _callbackReturn updateHandler( _event event ); // Will be called when something like "style.resizeable" is changed
		static _callbackReturn mouseClickHandler( _event event );
		
	public:
		
		//! Set Title of window
		void setStrValue( string title );
		
		//! Get the title
		string getStrValue() const { return this->label->getStrValue(); }
		
		//! Set Icon to show in the top left corner
		void setIcon( _bitmap icon );
		
		//! Check if the window has an Icon
		bool hasIcon() const { return this->icon->getImage().isValid(); }
		
		//! Set whether the window can be closed
		void setCloseable( bool flag ){ this->closeable = flag; this->update(); }
		
		//! Check whether the window can be closed
		bool isCloseable() const { return this->closeable; }
		
		//! Maximize the window to full screen size
		void maximize();
		
		//! unMaximize the widnwo to its original size
		void unMaximize();
		
		//! Minimize the window into taskbar
		void minimize();
		
		//! Restore the window from taskbar
		void restore();
		
		//! Check whether the window is currently maximized
		bool isMaximized() const { return this->normalDimensions != nullptr; }
		
		//! Check whether the window is currently minimized
		bool isMinimized() const { return this->minimized; }
		
		//! Check whether the window is minimizeable
		bool isMinimizeable() const { return this->minimizeable; }
		
		//! Set whether the window is minimizeable
		void setMinimizeable( bool val ){ this->minimizeable = val; this->update(); }
		
		//! Get the window's icon
		const _bitmap& getIcon() const { return this->icon->getImage(); }
		
		//! Ctor
		_window( _length width , _length height , _coord x , _coord y , string title , bool minimizeable = true , bool closeable = true , _style&& style = _style() | _styleAttr::draggable ) :
			_window( width , height , x , y , title , _bitmap() , minimizeable , closeable , (_style&&)style ) // C++0x! Yay!
		{ }
		
		//! Ctor with icon
		_window( _length width , _length height , _coord x , _coord y , string title , _bitmap icon , bool minimizeable = true , bool closeable = true , _style&& style = _style() | _styleAttr::draggable );
		
		//! Dtor
		~_window();
} PACKED ;

#endif