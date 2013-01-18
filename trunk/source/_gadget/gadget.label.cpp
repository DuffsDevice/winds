#include "_gadget/gadget.label.h"
#include "_type/type.system.h"

void _label::setStrValue( string val )
{
	// Set Value...
	this->strValue = val;
	
	if( this->computeW == 1 ){
		this->computeW = 2;
		this->computeSize();
	}
	this->bubbleRefresh( true );
}

void _label::setFont( _font* ft )
{
	if( this->font == ft )
		return;
	
	// Set Font...
	this->font = ft;
	if( this->computeW )
		this->computeW = 2;
	if( this->computeH )
		this->computeH = 2;
	this->computeSize();
	this->bubbleRefresh( true );
}

void _label::setFontSize( _u8 fontSize )
{
	if( this->fontSize == fontSize )
		return;
	
	// Set FontSize...
	this->fontSize = fontSize;
	if( this->computeW )
		this->computeW = 2;
	if( this->computeH )
		this->computeH = 2;
	this->computeSize();
	this->bubbleRefresh( true );
}

void _label::computeSize()
{
	if( !this->font || !this->font->valid() )
		return;
	
	// Compute Height
	if( this->computeH == 2 )
	{
		_gadget::setHeight( max( _length( 1 ) , this->font->getHeight( this->fontSize ) ) );
		this->computeH = 1;
	}
	
	// Compute Width
	if( this->computeW == 2 )
	{
		_gadget::setWidth( max( this->font->getStringWidth( this->getStrValue() , this->fontSize ) , _length(1) ) );
		this->computeW = 1;
	}
}

_callbackReturn _label::refreshHandler( _event event )
{
	// Receive Gadget
	_label* that = event.getGadget<_label>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().relativate( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	bP.fill( that->bgColor );
	
	// If there is no font it doesn't make sense to paint
	if( !that->font || !that->font->valid() )
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
			x = that->dimensions.width - that->font->getStringWidth( that->getStrValue() , that->fontSize );
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
			y = that->dimensions.height - that->font->getAscent( that->fontSize );
			break;
	}
	
	// Draw Text...
	bP.drawString( x , y , that->font , that->getStrValue() , that->color , that->fontSize );
	
	return use_default;
}

// Methods to set Size (Pass something not valid to tell the _label to compute the size)
void _label::setWidth( _length width ){
	if( !width ){
		this->computeW = 2;
		this->computeSize();
	} else {
		this->computeW = 0;
		_gadget::setWidth( width );
	}
}
void _label::setDimensions( _rect dim ){ 
	if( !dim.isValid() ){
		this->computeH = 2;
		this->computeW = 2;
		this->computeSize();
	} else {
		this->computeH = 0; 
		this->computeW = 0;
		_gadget::setDimensions( dim );
	}
}
void _label::setHeight( _length height ){
	if( !height ){ 
		this->computeH = 2;
		this->computeSize(); 
	} else {
		this->computeH = 0;
		_gadget::setHeight( height );
	}
}


_label::_label( _length width , _length height , _coord x , _coord y , string text , _style style ) :
	_gadget( _gadgetType::label , width , height , x , y , style )
	, color( RGB( 0 , 0 , 0 ) )
	, bgColor( COLOR_TRANSPARENT )
	, computeW( 0 )
	, computeH( 0 )
	, align( _align::center )
	, vAlign( _valign::middle )
	, strValue( text )
{
	this->font = _system::getFont();
	this->fontSize = _system::_runtimeAttributes_->defaultFontSize;
	
	// Register my handler as the default Refresh-Handler
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( refresh , new _staticCallback( &_label::refreshHandler ) );
	this->registerEventHandler( onResize , new _staticCallback( &_label::refreshHandler ) );
	
	// Refresh
	this->refreshBitmap();
}

_label::_label( _coord x , _coord y , string text , _style style ) :
	_gadget( _gadgetType::label , 1 , 1 , x , y , style )
	, color( RGB( 0 , 0 , 0 ) )
	, bgColor( COLOR_TRANSPARENT )
	, computeW( 2 )
	, computeH( 2 )
	, align( _align::center )
	, vAlign( _valign::middle )
	, strValue( text )
{
	this->font = _system::getFont();
	this->fontSize = _system::_runtimeAttributes_->defaultFontSize;
	
	// Register my handler as the default Refresh-Handler
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( refresh , new _staticCallback( &_label::refreshHandler ) );
	
	// Refresh
	this->computeSize();
	this->refreshBitmap();
}