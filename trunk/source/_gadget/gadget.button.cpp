#include "_gadget/gadget.button.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.system.h"

void _button::setAutoSelect( bool aS ){ this->autoSelect = aS; this->bubbleRefresh( true ); }
bool _button::isAutoSelect(){ return this->autoSelect; }

void _button::setStrValue( string val )
{
	if( this->strValue == val )
		return;
	
	// Set Value...
	this->strValue = val;
	
	if( this->computeW == 1 ){
		this->computeW = 2;
		this->computeSize();
	}
	this->bubbleRefresh( true );
}

void _button::setFont( _font* ft )
{
	if( this->font == ft )
		return;
	
	this->font = ft;
	
	this->computeSize();
	this->bubbleRefresh( true );
}

void _button::setFontSize( _u8 fontSize )
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

void _button::computeSize()
{
	if( !this->font->valid() )
		return;
	
	// Compute Height
	if( this->computeH == 2 && ( this->computeH = 1 ) )
		this->setHeight( this->font->getHeight() + 2 );
	
	// Compute Width
	if( this->computeW == 2 && ( this->computeW = 1 ) )
		this->setWidth( max( 28 , 7 + this->font->getStringWidth( this->getStrValue() ) ) );
}

_callbackReturn _button::refreshHandler( _event event )
{
	// Receive Gadget
	_button* that = event.getGadget<_button>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	if( that->isPressed() )
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
	if( !that->font || !that->font->valid() )
		return use_default;
	
	_coord x = 0;
	_coord y = 0;
	
	switch( that->getAlign() )
	{
		case _align::center:
			x = ( myW - that->font->getStringWidth( that->getStrValue() , that->fontSize ) ) >> 1;
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
			y = ( ( myH - that->font->getAscent( that->fontSize ) - 2 ) >> 1 );
			break;
		case _valign::top:
			y = 0;
			break;
		case _valign::bottom:
			y = that->dimensions.height - that->font->getAscent( that->fontSize );
			break;
	}
	
	// Draw Text...
	bP.drawString( x , y , that->font , that->getStrValue() , that->fontColor , that->fontSize );
	
	return use_default;
}

void _button::init( string text )
{
	this->style.doubleClickable = false;
	
	// Font
	this->strValue = text;
	this->font = _system::getFont();
	this->fontSize = _system::_runtimeAttributes_->defaultFontSize;
	this->fontColor = COLOR_BLACK;
	
	this->vAlign = _valign::middle;
	this->align = _align::center;
	
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( refresh , new _staticCallback( &_button::refreshHandler ) );
	this->registerEventHandler( onMouseEnter , new _gadget::eventForwardRefresh() );
	this->registerEventHandler( onMouseLeave , new _gadget::eventForwardRefresh() );
	this->registerEventHandler( mouseClick , new _gadget::eventForward<onAction>() );
	this->registerEventHandler( mouseRepeat , new _gadget::eventForward<onAction>() );
	
	// Compute the necesary Width
	this->computeSize();
	
	// Refresh Me
	this->refreshBitmap();
}

// Methods to set Size
void _button::setWidth( _length width ){ 
	if( !width ){ this->computeW = 2; this->computeSize(); return; }
	this->computeW = 0; _gadget::setWidth( width );
}
void _button::setDimensions( _rect dim ){
	if( !dim.isValid() ){ this->computeH = 2; this->computeW = 2; this->computeSize(); return; }
	this->computeH = 0; this->computeW = 0; _gadget::setDimensions( dim );
}
void _button::setHeight( _length height ){
	if( !height ){ this->computeH = 2; this->computeSize(); return; }
	this->computeH = 0; _gadget::setHeight( height );
}

_button::_button( _length width , _length height , _coord x , _coord y , string text , _style style ) :
	_gadget( _gadgetType::button , width , height , x , y , style )
	, autoSelect( false )
	, computeW( 0 )
	, computeH( 0 )
{
	// Link to Constructor
	this->init( text );
}

_button::_button( _coord x , _coord y , string text , _style style ) :
	_gadget( _gadgetType::button , 32 , 9 , x , y , style )
	, autoSelect( false )
	, computeW( 2 )
	, computeH( 2 )
{	
	// Link to Constructor
	this->init( text );	
}