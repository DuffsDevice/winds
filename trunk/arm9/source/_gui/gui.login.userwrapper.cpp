#include "_gui/gui.login.userwrapper.h"

//! Gadgets we need
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.button.action.h"

_callbackReturn _userWrapper::textboxRefreshHandler( _event event )
{
	// Get Gadget
	_textBox* that = event.getGadget<_textBox>();
	
	// Get bitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	bP.drawRect( 0 , 0 , myW , myH , _color::white );
	
	bP.drawPixel( myW - 1 , myH - 1 , _color::transparent );
	bP.drawPixel( myW - 1 , 0 , _color::transparent );
	bP.drawPixel( 0 , myH - 1 , _color::transparent );
	bP.drawPixel( 0 , 0 , _color::transparent );
	
	return use_default;
}

_callbackReturn _userWrapper::refreshHandler( _event event )
{
	_userWrapper* that = event.getGadget<_userWrapper>();
	
	// Get bitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( _color::fromHex( 0x5A7EDC ) );
	
	if( that->hasFocus() )
	{
		bP.drawVerticalLine( 0 , 3 , 14 , _color::fromRGB8( 165 , 178 , 234 ) );
		
		// Background
		bP.drawFilledRect( 1 , 1 , 16 , 18 , _color::fromRGB8( 18 , 66 , 166 ) );
		bP.drawHorizontalGradient( 17 , 1 , 87 - 15 , 18 , _color::fromRGB8( 18 , 66 , 166 ) , _color::fromHex( 0x5A7EDC ) );
		
		// LeftTop border
		bP.drawHorizontalLine( 0 , 2 , 2 , _color::fromRGB8( 112 , 142 , 215 ) );
		bP.drawVerticalLine( 2 , 0 , 2 , _color::fromRGB8( 112 , 142 , 215 ) );
		bP.drawPixel( 1 , 1 , _color::fromRGB8( 165 , 178 , 234 ) );
		
		// LeftBottom border
		bP.drawHorizontalLine( 0 , 17 , 2 , _color::fromRGB8( 112 , 142 , 215 ) );
		bP.drawVerticalLine( 2 , 18 , 2 , _color::fromRGB8( 112 , 142 , 215 ) );
		bP.drawPixel( 1 , 18 , _color::fromRGB8( 165 , 178 , 234 ) );
		
		// Top Line
		bP.drawHorizontalGradient( 3 , 0 , 87 , 1 , _color::fromRGB8( 165 , 178 , 234 ) , _color::fromHex( 0x5A7EDC ) );
		
		// Bottom Line
		bP.drawHorizontalGradient( 3 , 19 , 87 , 1 , _color::fromRGB8( 165 , 178 , 234 ) , _color::fromHex( 0x5A7EDC ) );
	}
	
	return use_default;
}


_callbackReturn _userWrapper::submitHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	_userWrapper* wrapper = (_userWrapper*) that->getStyle().data;
	
	// Check if the user really has a password
	if( wrapper->user.hasPassword() )
	{
		if( that->getType() == _gadgetType::imagegadget )
			return handled;
		
		if( !wrapper->user.checkPassword( wrapper->passwordbox->getStrValue() ) )
		{
			// Reset Password _textBox
			wrapper->passwordbox->setStrValue( "" );
			
			// Make it blink
			wrapper->passwordbox->blink();
			
			return handled;
		}
	}
	
	// Throw an event on which _guiLogin will react!
	wrapper->triggerEvent( onMouseClick );
	
	return handled;
}

_callbackReturn _userWrapper::focusHandler( _event event )
{
	_userWrapper* that = event.getGadget<_userWrapper>();
	
	if( that->user.hasPassword() )
	{
		if( event == onFocus )
		{
			that->passwordbox = new _passcodeBox( 19 , 13 , 55 , 8 , "" , nullptr , 0 , _style::storeHandle( that ) );
			that->passwordsubmit = new _actionButton( 77 , 13 , _actionButtonType::next , _style::storeHandle( that , _style::canNotTakeFocus ) );
			that->passwordsubmit->setInternalEventHandler( onMouseClick , make_callback( &_userWrapper::submitHandler ) );
			that->passwordbox->setUserEventHandler( onDraw , make_callback( &_userWrapper::textboxRefreshHandler ) );
			that->passwordbox->redraw();
			that->addChild( that->passwordbox );
			that->addChild( that->passwordsubmit );
			
			that->passwordbox->focus();
		}
		else
		{
			delete that->passwordbox;
			delete that->passwordsubmit;
		}
	}
	
	that->redraw();
	
	return handled;
}


_userWrapper::_userWrapper( _coord x , _coord y , string iniPath , _style&& style ) :
	_gadget( _gadgetType::none , x , y , 90 , 23 , (_style&&)style )
	, passwordbox( nullptr )
	, passwordsubmit( nullptr )
	, user( iniPath )
{
	// Register some handler
	this->setInternalEventHandler( onDraw , make_callback( _userWrapper::refreshHandler ) );
	this->setInternalEventHandler( onFocus , make_callback( _userWrapper::focusHandler ) );
	this->setInternalEventHandler( onBlur , make_callback( _userWrapper::focusHandler ) );
	
	// Click on the image to login a user without password
	_imageGadget* img = new _imageGadget( 3 , 3 , user.getLogo() , ignore , ignore , _style::storeHandle( this , _style::canNotTakeFocus ) );
	img->setInternalEventHandler( onMouseClick , make_callback( _userWrapper::submitHandler ) );
	
	// Add the logo
	this->addChild( img );
	
	_label* lbl = new _label( 20 , 4 , ignore , ignore , user.getName() , _style::canNotTakeFocus);
	lbl->setColor( _color::white );
	this->addChild( lbl );
	
	// Refresh
	this->redraw();
}


_userWrapper::~_userWrapper()
{
	// Remove additional gadgets
	this->blur();
	
	// Remove Children!
	this->removeChildren( true );
}