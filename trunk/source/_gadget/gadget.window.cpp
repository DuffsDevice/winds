#include "_gadget/gadget.window.h"
#include "_gadget/gadget.windows.h"
#include "_type/type.system.h"
#include "_type/type.runtimeAttributes.h"


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
	
	if( event.getType() == onMaximize || event.getType() == onUnMaximize )
	{
		that->button[1]->bubbleRefresh( true );
		return handled;
	}
	
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
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	bP.fill( RGB( 30 , 30 , 29 ) );
	
	if( that->hasFocus() )
	{
		const _bitmap& design = _system::_rtA_->getWindowsDesignActive();
		
		// Window-Bar
		bP.copyHorizontalStretch( 0 , 0 , that->getWidth() , design );
		
		// Bottom Border
		bP.drawVerticalLine( 0 , 1 , that->getHeight() - 1 , design[0] );
		bP.drawVerticalLine( that->getWidth() - 1 , 1 , that->getHeight() - 1 , design[9] );
		bP.drawHorizontalLine( 0 , that->getHeight() - 1 , that->getWidth() , design[9] );
	}
	else
	{
		const _bitmap& design = _system::_rtA_->getWindowsDesignBlurred();
		
		// Window-Bar
		bP.copyHorizontalStretch( 0 , 0 , that->getWidth() , design );
		
		// Bottom Border
		bP.drawVerticalLine( 0 , 1 , that->getHeight() - 1 , design[0] );
		bP.drawVerticalLine( that->getWidth() - 1 , 1 , that->getHeight() - 1 , design[9] );
		bP.drawHorizontalLine( 0 , that->getHeight() - 1 , that->getWidth() , design[9] );
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
		if( event.getPosY() > 11 )
			return prevent_default;
		
		// If y is on the windowbar, drag Me!
		if( that->isMaximized() )
			return prevent_default; // Don't try to drag a window while its maximized
		
		return handled;
	}
	else if( event.getType() == dragging )
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
	else // mouseDoubleClick
	{
		if( event.getPosY() > 11 || !that->isResizeable() )
			return not_handled;
		
		if( that->isMaximized() )
			that->unMaximize();
		else
			that->maximize();
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
	else if( event.getGadget<_windowButton>() == this->button[2] )
	{
		this->minimize();
		if( _system::_gadgetHost_ && _system::_gadgetHost_->getScreenType() == _gadgetScreenType::windows )
			((_windows*)_system::_gadgetHost_)->refreshTask( this );
	}
	
	return handled;
}



// C++0x! Yay!
_window::_window( _length width , _length height , _coord x , _coord y , string title , _style&& style ) :
	_window( width , height , x , y , title , _bitmap() , (_style&&)style )
{ }



_window::_window( _length width , _length height , _coord x , _coord y , string title , _bitmap bmp , _style&& style ) :
	_gadget( _gadgetType::window , width , height , x , y , style | _styleAttr::doubleClickable | _styleAttr::focusBringsFront )
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
	this->button[0]->setInternalEventHandler( onAction , _classCallback( this , &_window::buttonHandler ) );
	this->button[1]->setInternalEventHandler( onAction , _classCallback( this , &_window::buttonHandler ) );
	this->button[2]->setInternalEventHandler( onAction , _classCallback( this , &_window::buttonHandler ) );
	
	this->button[0]->enhanceToParent( this );
	this->button[1]->enhanceToParent( this );
	this->button[2]->enhanceToParent( this );
	
	// Register my handler as the default Refresh-Handler
	this->setInternalEventHandler( refresh , _staticCallback( &_window::refreshHandler ) );
	this->setInternalEventHandler( onBlur , _staticCallback( &_window::refreshHandler ) );
	this->setInternalEventHandler( onFocus , _staticCallback( &_window::refreshHandler ) );
	this->setInternalEventHandler( dragging , _staticCallback( &_window::dragHandler ) );
	this->setInternalEventHandler( dragStart , _staticCallback( &_window::dragHandler ) );
	this->setInternalEventHandler( onStyleSet , _staticCallback( &_window::restyleHandler ) );
	this->setInternalEventHandler( onMaximize , _staticCallback( &_window::restyleHandler ) );
	this->setInternalEventHandler( onUnMaximize , _staticCallback( &_window::restyleHandler ) );
	this->setInternalEventHandler( onResize , _staticCallback( &_window::resizeHandler ) );
	this->setInternalEventHandler( mouseDoubleClick , _staticCallback( &_window::dragHandler ) );
	
	// Refresh Window-Buttons
	this->triggerEvent( onStyleSet );
	
	// Refresh Me
	this->refreshBitmap();
}