#include "_gadget/gadget.button.h"
#include "_gadget/gadget.windows.h"
void _button::setAutoSelect( bool aS ){ this->autoSelect = aS; this->bubbleRefresh( true ); }

bool _button::isAutoSelect(){ return this->autoSelect; }

void _button::setFont( _font* ft )
{
	if( this->label )
	{
		this->label->setFont( ft );
		this->computeSize();
		this->bubbleRefresh( true );
	}
}

_gadgetEventReturnType _button::resizeHandler( _gadgetEvent event )
{
	_button* that = event.getGadget<_button>();
	
	if( !that->label )
		return handled;
	
	that->label->setWidth( max( _length(1) , that->getWidth() - 2 ) );
	that->label->setHeight( max( _length(1) , that->getHeight() - 2 ) );
	
	return handled;
}

void _button::computeSize()
{
	if( !this->label || !this->label->font || !this->label->font->valid() )
		return;
	
	// Compute Height
	if( this->computeH == 2 && ( this->computeH = 1 ) )
		this->setHeight( this->label->font->getHeight() + 2 );
	
	// Compute Width
	if( this->computeW == 2 && ( this->computeW = 1 ) )
		this->setWidth( max( 28 , 7 + this->label->font->getStringWidth( this->getTitle() ) ) );
}

_gadgetEventReturnType _button::refreshHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_button* that = event.getGadget<_button>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.resetClippingRects();
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	if( that->pressed )
	{
		// Background
		bP.fill( RGB( 25 , 25 , 25 ) );
		
		// Inner Shadow
		bP.drawHorizontalLine( 1 	, 1 	  , myW - 3 , RGB( 20 , 20 , 20 ) );
		bP.drawHorizontalLine( 2 	, myH - 2 , myW - 4 , RGB( 29 , 29 , 28 ) );
		bP.drawVerticalLine( 1 , 2 	  , myH - 4 , RGB( 20 , 20 , 20 ) );
	}
	else
	{
		// Background
		bP.fill( RGB( 31 , 31 , 31 ) );
		
		if( that->autoSelect )
		{
			// Inner Shadow
			bP.drawRect( 1 	, 1 , myW - 2 , myH - 2 , RGB( 18 , 22 , 31 ) );
			bP.drawRect( 2 	, 2 , myW - 4 , myH - 4 , RGB( 24 , 28 , 31 ) );
		}
		else
		{
			// Inner Shadow
			bP.drawHorizontalLine( 2 	, myH - 2 , myW - 4 , RGB( 25 , 25 , 25 ) );
			bP.drawVerticalLine( myW - 2 	, 2 	  , myH - 4 , RGB( 25 , 25 , 25 ) );
			bP.drawHorizontalLine( 2 	, 1 , myW - 4 , RGB( 29 , 29 , 29 ) );
			bP.drawVerticalLine( 1 	, 2 	  , myH - 4 , RGB( 29 , 29 , 29 ) );
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
	
	return use_default;
}

_gadgetEventReturnType _button::dragHandler( _gadgetEvent event )
{
	// Receive Gadget
	_button* that = event.getGadget<_button>();
	
	if( event.getType() == "dragStart" )
		return handled;
	else if( event.getType() == "dragging" ){
		
		if( !that->getAbsoluteDimensions().contains( event.getPosX() , event.getPosY() ) )
		{
			// I'm not pressed anymore!
			that->pressed = false;
			
			// Refresh my parents
			that->bubbleRefresh( true );
		}
		return handled;
	}
	
	return not_handled;
}

_gadgetEventReturnType _button::mouseHandler( _gadgetEvent event )
{
	// Receive Gadget
	_button* that = event.getGadget<_button>();
	
	if( event.getType() == "mouseDown" )
		that->pressed = true;
	else if( event.getType() == "mouseUp" )
	{
		if( that->pressed )
			that->handleEvent( _gadgetEvent( "listener" ) );
		else
			return handled;
		that->pressed = false;
	}
	
	// Refresh
	that->bubbleRefresh( true );
	
	return handled;
}

void _button::init( string text )
{	
	// Create a Label
	this->label = new _label( this->getWidth() - 2 , this->getHeight() - 2 , 1 , 1 , text );
	//this->label->setBgColor( RGB( 31 , 0 , 0 ) );
	this->label->setAlign( _align::center );
	this->label->setVAlign( _valign::middle );
	
	this->pressed = false;
	
	// Append it to this button
	this->addChild( this->label );
	
	// Register my handler as the default Refresh-Handler
	this->unregisterEventHandler( "mouseDoubleClick" );
	this->registerEventHandler( "refresh" , &_button::refreshHandler );
	this->registerEventHandler( "mouseDown" , &_button::mouseHandler );
	this->registerEventHandler( "mouseUp" , &_button::mouseHandler );
	this->registerEventHandler( "dragStart" , &_button::dragHandler );
	this->registerEventHandler( "dragging" , &_button::dragHandler );
	this->registerEventHandler( "resize" , &_button::resizeHandler );
	
	// Compute the necesary Width
	this->computeSize();
	
	// Refresh Me
	this->refreshBitmap();
}

// Methods to set Size
void _button::setWidth( _u8 width ){ 
	if( width <= 0 ){ this->computeW = 2; this->computeSize(); return; }
	this->computeW = 0; _gadget::setWidth( width );
}
void _button::setDimensions( _rect dim ){ 
	if( !dim.isValid() ){ this->computeH = 2; this->computeW = 2; this->computeSize(); return; }
	this->computeH = 0; this->computeW = 0; _gadget::setDimensions( dim );
}
void _button::setHeight( _u8 height ){
	if( height <= 0 ){ this->computeH = 2; this->computeSize(); return; }
	this->computeH = 0; _gadget::setHeight( height );
}

_button::_button( _length width , _length height , _coord x , _coord y , string text , _gadgetStyle style ) :
	_gadget( _gadgetType::button , width , height , x , y , style )
	, _interface_input( text )
	, autoSelect( false )
	, pressed( false )
	, computeW( 0 )
	, computeH( 0 )
{
	// Link to Constructor
	this->init( text );
}

_button::_button( _coord x , _coord y , string text , _gadgetStyle style ) :
	_gadget( _gadgetType::button , 32 , 9 , x , y , style )
	, _interface_input( text )
	, autoSelect( false )
	, pressed( false )
	, computeW( 2 )
	, computeH( 2 )
{	
	// Link to Constructor
	this->init( text );	
}

_button::~_button(){
	delete this->label;
}