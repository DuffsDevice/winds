// Check if already included
#ifndef _WIN_G_WINDOW_
#define _WIN_G_WINDOW_

#include "_type/type.gadget.h"
#include "_type/type.bitmap.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.button.h"
#include "_gadget/gadget.imagegadget.h"

class _windowButton : public _button{
	private:
		_u8 buttonType;
		static _callbackReturn refreshHandler( _event event );
	public:
		_windowButton( _coord x , _coord y , _u8 buttonType );
};

class _window : public _gadget {
	
	private:
		
		_label*			label;
		_imagegadget*	icon;
		
		bool 			dragMe;
		
		_windowButton*	button[3];
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn dragHandler( _event event );
		_callbackReturn buttonHandler( _event event ); // Handler for _window-Buttons
		static _callbackReturn restyleHandler( _event event );
		
		// Will be called if the window is resized ->label will also be resized
		static _callbackReturn resizeHandler( _event e );
		
	public:
		
		//! Set Title of window
		void setStrValue( string title );
		
		//! Get the title
		string getStrValue() const { return this->label->getStrValue(); }
		
		//! Set Icon to show in the top left corner
		void setIcon( _bitmap icon );
		
		//! Check if the window has an Icon
		bool hasIcon() const { return this->icon->getImage().isValid(); }
		
		//! Get the windows icon
		_bitmap* getIcon(){ return &this->icon->getModifyableImage(); }
		
		//! Ctor
		_window( _length width , _length height , _coord x , _coord y , string title , _style style = _style() );
		
		//! Ctor with icon
		_window( _length width , _length height , _coord x , _coord y , string title , _bitmap icon , _style style = _style() );
		
		//! Dtor
		~_window();
};

#endif