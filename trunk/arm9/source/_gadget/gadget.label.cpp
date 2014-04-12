#include "_gadget/gadget.label.h"
#include "_type/type.system.h"

void _label::setStrValue( string val )
{
	if( val == this->strValue )
		return;
	
	// Set Value...
	this->strValue = val;
	
	// Inform that maybe, size should change now
	this->update();
	
	// And refresh
	this->redraw();
}

void _label::setFont( _fontPtr ft )
{
	if( this->font == ft || !ft )
		return;
	
	// Set Font...
	this->font = ft;
	
	// Inform that maybe, size should change now
	this->update();
	
	// And refresh
	this->redraw();
}

void _label::setFontSize( _u8 fontSize )
{
	if( this->fontSize == fontSize )
		return;
	
	// Set FontSize...
	this->fontSize = fontSize;
	
	// Inform that maybe, size should change now
	this->update();
	
	// And refresh
	this->redraw();
}

_callbackReturn _label::updateHandler( _event event )
{
	// Receive Gadget
	_label* that = event.getGadget<_label>();
	
	if( !that->font || !that->font->isValid() )
		return handled;
	
	that->setSizeIfAuto(
		that->font->getStringWidth( that->getStrValue() , that->fontSize )
		, that->font->getHeight( that->fontSize )
	);
	
	return handled;
}

_callbackReturn _label::refreshHandler( _event event )
{
	// Receive Gadget
	_label* that = event.getGadget<_label>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( that->bgColor );
	
	// If there is no font it doesn't make sense to paint
	if( !that->font || !that->font->isValid() )
		return use_default;
	
	_length myH = that->getHeight();
	_length myW = that->getWidth();
	
	_coord x = 0;
	_coord y = 0;
	
	switch( that->getAlign() )
	{
		case _align::center:
			x = ( myW >> 1 ) - ( ( that->font->getStringWidth( that->getStrValue() , that->fontSize ) - 1 ) >> 1 );
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
			y = ( ( myH - 1 ) >> 1 ) - ( ( that->font->getAscent( that->fontSize ) + 1 ) >> 1 );
			break;
		case _valign::top:
			y = 0;
			break;
		case _valign::bottom:
			y = that->getHeight() - that->font->getAscent( that->fontSize );
			break;
	}
	
	// Draw Text...
	bP.drawString( x , y , that->font , that->getStrValue() , that->color , that->fontSize );
	
	return use_default;
}


_label::_label( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value , _style&& style ) :
	_gadget( _gadgetType::label , x , y , width , height , (_style&&)style )
	, color( _color::black )
	, bgColor( _color::transparent )
	, font( _system::getFont() )
	, fontSize( _system::getRTA().getDefaultFontSize() )
	, align( _align::left )
	, vAlign( _valign::middle )
	, strValue( value )
{
	this->setInternalEventHandler( onUpdate , make_callback( &_label::updateHandler ) );
	
	// force an update of the size
	this->updateNow();
	
	// Register my handler as the default Refresh-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_label::refreshHandler ) );
	this->setInternalEventHandler( onResize , make_callback( &_label::updateHandler ) );
	
	// Refresh and 
	this->redraw();
}