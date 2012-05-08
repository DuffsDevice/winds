#include "_gadget/gadget.window.h"
#include "_type/type.system.h"

_gadgetEventReturnType _windowButton::refreshHandler( _gadgetEvent event )
{
	// Get Source
	_windowButton* that = (_windowButton*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	_pixel color[] = { 
		RGB255( 114 , 154 , 250 ) , RGB255( 5 , 88 , 226 ) , RGB255( 75 , 126 , 245 ) , // Blue Values
		RGB255( 227 , 92 , 59 ) , RGB255( 234 , 131 , 106 ) , RGB255( 250 , 221 , 211 ) , // Red Values
		RGB255( 186 , 61 , 239 ) // Violet (Blue =/= Red)
	};
	
	switch( that->buttonType ){
		case 0:
			bP.drawRect( 0 , 0 , 8 , 8 , color[3] );
			bP.drawRect( 1 , 1 , 6 , 6 , color[4] );
			bP.drawRect( 2 , 2 , 4 , 4 , color[4] );
			bP.drawFilledRect( 3 , 3 , 2 , 2 , color[5] );
			bP.drawPixel( 2 , 2 , color[5] );
			bP.drawPixel( 2 , 5 , color[5] );
			bP.drawPixel( 5 , 2 , color[5] );
			bP.drawPixel( 5 , 5 , color[5] );
			//printf("CLOSE\n");
			break;
		case 1:
			bP.drawRect( 0 , 0 , 8 , 8 , color[0] );
			bP.drawRect( 1 , 1 , 6 , 6 , color[1] );
			bP.drawRect( 3 , 3 , 2 , 2 , color[1] );
			bP.drawRect( 2 , 2 , 4 , 4 , color[0] );
			bP.drawHorizontalLine( 2 , 2 , 4 , RGB( 31 , 31 , 31 ) );
			//printf("MAX\n");
			break;
		case 2:
			bP.drawRect( 0 , 0 , 8 , 8 , color[0] );
			bP.drawFilledRect( 1 , 1 , 6 , 6 , color[1] );
			bP.drawHorizontalLine( 2 , 5 , 3 , RGB( 31 , 31 , 31 ) );
			//printf("MIN\n");
			break;
	}
	
	if( that->buttonType ){
		bP.drawPixel( 0 , 0 , color[2] );
		bP.drawPixel( 7 , 0 , color[2] );
		bP.drawPixel( 7 , 7 , color[2] );
		bP.drawPixel( 0 , 7 , color[2] );
	}
	else{
		bP.drawPixel( 0 , 0 , color[6] );
		bP.drawPixel( 7 , 0 , color[6] );
		bP.drawPixel( 7 , 7 , color[6] );
		bP.drawPixel( 0 , 7 , color[6] );
	}
	
	return use_default;
}
_windowButton::_windowButton( _coord x , _coord y , _u8 buttonType ) :
	_button( 8 , 8 , x , y , "" )
	, buttonType( buttonType )
{
	this->registerEventHandler( refresh , &_windowButton::refreshHandler );
	this->refreshBitmap();
}

////////////////////////////////////////////
void _window::onResize(){
	if( this->label != nullptr ){
		this->label->setWidth( this->getWidth() - 30 ); 
	}
}

_gadgetEventReturnType _window::refreshHandler( _gadgetEvent event )
{
	// Get Source
	_window* that = (_window*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	bP.fill( RGB( 30 , 30 , 29 ) );
	
	// Window-Bar
	bP.copyHorizontalStretch( 1 , 0 , that->getWidth() - 2 , _system_->_runtimeAttributes_->windowBar );
	
	// Bottom Border
	bP.drawVerticalLine( 0 , 1 , that->getHeight() - 1 , (*_system_->_runtimeAttributes_->windowBar)[0] );
	bP.drawVerticalLine( that->getWidth() - 1 , 1 , that->getHeight() - 1 , (*_system_->_runtimeAttributes_->windowBar)[9] );
	bP.drawHorizontalLine( 1 , that->getHeight() - 1 , that->getWidth() - 2 , (*_system_->_runtimeAttributes_->windowBar)[9] );
	
	that->label->refreshBitmap();
	
	// Set my Corners to transparent
	bP.drawPixel( 0 , 0 , NO_COLOR );
	bP.drawPixel( that->getWidth() - 1 , 0 , NO_COLOR );
	bP.drawPixel( bP.getWidth() - 1 , that->getHeight() - 1 , NO_COLOR );
	bP.drawPixel( 0 , that->getHeight() - 1 , NO_COLOR );
	
	return use_default;
}

_gadgetEventReturnType _window::dragHandler( _gadgetEvent event )
{	
	// Get Source
	_window* that = (_window*)event.getGadget();
	
	if( event.getType() == dragStart )
	{
		// If y pos is not on the windowbar, let my children gagdet be the subject of Dragment :-)
		if( event.getArgs().getPosY() > 9 ){
			that->dragMe = false;
			return use_default;
		}
		
		that->dragMe = true;
		
		// If y is on the windowbar, drag Me!
		return handled;
	}
	else if( event.getType() == dragging )
	{
		// Check if there is a gadget who receives drag-events,
		// If not, it has to be me who's dragged
		if( !that->dragMe )
			return use_default;
		
		/**
		 * Handling of my 'dragment' !
		**/
		
		// Has the Gadget to move?
		if( event.getArgs().getDeltaX() == 0 && event.getArgs().getDeltaY() == 0 )
			return handled;
		
		// Move it relatively
		that->moveRelative( event.getArgs().getDeltaX() , event.getArgs().getDeltaY() );
		
		// Return
		return handled;
	}
	else if( event.getType() == dragStop )
	{
		// Check if there is a gadget who receives drag-events,
		// If not, it has to be me who's dragged
		if( !that->dragMe )
			return use_default;
		
		// Return
		return handled;
	}
	
	// Default return
	return not_handled;
}

_gadgetEventReturnType _window::closeHandler( _gadgetEvent event )
{	
	// Get Source
	_windowButton* that = (_windowButton*)event.getGadget();
	
	that->getParent()->setParent( nullptr ); 
	
	return handled;
}

_window::_window( _length width , _length height , _coord x , _coord y , string title , _gadgetStyle style ) :
	_gadget( _gadgetType::window , width , height , x , y , style ),
	_interface_input( title )
{
	this->setPadding( _padding( 1 , 10 , 1 , 1 ) );
	
	// Create a Label
	this->label = new _label( this->getWidth() - 30 , 6 , 2 , 2 , title );
	this->label->setEnhanced();
	this->label->setAlign( _align::left );
	this->label->setVAlign( _valign::middle );
	this->label->setColor( RGB( 31 , 31 , 31 ) );
	
	// Append it to this button
	this->addChild( this->label );
	
	this->button[0] = new _windowButton( this->getWidth() - 10 , 1 , 0 );
	this->button[1] = new _windowButton( this->getWidth() - 19 , 1 , 1 );
	this->button[2] = new _windowButton( this->getWidth() - 28 , 1 , 2 );
	this->button[0]->setEnhanced();
	this->button[1]->setEnhanced();
	this->button[2]->setEnhanced();
	this->addChild( this->button[0] );
	this->addChild( this->button[1] );
	this->addChild( this->button[2] );
	this->button[0]->registerEventHandler( mouseClick , &_window::closeHandler );
	
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( refresh , &_window::refreshHandler );
	this->registerEventHandler( dragging , &_window::dragHandler );
	this->registerEventHandler( dragStart , &_window::dragHandler );
	this->registerEventHandler( dragStop , &_window::dragHandler );
	
	// Refresh Me
	this->refreshBitmap();
	
}