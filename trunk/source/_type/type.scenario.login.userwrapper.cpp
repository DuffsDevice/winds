#include "_type/type.scenario.login.userwrapper.h"
#include "_type/type.system.h"

//! Gadgets we need
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.actionButton.h"

_callbackReturn _userWrapper::textboxRefreshHandler( _event event )
{	
	_textbox* that = event.getGadget<_textbox>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	bP.fill( COLOR_WHITE );
	bP.drawPixel( myW - 1 , myH - 1 , NO_COLOR );
	bP.drawPixel( myW - 1 , 0 , NO_COLOR );
	bP.drawPixel( 0 , myH - 1 , NO_COLOR );
	bP.drawPixel( 0 , 0 , NO_COLOR );
	
	// If there is no font it doesn't make sense to paint
	if( that->getFont() && that->getFont()->valid() )
	{
		string str = string( that->getStrValue().length() , '°' );
		
		// Draw Shadow Text...
		if( !str.length() )
			bP.drawString( 2 , 1 , that->getFont() , _system::getLocalizedString("lbl_passcode") , RGB( 18 , 18 , 18 ) );
		else
			bP.drawString( 2 , 1 , that->getFont() , str , that->getColor() );
		
		if( that->getCursor() >= 0 )
		{
			// Get String until cursor
			string str2 = str;
			str2.resize( that->getCursor() );
			
			_length strWidthUntilCursor = that->getFont()->getStringWidth( str2 );
			
			bP.drawVerticalLine( strWidthUntilCursor + 2 - 1 , 1 /* */ - 1 , that->getFont()->getAscent() + 2 , COLOR_RED );
		}
	}
	
	return use_default;
}

_callbackReturn _userWrapper::refreshHandler( _event event )
{
	_userWrapper* that = event.getGadget<_userWrapper>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	bP.fill( RGBHEX( 0x5A7EDC ) );
	
	if( that->hasFocus() )
	{
		bP.drawVerticalLine( 0 , 3 , 14 , RGB255( 165 , 178 , 234 ) );
		
		// Background
		bP.drawFilledRect( 1 , 1 , 16 , 18 , RGB255( 18 , 66 , 166 ) );
		bP.drawHorizontalGradient( 17 , 1 , 87 - 15 , 18 , RGB255( 18 , 66 , 166 ) , RGBHEX( 0x5A7EDC ) );
		
		// LeftTop border
		bP.drawHorizontalLine( 0 , 2 , 2 , RGB255( 112 , 142 , 215 ) );
		bP.drawVerticalLine( 2 , 0 , 2 , RGB255( 112 , 142 , 215 ) );
		bP.drawPixel( 1 , 1 , RGB255( 165 , 178 , 234 ) );
		
		// LeftBottom border
		bP.drawHorizontalLine( 0 , 17 , 2 , RGB255( 112 , 142 , 215 ) );
		bP.drawVerticalLine( 2 , 18 , 2 , RGB255( 112 , 142 , 215 ) );
		bP.drawPixel( 1 , 18 , RGB255( 165 , 178 , 234 ) );
		
		// Top Line
		bP.drawHorizontalGradient( 3 , 0 , 87 , 1 , RGB255( 165 , 178 , 234 ) , RGBHEX( 0x5A7EDC ) );
		
		// Bottom Line
		bP.drawHorizontalGradient( 3 , 19 , 87 , 1 , RGB255( 165 , 178 , 234 ) , RGBHEX( 0x5A7EDC ) );
	}
	
	return use_default;
}


_callbackReturn _userWrapper::submitHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	_userWrapper* wrapper = (_userWrapper*) that->getStyle().data;
	
	// Check if the user really has a password
	if( wrapper->user->hasPassword() )
	{
		if( that->getType() == _gadgetType::imagegadget )
			return handled;
		
		if( !wrapper->user->checkPassword( wrapper->passwordbox->getStrValue() ) )
		{
			// Reset Password _textbox
			wrapper->passwordbox->setStrValue( "" );
			
			// Make it blink
			wrapper->passwordbox->blink();
			
			return handled;
		}
	}
	
	// Throw an event on which _scLogin will react!
	wrapper->triggerEvent( onAction );
	
	return handled;
}

_callbackReturn _userWrapper::focusHandler( _event event )
{
	_userWrapper* that = event.getGadget<_userWrapper>();
	
	if( that->user->hasPassword() )
	{
		if( event.getType() == onFocus )
		{
			that->passwordbox = new _textbox( 19 , 13 , 55 , 9 , "" , _style::storeHandle( that ) );
			that->passwordsubmit = new _actionButton( _actionButtonType::next , 77 , 13 , _style::storeHandle( that , _styleAttr() | _styleAttr::canNotTakeFocus ) );
			that->passwordsubmit->registerEventHandler( onAction , new _staticCallback( &_userWrapper::submitHandler ) );
			that->passwordbox->registerEventHandler( refresh , new _staticCallback( &_userWrapper::textboxRefreshHandler ) );
			that->passwordbox->refreshBitmap();
			that->addChild( that->passwordbox );
			that->addChild( that->passwordsubmit );
		}
		else
		{
			delete that->passwordbox;
			delete that->passwordsubmit;
		}
	}
	
	that->bubbleRefresh( true );
	
	return handled;
}


_userWrapper::_userWrapper( _coord x , _coord y , _user* user , _style style ) :
	_gadget( _gadgetType::_plain , 90 , 23 , x , y , style )
	, passwordbox( nullptr )
	, passwordsubmit( nullptr )
	, user( user )
{
	// Register some handler
	this->registerEventHandler( refresh , new _staticCallback( _userWrapper::refreshHandler ) );
	this->registerEventHandler( onFocus , new _staticCallback( _userWrapper::focusHandler ) );
	this->registerEventHandler( onBlur , new _staticCallback( _userWrapper::focusHandler ) );
	
	// Click on the image to login a user without password
	_imagegadget* img = new _imagegadget( 3 , 3 , this->user->getLogo() , _style::storeHandle( this , _styleAttr() | _styleAttr::canNotTakeFocus ) );
	img->registerEventHandler( mouseClick , new _staticCallback( _userWrapper::submitHandler ) );
	
	// Add the logo
	this->addChild( img );
	
	_label* lbl = new _label( 20 , 4 , this->user->getUsername() , _styleAttr() | _styleAttr::canNotTakeFocus);
	lbl->setColor( COLOR_WHITE );
	this->addChild( lbl );
	
	// Refresh
	this->refreshBitmap();
}


_userWrapper::~_userWrapper()
{
	// Remove additional gadgets
	this->blur();
	
	// Remove Children!
	this->removeChildren( true );
}