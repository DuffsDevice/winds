#include "_gadget/gadget.button.image.h"

_callbackReturn _imageButton::refreshHandler( _event event )
{
	// Receive Gadget
	_imageButton* that = event.getGadget<_imageButton>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	bP.copy( 0 , 0 , that->pressed ? that->pressedImage : that->image );
	
	// If there is no font it doesn't make sense to paint
	if( !that->font || !that->font->isValid() )
		return use_default;
	
	_coord x = 0;
	_coord y = 0;
	
	switch( that->getAlign() )
	{
		case _align::center:
			x = ( myW - that->font->getStringWidth( that->getStrValue() , that->fontSize ) + 1 ) >> 1;
			break;
		case _align::left:
			x = 0;
			break;
		case _align::right:
			x = that->getWidth() - that->font->getStringWidth( that->getStrValue() , that->fontSize );
			break;
	}
	
	switch( that->getVAlign() )
	{
		case _valign::middle:
			y = ( ( myH - that->font->getAscent( that->fontSize ) - 2 ) >> 1 );
			break;
		case _valign::top:
			y = 0;
			break;
		case _valign::bottom:
			y = that->getHeight() - that->font->getAscent( that->fontSize );
			break;
	}
	
	// Draw Text...
	bP.drawString( x , y , that->font , that->getStrValue() , that->fontColor , that->fontSize );
	
	return use_default;
}

_imageButton::_imageButton( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _bitmap image , _bitmap imagePressed , string value , _style&& style ) :
	_button( x , y , width , height , value , (_style&&)style )
	, image( (_bitmap&&)image )
	, pressedImage( (_bitmap&&)imagePressed )
{
	// Indicate, I am no normal button!
	this->setType( _gadgetType::imagebutton );
	
	// Register modified handler
	this->setInternalEventHandler( onDraw , make_callback( &_button::refreshHandler ) );
	
	// Refresh
	this->redraw();
}