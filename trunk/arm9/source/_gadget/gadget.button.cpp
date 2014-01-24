#include "_gadget/gadget.button.h"
#include "_type/type.system.h"

void _button::setStrValue( string val )
{
	if( this->strValue == val )
		return;
	
	// Set Value...
	this->strValue = val;
	
	// Inform that maybe, size should change now
	this->update();
	
	// ...and refresh!
	this->redraw();
}

void _button::setFont( const _font* ft )
{
	if( this->font == ft || !ft )
		return;
	
	this->font = ft;
	
	// Inform that maybe, size should change now
	this->update();
	
	// ...and refresh!
	this->redraw();
}

void _button::setFontSize( _u8 fontSize )
{
	if( this->fontSize == fontSize )
		return;
	
	// Set FontSize...
	this->fontSize = fontSize;
	
	// Inform that maybe, size should change now
	this->update();
	
	// ...and refresh!
	this->redraw();
}

_callbackReturn _button::updateHandler( _event event )
{
	// Receive Gadget
	_button* that = event.getGadget<_button>();
	
	if( !that->font || !that->font->isValid() )
		return handled;
	
	that->setSizeIfAuto(
		max( 28 , that->font->getStringWidth( that->getStrValue() , that->fontSize ) + 7 )
		, that->font->getHeight( that->fontSize ) + 2
	);
	return handled;
}

_callbackReturn _button::refreshHandler( _event event )
{
	// Receive Gadget
	_button* that = event.getGadget<_button>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	if( that->pressed )
	{
		// Background
		bP.fill( RGB( 25 , 25 , 25 ) );
		
		// Inner Shadow
		bP.drawHorizontalLine( 1 , 1 , myW - 3 , RGB( 20 , 20 , 20 ) );
		bP.drawHorizontalLine( 2 , myH - 2 , myW - 4 , RGB( 29 , 29 , 28 ) );
		bP.drawVerticalLine( 1 , 2 , myH - 4 , RGB( 20 , 20 , 20 ) );
	}
	else
	{
		// Background
		bP.fill( RGB( 31 , 31 , 31 ) );
		
		if( that->autoSelect )
		{
			// Blue Border
			bP.drawRect( 1 	, 1 , myW - 2 , myH - 2 , RGB( 18 , 22 , 31 ) );
			bP.drawRect( 2 	, 2 , myW - 4 , myH - 4 , RGB( 26 , 29 , 31 ) );
		}
		else
		{
			// Inner Shadow
			bP.drawHorizontalLine( 2 	, myH - 2 , myW - 4 , RGB( 25 , 25 , 25 ) );
			bP.drawVerticalLine( myW - 2 	, 2 	  , myH - 4 , RGB( 25 , 25 , 25 ) );
			bP.drawHorizontalLine( 2 	, 1 , myW - 4 , RGB( 29 , 29 , 29 ) );
			bP.drawVerticalLine( 1 	, 2 	  , myH - 4 , RGB( 29 , 29 , 29 ) );
			// Corners
			bP.drawPixel( myW - 2 	, myH - 2 , RGB( 23 , 23 , 23 ) );
			bP.drawPixel( myW - 2 	, 1 	   , RGB( 23 , 23 , 23 ) );
			bP.drawPixel( 1			, 1 	   , RGB( 23 , 23 , 23 ) );
			bP.drawPixel( 1			, myH - 2 , RGB( 23 , 23 , 23 ) );
		}
		
		//bP.drawHorizontalLine( 2 , myH - 3 , myW - 4 , RGB( 29 , 29 , 29 ) );		
	}
	
	// Border
	bP.drawVerticalLine( 0 		, 2 	  , myH - 4 , RGB( 0 , 10 , 17 ) );
	bP.drawVerticalLine( myW - 1 , 2 	  , myH - 4 , RGB( 0 , 10 , 17 ) );
	bP.drawHorizontalLine( 2 	, 0 	  , myW - 4 , RGB( 0 , 10 , 17 ) );
	bP.drawHorizontalLine( 2 	, myH - 1 , myW - 4 , RGB( 0 , 10 , 17 ) );
	
	// Corners
	bP.drawPixel( myW - 1 , myH - 2 , RGB( 6 , 14 , 19 ) ); // Bottom Right
	bP.drawPixel( myW - 2 , myH - 1 , RGB( 6 , 14 , 19 ) ); // Bottom Right
	bP.drawPixel( myW - 1 , 1 	   , RGB( 6 , 14 , 19 ) ); // Top Right
	bP.drawPixel( myW - 2 , 0 	   , RGB( 6 , 14 , 19 ) ); // Top Right
	bP.drawPixel( 1 		 , myH - 1 , RGB( 6 , 14 , 19 ) ); // Bottom Left
	bP.drawPixel( 0 		 , myH - 2 , RGB( 6 , 14 , 19 ) ); // Bottom Left
	bP.drawPixel( 1 		 , 0	   , RGB( 6 , 14 , 19 ) ); // Top Left
	bP.drawPixel( 0 		 , 1	   , RGB( 6 , 14 , 19 ) ); // Top Left
	
	bP.drawPixel( 0 , 0 , NO_COLOR );
	bP.drawPixel( myW - 1 , 0 , NO_COLOR );
	bP.drawPixel( myW - 1 , myH - 1 , NO_COLOR );
	bP.drawPixel( 0 , myH - 1 , NO_COLOR );
	
	//
	// TEXT PAINTING!
	//
	
	
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

_callbackReturn _button::mouseHandler( _event event )
{
	// Update button status
	event.getGadget<_button>()->pressed = ( event == onMouseEnter ) ? true : false;
	
	// Refresh
	event.getGadget()->redraw();
	
	return handled;
}

_button::_button( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value , _style&& style ) :
	_gadget( _gadgetType::button , x , y , width , height , style | _styleAttr::notDoubleClickable )
	, autoSelect( false )
	, strValue( value )
	, font( _system::getFont() )
	, fontColor( COLOR_BLACK )
	, fontSize( _system::getRTA().getDefaultFontSize() )
	, pressed( false )
	, align( _align::center )
	, vAlign( _valign::middle )
{
	this->setInternalEventHandler( onUpdate , make_callback( &_button::updateHandler ) );
	
	// force update of size
	this->updateNow();
	
	// Register my handlers
	this->setInternalEventHandler( onDraw , make_callback( &_button::refreshHandler ) );
	this->setInternalEventHandler( onMouseEnter , make_callback( &_button::mouseHandler ) );
	this->setInternalEventHandler( onMouseLeave , make_callback( &_button::mouseHandler ) );
	this->setInternalEventHandler( onMouseRepeat , _gadgetHelpers::eventForward(onMouseClick) );
	this->setInternalEventHandler( onResize , make_callback( &_button::updateHandler ) );
	
	// Refresh
	this->redraw();
}