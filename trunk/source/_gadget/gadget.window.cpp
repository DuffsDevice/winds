#include "_gadget/gadget.window.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.system.h"
#include "_type/type.runtimeAttributes.h"

_callbackReturn _windowButton::refreshHandler( _event event )
{
	// Get Source
	_windowButton* that = event.getGadget<_windowButton>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().relativate( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	_pixel color[] = { 
		RGB255( 114 , 154 , 250 ) , RGB255( 5 , 88 , 226 ) , RGB255( 75 , 126 , 245 ) , // Blue Values
		RGB255( 227 , 92 , 59 ) , RGB255( 234 , 131 , 106 ) , RGB255( 250 , 221 , 211 ) , // Red Values
		RGB255( 186 , 61 , 239 ) // Violet (Blue =/= Red)
	};
	
	bP.fill( NO_COLOR );
	
	switch( that->buttonType ){
		case 0:
			// Close
			bP.drawRect( 0 , 0 , 8 , 8 , color[3] );
			bP.drawRect( 1 , 1 , 6 , 6 , color[4] );
			bP.drawRect( 2 , 2 , 4 , 4 , color[4] );
			bP.drawFilledRect( 3 , 3 , 2 , 2 , color[5] );
			bP.drawPixel( 2 , 2 , color[5] );
			bP.drawPixel( 2 , 5 , color[5] );
			bP.drawPixel( 5 , 2 , color[5] );
			bP.drawPixel( 5 , 5 , color[5] );
			break;
		case 1:
			// Maximize/Restore
			bP.drawRect( 0 , 0 , 8 , 8 , color[0] ); // White Border
			
			if( that->getParent()->isMaximized() )
			{
				bP.drawRect( 3 , 2 , 3 , 3 , color[0] ); // Semi-White Line
				bP.drawRect( 2 , 4 , 2 , 2 , color[0] ); // Semi-White Line
				bP.drawHorizontalLine( 3 , 2 , 3 , COLOR_WHITE ); // White Topper Line
				bP.drawHorizontalLine( 2 , 4 , 2 , COLOR_WHITE ); // White Topper Line
			}
			else
			{
				//bP.drawRect( 1 , 1 , 6 , 6 , color[1] );
				//bP.drawRect( 3 , 3 , 2 , 2 , color[1] );
				bP.drawRect( 2 , 2 , 4 , 4 , color[0] ); // Semi-White Line
				bP.drawHorizontalLine( 2 , 2 , 4 , COLOR_WHITE ); // White Topper Line
			}
			break;
		case 2:
			// Minimize
			bP.drawRect( 0 , 0 , 8 , 8 , color[0] );
			//bP.drawFilledRect( 1 , 1 , 6 , 6 , color[1] );
			bP.drawHorizontalLine( 2 , 5 , 3 , RGB( 31 , 31 , 31 ) );
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
	// Reset Bitmap
	this->bitmap.reset( NO_COLOR );
	
	this->registerEventHandler( refresh , new _staticCallback( &_windowButton::refreshHandler ) );
	this->refreshBitmap();
}

////////////////////////////////////////////
_callbackReturn _window::resizeHandler( _event event )
{
	_window* that = event.getGadget<_window>();
	
	int lblWidth = that->getWidth();
	
	if( that->isDestroyable() )
	{
		that->button[0]->setX( that->getWidth() - 10 );
		lblWidth -= 10;
	}
	
	if( that->isResizeable() )
	{
		that->button[1]->setX( that->getWidth() - ( that->isDestroyable() ? 9 : 0 ) - 10 );
		lblWidth -= 10;
	}
	
	if( that->isMinimizeable() )
	{
		that->button[2]->setX( that->getWidth() - ( that->isDestroyable() ? 9 : 0 ) - ( that->isResizeable() ? 9 : 0 ) - 10 );
		lblWidth -= 10;
	}
	
	that->label->setWidth( lblWidth - ( that->icon->getImage().isValid() ? 10 : 0 ) - 3 );
	that->label->setX( that->icon->getImage().isValid() ? 10 : 2 );
	
	return handled;
}

_callbackReturn _window::restyleHandler( _event event )
{
	_window* that = event.getGadget<_window>();
	
	// Toggle Window->Buttons (Close, Minimize, Restore etc...)
	if( that->isDestroyable() )
		that->button[0]->show();
	else
		that->button[0]->hide();
	
	if( that->isResizeable() )
		that->button[1]->show();
	else
		that->button[1]->hide();
	
	if( that->icon->getImage().isValid() )
		that->icon->show();
	else
		that->icon->hide();
	
	if( that->isMinimizeable() )
	{
		if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
			((_windows*)_system::_gadgetHost_)->registerTask( that );
		that->button[2]->show();
	}
	else
	{
		if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
			((_windows*)_system::_gadgetHost_)->removeTask( that );
		that->button[2]->hide();
	}
	
	that->handleEvent( onResize );
	
	return handled;
}

void _window::setStrValue( string title )
{		
	this->label->setStrValue( title );
	
	// Refresh Task Button
	if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
		((_windows*)_system::_gadgetHost_)->refreshTask( this );
}


void _window::setIcon( _bitmap bmp )
{
	bmp.resize( 6 , 6 ); // resize to 6x6
	
	this->icon->setImage( bmp );
	this->triggerEvent( onStyleSet );
	
	// Refresh Task Button
	if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
		((_windows*)_system::_gadgetHost_)->refreshTask( this );
}

_callbackReturn _window::refreshHandler( _event event )
{
	// Get Source
	_window* that = (_window*)event.getGadget();
	
	if( event.getType() == onBlur || event.getType() == onFocus )
	{
		that->bubbleRefresh( true );
		
		// Refresh Button
		if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
			((_windows*)_system::_gadgetHost_)->refreshTask( that );
		
		return handled;
	}
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().relativate( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	bP.fill( RGB( 30 , 30 , 29 ) );
	
	if( that->hasFocus() )
	{
		// Window-Bar
		bP.copyHorizontalStretch( 0 , 0 , that->getWidth() , _system::_runtimeAttributes_->windowBar );
		
		// Bottom Border
		bP.drawVerticalLine( 0 , 1 , that->getHeight() - 1 , _system::_runtimeAttributes_->windowBar[0] );
		bP.drawVerticalLine( that->getWidth() - 1 , 1 , that->getHeight() - 1 , _system::_runtimeAttributes_->windowBar[9] );
		bP.drawHorizontalLine( 0 , that->getHeight() - 1 , that->getWidth() , _system::_runtimeAttributes_->windowBar[9] );
	}
	else
	{
		// Window-Bar
		bP.copyHorizontalStretch( 0 , 0 , that->getWidth() , _system::_runtimeAttributes_->windowBarBlurred );
		
		// Bottom Border
		bP.drawVerticalLine( 0 , 1 , that->getHeight() - 1 , _system::_runtimeAttributes_->windowBarBlurred[0] );
		bP.drawVerticalLine( that->getWidth() - 1 , 1 , that->getHeight() - 1 , _system::_runtimeAttributes_->windowBarBlurred[9] );
		bP.drawHorizontalLine( 0 , that->getHeight() - 1 , that->getWidth() , _system::_runtimeAttributes_->windowBarBlurred[9] );
	}
	
	that->label->refreshBitmap();
	
	// Set my Corners to transparent
	if( !that->isMaximized() )
	{
		bP.drawPixel( 0 , 0 , NO_COLOR );
		bP.drawPixel( that->getWidth() - 1 , 0 , NO_COLOR );
		bP.drawPixel( bP.getWidth() - 1 , that->getHeight() - 1 , NO_COLOR );
		bP.drawPixel( 0 , that->getHeight() - 1 , NO_COLOR );
	}
	
	return use_default;
}

_callbackReturn _window::dragHandler( _event event )
{	
	// Get Source
	_window* that = (_window*)event.getGadget();
	
	if( event.getType() == dragStart )
	{
		// If y pos is not on the windowbar, let my children gagdet be the subject of Dragment :-)
		if( event.getPosY() > 9 ){
			that->dragMe = false;
			return use_default;
		}
		
		that->dragMe = true;
		
		// If y is on the windowbar, drag Me!
		if( that->isMaximized() )
			return not_handled; // Don't try to drag a window while its maximized
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
		if( event.getDeltaX() == 0 && event.getDeltaY() == 0 )
			return handled;
		
		// Move it relatively
		that->moveRelative( event.getDeltaX() , event.getDeltaY() );
		
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

_callbackReturn _window::buttonHandler( _event event )
{
	// Close
	if( event.getGadget<_windowButton>() == this->button[0] )
	{
		// Get Source
		_callbackReturn cr = this->handleEvent( onClose );
		
		// Close the window
		if( cr == not_handled || cr == use_default )
			this->setParent( nullptr );
		
		if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
			((_windows*)_system::_gadgetHost_)->removeTask( this );
	}
	// maximize or restore
	else if( event.getGadget<_windowButton>() == this->button[1] )
	{
		if( this->isMaximized() )
			this->unMaximize();
		else
			this->maximize();
	}
	// Minimize
	if( event.getGadget<_windowButton>() == this->button[2] )
	{
		this->minimize();
		if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
			((_windows*)_system::_gadgetHost_)->refreshTask( this );
	}
	
	return handled;
}

// C++0x! Yay!
_window::_window( _length width , _length height , _coord x , _coord y , string title , _style style ) :
	_window( width , height , x , y , title , _bitmap() , style )
{ }

_window::_window( _length width , _length height , _coord x , _coord y , string title , _bitmap bmp , _style style ) :
	_gadget( _gadgetType::window , width , height , x , y , style )
{
	this->setPadding( _padding( 1 , 10 , 1 , 1 ) );
	
	// Create a Label
	this->label = new _label( this->getWidth() - 2 , 6 , 2 , 2 , title );
	this->label->setAlign( _align::left );
	this->label->setVAlign( _valign::middle );
	this->label->setColor( RGB( 31 , 31 , 31 ) );
	
	// Create Icon
	bmp.resize( 6 , 6 ); // Crop to 5x5
	this->icon = new _imagegadget( 2 , 2 , bmp );
	
	// Append it to this button
	this->addEnhancedChild( this->label );
	this->addEnhancedChild( this->icon );
	
	this->button[0] = new _windowButton( this->getWidth() - 10 , 1 , 0 );
	this->button[1] = new _windowButton( this->getWidth() - 19 , 1 , 1 );
	this->button[2] = new _windowButton( this->getWidth() - 28 , 1 , 2 );
	
	// Register close-handler
	this->button[0]->registerEventHandler( onAction , new _classCallback( this , &_window::buttonHandler ) );
	this->button[1]->registerEventHandler( onAction , new _classCallback( this , &_window::buttonHandler ) );
	this->button[2]->registerEventHandler( onAction , new _classCallback( this , &_window::buttonHandler ) );
	
	this->button[0]->enhanceToParent( this );
	this->button[1]->enhanceToParent( this );
	this->button[2]->enhanceToParent( this );
	
	// Register my handler as the default Refresh-Handler
	this->registerEventHandler( refresh , new _staticCallback( &_window::refreshHandler ) );
	this->registerEventHandler( onBlur , new _staticCallback( &_window::refreshHandler ) );
	this->registerEventHandler( onFocus , new _staticCallback( &_window::refreshHandler ) );
	this->registerEventHandler( dragging , new _staticCallback( &_window::dragHandler ) );
	this->registerEventHandler( dragStart , new _staticCallback( &_window::dragHandler ) );
	this->registerEventHandler( dragStop , new _staticCallback( &_window::dragHandler ) );
	this->registerEventHandler( onStyleSet , new _staticCallback( &_window::restyleHandler ) );
	this->registerEventHandler( onResize , new _staticCallback( &_window::resizeHandler ) );
	
	// Refresh Window-Buttons
	this->triggerEvent( onStyleSet );
	
	// Refresh Me
	this->refreshBitmap();
}

_window::~_window()
{
	delete this->button[0];
	delete this->button[1];
	delete this->button[2];
	delete this->label;
}