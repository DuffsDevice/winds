#include "_gadget/gadget.window.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.system.h"
#include "_type/type.runtimeAttributes.h"

void _window::maximize()
{
	if( this->isMaximized() || !this->isResizeable() )
		return;
	
	_gadgetScreen* screen = this->getScreen();
	
	// Fail
	if( screen == nullptr )
		return;
	
	// Fetch maximized Dimensions from the _screen
	_rect maxDim = screen->getMaximizedDimensions();
	
	if( maxDim.isValid() )
	{
		this->normalDimensions = new _rect( this->getDimensions() );
		
		// Maximizing
		this->setDimensions( maxDim );
		this->setStyle( this->getStyle() | _styleAttr::notResizeable );
		this->triggerEvent( onMaximize );
	}
}

void _window::restore()
{
	if( this->minimized )
	{
		this->minimized = false;
		this->show();
		this->triggerEvent( onRestore );
	}
}

void _window::unMaximize()
{
	if( this->isMaximized() )
	{
		_rect dims = *this->normalDimensions;
		delete this->normalDimensions;
		this->normalDimensions = nullptr;
		
		// Set back the old dimensions
		this->setStyle( this->getStyle() | _styleAttr::resizeable );
		this->setDimensions( dims );
		this->triggerEvent( onUnMaximize );
	}
}

void _window::minimize()
{
	if( !this->minimized && this->minimizeable )
	{
		// Blur
		this->blur();
		
		this->minimized = true;
		this->hide();
		this->triggerEvent( onMinimize );
	}
}

_callbackReturn _window::updateHandler( _event event )
{
	_window* that = event.getGadget<_window>();
	
	if( event == onMaximize || event == onUnMaximize )
	{
		that->button[1]->redraw();
		return handled;
	}
	
	int btnX = that->getWidth();
	int lblWidth = btnX - 3;
	
	// Toggle Window->Buttons (Close, Minimize, Restore etc...)
	if( that->isCloseable() )
	{
		lblWidth -= 10;
		btnX -= 9;
		that->button[0]->setX( btnX - 1 );
		that->button[0]->show();
	}
	else
		that->button[0]->hide();
	
	if( that->isResizeable() || that->isMaximized() )
	{
		lblWidth -= 10;
		btnX -= 9;
		that->button[1]->setX( btnX - 1 );
		that->button[1]->show();
	}
	else
		that->button[1]->hide();
	
	if( that->icon->getImage().isValid() )
	{
		lblWidth -= 10;
		that->icon->show();
		that->label->setX( 10 );
	}
	else
	{
		that->icon->hide();
		that->label->setX( 2 );
	}
	
	if( that->isMinimizeable() )
	{
		lblWidth -= 10;
		that->button[2]->setX( btnX - 10 );
		that->button[2]->show();
		
		// Add to tasklist
		if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
			((_windows*)_system::_gadgetHost_)->registerTask( that );
	}
	else
	{
		that->button[2]->hide();
		
		// Remove from tasklist
		if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
			((_windows*)_system::_gadgetHost_)->removeTask( that );
	}
	
	// Set label width
	that->label->setWidth( lblWidth );
	
	return handled;
}

void _window::setStrValue( string title )
{
	this->label->setStrValue( title );
	
	// Refresh Task Button
	if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
		((_windows*)_system::_gadgetHost_)->refreshTask( this );
}

void _window::setIcon( const _bitmap& bmp )
{
	this->icon->setImage( bmp );
	
	this->update();
	
	// Refresh Task Button
	if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
		((_windows*)_system::_gadgetHost_)->refreshTask( this );
}

_callbackReturn _window::focusHandler( _event event )
{
	// Get Source
	_window* that = (_window*)event.getGadget();
	
	// Refresh
	that->redraw();
	
	// Refresh Button
	if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
		((_windows*)_system::_gadgetHost_)->refreshTask( that );
	
	return handled;
}

_callbackReturn _window::refreshHandler( _event event )
{
	// Get Source
	_window* that = (_window*)event.getGadget();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( _system::getRTA().getControlBackground() );
	
	if( that->hasFocus() )
	{
		_constBitmap& design = _system::getRTA().getWindowsDesignActive();
		
		// Window-Bar
		bP.copyHorizontalStretch( 0 , 0 , that->getWidth() , design );
		
		// Bottom Border
		bP.drawVerticalLine( 0 , 1 , that->getHeight() - 1 , design[0] );
		bP.drawVerticalLine( that->getWidth() - 1 , 1 , that->getHeight() - 1 , design[9] );
		bP.drawHorizontalLine( 0 , that->getHeight() - 1 , that->getWidth() , design[9] );
	}
	else
	{
		_constBitmap& design = _system::getRTA().getWindowsDesignBlurred();
		
		// Window-Bar
		bP.copyHorizontalStretch( 0 , 0 , that->getWidth() , design );
		
		// Bottom Border
		bP.drawVerticalLine( 0 , 1 , that->getHeight() - 1 , design[0] );
		bP.drawVerticalLine( that->getWidth() - 1 , 1 , that->getHeight() - 1 , design[9] );
		bP.drawHorizontalLine( 0 , that->getHeight() - 1 , that->getWidth() , design[9] );
	}
	
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

_callbackReturn _window::mouseClickHandler( _event event )
{
	// Get Source
	_window* that = (_window*)event.getGadget();
	
	if( event.getPosY() > 11 )
		return not_handled;
	
	if( !that->isResizeable() && !that->isMaximized() )
		return not_handled;
	
	if( that->isMaximized() )
		that->unMaximize();
	else
		that->maximize();
	
	return handled;
}

void _window::close()
{
	// Check for permission
	if( this->handleEvent( onClose , true ) != prevent_default )
	{
		// Close the window
		this->setParent( nullptr );
		
		// Unregister window from taskbar 
		if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
			((_windows*)_system::_gadgetHost_)->removeTask( this );
	}
}

_callbackReturn _window::dragHandler( _event event )
{	
	// Get Source
	_window* that = (_window*)event.getGadget();
	
	if( event == onDragStart )
	{
		// If y pos is not on the windowbar, let my children gagdet be the subject of Dragment :-)
		if( event.getPosY() > 11 )
			return prevent_default;
		
		// If y is on the windowbar, drag Me!
		if( that->isMaximized() )
			return prevent_default; // Don't try to drag a window while its maximized
		
		return handled;
	}
	else if( event == onDragging )
	{
		/**
		 * Handling of my 'dragment' !
		 */
		
		// Has the Gadget to move?
		if( event.getDeltaX() == 0 && event.getDeltaY() == 0 )
			return handled;
		
		// Move it relatively
		that->moveRelative( event.getDeltaX() , event.getDeltaY() );
		
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
		this->close();
	// maximize or restore
	else if( event.getGadget<_windowButton>() == this->button[1] )
	{
		if( this->isMaximized() )
			this->unMaximize();
		else
			this->maximize();
	}
	// Minimize
	else if( event.getGadget<_windowButton>() == this->button[2] )
	{
		this->minimize();
		if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
			((_windows*)_system::_gadgetHost_)->refreshTask( this );
	}
	
	return handled;
}

// Dtor
_window::~_window()
{
	delete this->button[0];
	delete this->button[1];
	delete this->button[2];
	delete this->icon;
	
	if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
		((_windows*)_system::_gadgetHost_)->removeTask( this );
}

_window::_window( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string title , _bitmap bmp , bool minimizeable , bool closeable , _style&& style ) :
	_gadget( _gadgetType::window , x , y , width , height , style | _styleAttr::doubleClickable | _styleAttr::focusBringsFront )
	, normalDimensions( nullptr )
	, minimizeable( minimizeable )
	, minimized( false )
	, closeable( closeable )
{
	this->setPadding( { 1 , 10 , 1 , 1 } );
	
	// Set minimum size
	this->setMinWidth( 41 );
	this->setMinHeight( 19 );
	
	// Create a Label
	this->label = new _label( 2 , 2 , this->getWidth() - 2 , 6 , title );
	this->label->setAlign( _align::left );
	this->label->setVAlign( _valign::middle );
	this->label->setColor( COLOR_WHITE );
	
	// Create Icon
	bmp.resize( 6 , 6 ); // Crop to 6x6
	this->icon = new _imageGadget( 2 , 2 , move(bmp) , _style() | _styleAttr::notResizeable );
	
	// Append it to this button
	this->addEnhancedChild( this->label );
	this->addEnhancedChild( this->icon );
	
	this->button[0] = new _windowButton( this->getWidth() - 10 , 1 , 0 );
	this->button[1] = new _windowButton( this->getWidth() - 19 , 1 , 1 );
	this->button[2] = new _windowButton( this->getWidth() - 28 , 1 , 2 );
	
	// Register close-handler
	this->button[0]->setUserEventHandler( onMouseClick , make_callback( this , &_window::buttonHandler ) );
	this->button[1]->setUserEventHandler( onMouseClick , make_callback( this , &_window::buttonHandler ) );
	this->button[2]->setUserEventHandler( onMouseClick , make_callback( this , &_window::buttonHandler ) );
	
	this->button[0]->enhanceToParent( this );
	this->button[1]->enhanceToParent( this );
	this->button[2]->enhanceToParent( this );
	
	// Register update-Handler...
	this->setInternalEventHandler( onUpdate , make_callback( &_window::updateHandler ) );
	
	// that will refresh Window-Buttons and labels
	this->updateNow();
	
	// Register my handler as the default Refresh-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_window::refreshHandler ) );
	this->setInternalEventHandler( onBlur , make_callback( &_window::focusHandler ) );
	this->setInternalEventHandler( onFocus , make_callback( &_window::focusHandler ) );
	this->setInternalEventHandler( onDragging , make_callback( &_window::dragHandler ) );
	this->setInternalEventHandler( onDragStart , make_callback( &_window::dragHandler ) );
	this->setInternalEventHandler( onRestyle , make_callback( &_window::updateHandler ) );
	this->setInternalEventHandler( onMaximize , make_callback( &_window::updateHandler ) );
	this->setInternalEventHandler( onUnMaximize , make_callback( &_window::updateHandler ) );
	this->setInternalEventHandler( onResize , make_callback( &_window::updateHandler ) );
	this->setInternalEventHandler( onMouseDblClick , make_callback( &_window::mouseClickHandler ) );

	
	// Refresh Me
	this->redraw();
}