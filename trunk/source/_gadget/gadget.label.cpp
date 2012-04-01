#include "_gadget/gadget.label.h"
#include "_gadget/gadget.windows.h"

void _label::setStrValue( string val )
{
	// Set Value...
	_interface_input::setStrValue( val );
	this->computeSize();
	this->bubbleRefresh( true );
}

void _label::setFont( _font* ft )
{
	if( this->font == ft )
		return;
	
	// Set Font...
	this->font = ft;
	this->computeSize();
	this->bubbleRefresh( true );
}

void _label::computeSize()
{
	if( !this->font || !this->font->valid() )
		return;
	
	// Compute Height
	if( this->computeH == 2 && ( this->computeH = 1 ) )
		this->setHeight( this->font->getHeight() );
	
	// Compute Width
	if( this->computeW == 2 && ( this->computeW = 1 ) )
		this->setWidth( this->font->getStringWidth( this->getStrValue() ) );
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
	if( !that->font || !that->font->valid() )
		return use_default;
	
	_length myH = that->bitmap->getHeight();
	_length myW = that->bitmap->getWidth();
	
	_coord x = 0;
	_coord y = 0;
	
	switch( that->getAlign() )
	{
		case center:
			x = ( myW >> 1 ) - ( ( that->font->getStringWidth( that->getStrValue() ) - 1 ) >> 1 );
			break;
		case left:
		case optimize:
			x = 0;
			break;
		case right:
			x = that->dimensions.width - that->font->getStringWidth( that->getStrValue() );
			break;
	}
	
	switch( that->getVAlign() )
	{
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
	
	// Draw Text...
	bP.drawString( x , y , that->font , that->getStrValue() , that->color );
	
	return use_default;
}

// Methods to set Size
void _label::setWidth( _u8 width )		{ this->computeW = 0; _gadget::setWidth( width ); }
void _label::setDimensions( _rect dim )	{ this->computeH = 0; this->computeW = 0; _gadget::setDimensions( dim ); }
void _label::setHeight( _u8 height )	{ this->computeH = 0; _gadget::setHeight( height ); }

// Methods to tell: We want it to compute the Size on its own
void _label::setWidth()					{ this->computeW = 2; this->computeSize(); }
void _label::setDimensions()			{ this->computeH = 0; this->computeW = 0; this->computeSize(); }
void _label::setHeight()				{ this->computeH = 2; this->computeSize(); }


_label::_label( _length width , _length height , _coord x , _coord y , string text , _gadgetStyle style ) :
	_gadget( _gadgetType::label , width , height , x , y , style )
	, color( RGB( 0 , 0 , 0 ) )
	, bgColor( NO_COLOR )
	, computeW( 0 )
	, computeH( 0 )
{
	this->font = _defaultRuntimeAttributes_.defaultFont;
	
	_interface_input::setStrValue( text );
	
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( refresh , &_label::refreshHandler );
	
	// Refresh
	this->refreshBitmap();
}

_label::_label( _coord x , _coord y , string text , _gadgetStyle style ) :
	_gadget( _gadgetType::label , 1 , 1 , x , y , style )
	, color( RGB( 0 , 0 , 0 ) )
	, bgColor( NO_COLOR )
	, computeW( 2 )
	, computeH( 2 )
{
	this->font = _defaultRuntimeAttributes_.defaultFont;
	
	_interface_input::setStrValue( text );
	
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( refresh , &_label::refreshHandler );
	
	// Refresh
	this->computeSize();
	this->refreshBitmap();
}