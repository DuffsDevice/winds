// Check if already included
#ifndef _WIN_G_IMAGEBUTTON_
#define _WIN_G_IMAGEBUTTON_

#include "_type/type.gadget.h"
#include "_gadget/gadget.button.h"

class _imageButton : public _button {
	
	protected:
		
		_bitmap image;
		_bitmap pressedImage;
		
		static _callbackReturn refreshHandler( _event );
		
	public:
		
		//! Set Image to be displayed by default
		void setImage( _bitmap img ){ this->image = img; if( !this->pressed ) this->redraw(); }
		
		//! Set Image to be displayed upon press
		void setImagePressed( _bitmap img ){ this->pressedImage = img; if( this->pressed ) this->redraw(); }
		
		//! Getters...
		const _bitmap& getImage() const { return this->image; }
		const _bitmap& getImagePressed() const { return this->pressedImage; }
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_imageButton( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _bitmap image , _bitmap imagePressed , string title = "" , _style&& style = _style() );
};

#endif