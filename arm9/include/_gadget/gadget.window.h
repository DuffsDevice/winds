// Check if already included
#ifndef _WIN_G_WINDOW_
#define _WIN_G_WINDOW_

#include "_type/type.gadget.h"
#include "_type/type.bitmap.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.image.h"
#include "_gadget/gadget.window.button.h"

class _window;

using _windowTaskHandler = _callback<void(_window*)>;

class _window : public _gadget {
	
	private:
		
		friend class _mainFrame;
		
		_label*			label;
		_imageGadget*	icon;
		_color			bgColor;
		
		_windowButton*	button[3];
		
		_rect*			normalDimensions; // Ptr where original dimensions are stored when maximized
		bool			minimizeable : 1;
		bool			minimized : 1;
		bool			closeable : 1;
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn focusHandler( _event );
		static _callbackReturn dragHandler( _event );
		static _callbackReturn maximizeHandler( _event );
		static _callbackReturn parentSetHandler( _event );
		static _callbackReturn restyleHandler( _event ); // Will be called when something like "style.resizeable" is changed
		static _callbackReturn doubleClickHandler( _event );
		_callbackReturn buttonHandler( _event ); // Handler for _window-Buttons
		
		//! Will be called if the window is resized ->label will also be resized
		static _callbackReturn updateHandler( _event );
		
		//! List that holds all window's that show up in the taskbar
		static _list<_window*>							taskWindows;
		static _list<_uniquePtr<_windowTaskHandler>>	taskHandlers;
		
		//! Notifys all listeners that want to be informed if the window maximizes, focuses
		void notifyTaskHandlers( bool onlyIfWindowIsTask = true );
		
		//! Checks wether this window meets the requirements of a task
		//! If it does, it pushes this window onto the list of 'taskWIndows'
		//! If not, removes it from the list
		void checkIfTask();
		
	public:
		
		//! Set Title of window
		void setStrValue( string title );
		
		//! Get the title
		string getStrValue() const { return this->label->getStrValue(); }
		
		//! Set Icon to show in the top left corner
		void setIcon( const _bitmap& icon );
		
		//! Check if the window has an Icon
		bool hasIcon() const { return this->icon->getImage().isValid(); }
		
		//! Set whether the window can be closed
		void setCloseable( bool flag ){ this->closeable = flag; this->update(); }
		
		//! Check whether the window can be closed
		bool isCloseable() const { return this->closeable; }
		
		//! Maximize the window to full screen size
		void maximize();
		
		//! unMaximize the window to its original size
		void unMaximize();
		
		//! Minimize the window into the taskbar
		void minimize();
		
		//! Restore the window from the taskbar
		void restore();
		
		//! Close the window
		void close();
		
		//! Check whether the window is currently maximized
		bool isMaximized() const { return this->normalDimensions != nullptr; }
		
		//! Check whether the window is currently minimized
		bool isMinimized() const { return this->minimized; }
		
		//! Check whether the window is minimizeable
		bool isMinimizeable() const { return this->minimizeable; }
		
		//! Set whether the window is minimizeable
		void setMinimizeable( bool val ){ this->minimizeable = val; this->update(); }
		
		//! Get the window's icon
		_constBitmap& getIcon() const { return this->icon->getImage(); }
		
		//! Checks wether this window meets the requirements of a task
		bool isTask(){ return this->isMinimizeable() && this->getParent(); }
		
		//! Set Background color of the window
		void setBgColor( _color value ){
			if( this->bgColor == value )
				return;
			this->bgColor = value;
			this->redraw();
		}
		
		//! Get background color
		_color getBgColor(){ return this->bgColor; }
		
		//! Ctor
		_window( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string title , bool minimizeable = true , bool closeable = true , _style&& style = _style() ) :
			_window( x , y , width , height , title , _bitmap() , minimizeable , closeable , (_style&&)style ) // C++0x! Yay!
		{}
		
		//! Ctor with icon
		_window( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string title , _bitmap icon , bool minimizeable = true , bool closeable = true , _style&& style = _style() );
		
		//! Dtor
		~_window();
		
		
		//! Retrieve the list of minimizeable window's
		static const _list<_window*>& getTaskWindows(){
			return _window::taskWindows;
		}
		
		//! Unregister a handler from the 'notify on minimize'-list
		static void removeTaskHandler( const _windowTaskHandler& cb ){
			taskHandlers.remove_if( [&cb]( _windowTaskHandler* val )->bool{ return (cb == *val) == 1; } );
		}
		
		//! Register a handler taht will be notified, when the window changes its appearence
		static void addTaskHandler( _paramAlloc<_windowTaskHandler> cb ){
			taskHandlers.emplace_back( cb.get() );
		}
		
};

#endif