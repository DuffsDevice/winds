#include "_gadget/gadget.label.h"
#include "_gadget/gadget.windows.h"

void _label::setStrValue( string val )
{
	_interface_input::setStrValue( val );
	if( this->autoWidth == 1 ){ 
		this->autoWidth = 2; 
		this->computeSize(); 
	}
	else{
		this->refreshBitmap();
		this->bubbleRealRefresh( true );
	}
}

void _label::setFont( _font* ft )
{
	this->font = ft;
	if( this->autoWidth == 1 ){
		this->autoWidth = 2; 
		this->computeSize(); 
	}
	else{
		this->refreshBitmap();
		this->bubbleRealRefresh( true );
	}
}

void _label::computeSize()
{
	if( this->autoWidth == 2 && this->font->valid() ){
		this->setWidth( this->font->getStringWidth( this->getStrValue() ) );
		this->setHeight( this->font->getHeight() );
		this->autoWidth = 1;
		this->bubbleRealRefresh( true );
	}
}

_gadgetEventReturnType _label::refreshHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_label* that = (_label*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().isBubbleRefresh() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	bP.fill( that->bgColor );
	
	// If there is no font it doesn't make sense to paint
	if( that->font == nullptr || !that->font->valid() )
		return use_default;
	
	_length myH = that->bitmap->getHeight();
	_length myW = that->bitmap->getWidth();
	
	_coord x = 0;
	_coord y = 0;
	
	switch( that->getAlign() ){
		case center:
			x = ( myW >> 1 ) - ( ( that->font->getStringWidth( that->getTitle() ) - 1 ) >> 1 );
			break;
		case left:
		case optimize:
			x = 0;
			break;
		case right:
			x = that->dimensions.width - that->font->getStringWidth( that->getTitle() );
			break;
	}
	
	switch( that->getVAlign() ){
		case middle:
			y = ( ( myH + 1 ) >> 1 ) - ( ( that->font->getMainHeight() + 1 ) >> 1 );
			break;
		case top:
			y = 0;
			break;
		case bottom:
			y = that->dimensions.height - that->font->getMainHeight();
			break;
	}
	
	// Text
	bP.drawString( x , y , that->font , that->getStrValue() , that->color );
	
	return use_default;
}

void _label::init( string text )
{	
	// Set Font...
	this->font = _defaultRuntimeAttributes_.defaultFont;
	
	// Init
	this->setStrValue( text );
	this->color = RGB( 0 , 0 , 0 );
	this->bgColor = NO_COLOR;
	
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( refresh , &_label::refreshHandler );
}

_label::_label( _length width , _length height , _coord x , _coord y , string text , _gadgetStyle style ) :
	_gadget( _gadgetType::label , width , height , x , y , style )
	, _interface_input( text )
	, autoWidth( 0 )
{
	// Link to Constructor
	this->init( text );
	
	// Refresh Me
	this->refreshBitmap();
}

_label::_label( _coord x , _coord y , string text , _gadgetStyle style ) :
	_gadget( _gadgetType::label , 1 , 9 , x , y , style )
	, _interface_input( text )
	, autoWidth( 2 )
{
	// Link to Constructor
	this->init( text );	
	
	// Compute the necesary Width
	this->computeSize();
	
	// Refresh Me
	this->refreshBitmap();
}