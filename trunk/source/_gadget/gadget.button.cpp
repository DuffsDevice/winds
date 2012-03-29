#include "_gadget/gadget.button.h"
#include "_gadget/gadget.windows.h"

void _button::setAutoSelect( bool aS ){ this->autoSelect = aS; this->handleEvent( refresh ); }

bool _button::isAutoSelect(){ return this->autoSelect; }

void _button::setFont( _font* ft ){
	if( this->label ) 
		this->label->setFont( ft );
	if( this->autoWidth == 1 ){
		this->autoWidth = 2;
		this->computeSize(); 
	} else{ 
		this->handleEvent( refresh );
	}
}

void _button::onResize()
{
	if( this->label != nullptr ){ 
		this->label->setWidth( max( 1 , this->getWidth() - 4 ) );
		this->label->setHeight( max( 1 , this->getHeight() - 4 ) );
	}
	this->handleEvent( refresh );
}

void _button::computeSize(){
	if( this->label != nullptr && this->autoWidth == 2 && this->label->font && this->label->font->valid() )
	{
		this->setWidth( max( 32 , 7 + this->label->font->getStringWidth( this->getTitle() ) ) );
		this->autoWidth = 1;
		// Refresh Me
		this->handleEvent( refresh );
	}
}

_gadgetEventReturnType _button::refreshHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_button* that = (_button*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().isBubbleRefresh() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
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
	_button* that = (_button*)event.getGadget();
	
	if( event.getType() == dragStart )
		return handled;
	else if( event.getType() == dragging ){
		
		if( !that->getAbsoluteDimensions().contains( event.getArgs().getPosX() , event.getArgs().getPosY() ) )
		{
			// I'm not pressed anymore!
			that->pressed = false;
			
			// Refresh my parents
			that->handleEvent( refresh );
		}
		return handled;
	}
	
	return not_handled;
}

_gadgetEventReturnType _button::mouseHandler( _gadgetEvent event )
{
	// Receive Gadget
	_button* that = (_button*)event.getGadget();
	
	switch( event.getType() )
	{
		case mouseDown:
			that->pressed = true;
			break;
		case mouseUp:
			if( that->pressed )
				that->handleEvent( individual );
			that->pressed = false;
			break;
		default:
			break;
	}
	
	// Refresh
	that->handleEvent( refresh );
	
	return handled;
}

_button::_button( _length width , _length height , _coord x , _coord y , string text , _gadgetStyle style ) :
	_gadget( _gadgetType::button , width , height , x , y , style )
	, _interface_input( text )
	, autoSelect( false )
	, autoWidth( 0 )
{
	// Link to Constructor
	this->init( text );
	
	// Refresh Me
	this->refreshBitmap();
}

void _button::init( string text )
{	
	// Create a Label
	this->label = new _label( this->getWidth() - 4 , this->getHeight() - 4 , 2 , 2 , text );
	this->label->setAlign( center );
	this->label->setVAlign( middle );
	
	this->pressed = false;
	
	// Append it to this button
	this->addChild( this->label );	
	
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( refresh , &_button::refreshHandler );
	this->registerEventHandler( mouseDown , &_button::mouseHandler );
	this->registerEventHandler( mouseUp , &_button::mouseHandler );
	this->registerEventHandler( dragStart , &_button::dragHandler );
	this->registerEventHandler( dragging , &_button::dragHandler );
}

_button::_button( _coord x , _coord y , string text , _gadgetStyle style ) :
	_gadget( _gadgetType::button , 1 , 9 , x , y , style )
	, _interface_input( text )
	, autoSelect( false )
	, autoWidth( 2 )
{	
	// Link to Constructor
	this->init( text );	
	
	// Compute the necesary Width
	this->computeSize();
}

_button::~_button(){
	delete this->label;
}