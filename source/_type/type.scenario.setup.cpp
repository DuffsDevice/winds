#include "_type/type.scenario.setup.h"
#include "_type/type.system.h"
#include "_type/type.system.controller.h"
#include "_gadget/gadget.startupScreen.h"
#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.imagegadget.h"
#include "_gadget/gadget.select.h"
#include "_gadget/gadget.counter.h"
#include "_gadget/gadget.radio.h"
#include "_gadget/gadget.actionButton.h"
#include "_gadget/gadget.textarea.h"

#include <string.h>
#include <math.h>

#define M_PI		3.14159265358979323846


_scSetup::_scSetup() :
	state( 0 )
	, profileName( "Admin" )
	, profileIcon( 0 )
	, systemTime( _time::now() )
{
	// Reset all gadgets to nullptr
	for( int i = 0; i < 20 ; this->gadgets[i++] = nullptr );
	
	_system::executeTimer( new _classCallback( this , &_scSetup::refreshCounterValue ) , 1000 , true );
	
	this->radiogroup = nullptr;
	
	_system::_gadgetHost_ = new _startupScreen( _system::_bgIdBack_ );
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ , 117 );
	
	_system::_gadgetHost_->registerEventHandler( _internal_ , new _classCallback( this , &_scSetup::refreshStateHandler ) );
	_system::_gadgetHost_->triggerEvent( _internal_ );
}


_scSetup::~_scSetup()
{
	this->destruct();
}


void _scSetup::destruct( bool removeLanguagSelect )
{
	for( int i = 0; i < 20 ; i++ )
		if( this->gadgets[i] && ( this->gadgets[i]->getType() != _gadgetType::selectbox || removeLanguagSelect || this->state != 0 ) )
		{
			delete this->gadgets[i];
			this->gadgets[i] = nullptr;
		}
	
	if( this->radiogroup )
	{
		delete this->radiogroup;
		this->radiogroup = nullptr;
	}
}


_callbackReturn _scSetup::profileNameTextboxHandler( _event e )
{
	_textbox* that = e.getGadget<_textbox>();
	
	// Write ProfileName
	if( that->getStyle().val > 2 )
		this->profileName = that->getStrValue();
	
	return handled;
}


_callbackReturn _scSetup::languageSelectHandler( _event e )
{
	_select* that = e.getGadget<_select>();
	
	// Write Language
	_system::setLanguage( (_language)that->getIntValue() );
	
	// Refresh all Labels!
	_system::_gadgetHost_->triggerEvent( _internal_ );
	
	return handled;
}


_callbackReturn _scSetup::timeCounterHandler( _event e )
{
	// Set Time according to Counter-Value
	systemTime.set( _timeAttr::hour , ((_counter*)this->gadgets[12])->getIntValue() , false );
	systemTime.set( _timeAttr::minute , ((_counter*)this->gadgets[13])->getIntValue() , false );
	systemTime.set( _timeAttr::second , ((_counter*)this->gadgets[14])->getIntValue() , false );
	
	_int seconds = systemTime.get( _timeAttr::second );
	_int minutes = systemTime.get( _timeAttr::minute );
	_int hours = systemTime.get( _timeAttr::hour );
	
	// Get Image and Bitmap
	_imagegadget* clkImage = (_imagegadget*)(this->gadgets[16]);
	_bitmap& clkBmp = clkImage->getModifyableImage();
	
	// Reset Clock-Image
	clkBmp.reset( RGBHEX( 0x5A7EDC ) );
	clkBmp.resetClippingRect();
	clkBmp.drawFilledCircle( 25 , 25 , 25 , RGBHEX( 0x6082E3 ) );
	
	// Dots
	for( float i = 0 ; i < 4 ; i+= 1 )
	{
		int c = cos( i * 30 / 180 * M_PI ) * 25;
		int s = sin( i * 30 / 180 * M_PI ) * 25;
		int c2 = c * 25/24;
		int s2 = s * 25/24;
		clkBmp.drawPixel( 25 - c , 25 - s , COLOR_WHITE );
		clkBmp.drawPixel( 25 + c , 25 - s , COLOR_WHITE );
		clkBmp.drawPixel( 25 - c , 25 + s , COLOR_WHITE );
		clkBmp.drawPixel( 25 + c , 25 + s , COLOR_WHITE );
		clkBmp.drawPixel( 25 - c2 , 25 - s2 , COLOR_WHITE );
		clkBmp.drawPixel( 25 + c2 , 25 - s2 , COLOR_WHITE );
		clkBmp.drawPixel( 25 - c2 , 25 + s2 , COLOR_WHITE );
		clkBmp.drawPixel( 25 + c2 , 25 + s2 , COLOR_WHITE );
	}
	
	// Second
	clkBmp.drawLine( 25 , 25
		, 25 - cos( float( seconds + 15 ) * 6 / 180 * M_PI ) * 23 + 0.5 
		, 25 - sin( float( seconds + 15 ) * 6 / 180 * M_PI ) * 23 + 0.5 
		, RGB255( 255 , 48 , 53 )
	);
	
	// Minute
	clkBmp.drawLine( 25 , 25
		, 25 - cos( float( minutes + 15 ) * 6 / 180 * M_PI ) * 20 + 0.5 
		, 25 - sin( float( minutes + 15 ) * 6 / 180 * M_PI ) * 20 + 0.5 
		, RGB( 21 , 24 , 31 )
	);
	
	float hrAngleInRad = ( float( hours + 3 ) * 30 + float( minutes ) / 2 ) / 180 * M_PI;
	
	// Hour
	clkBmp.drawLine( 25 , 25 
		, 25 - cos( hrAngleInRad ) * 12 + 0.5
		, 25 - sin( hrAngleInRad ) * 12 + 0.5
		, RGB( 0 , 6 , 15 )
	);
	
	clkImage->bubbleRefresh( true );
	return handled;
}


_callbackReturn _scSetup::imagegadgetProfileIconHandler( _event event )
{
	_imagegadget* that = event.getGadget<_imagegadget>();
	
	if( event.getType() == onFocus )
	{
		profileIcon = that->getStyle().val;
		event.getGadget()->bubbleRefresh( true );
		for( int i = 10 ; i < 18 ; i++ )
			if( i != profileIcon )
				this->gadgets[i]->bubbleRefresh( true );
		return handled;
	}
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	if( that->getImage().isValid() )
		bP.copyTransparent( 0 , 0 , that->getImage() );
	
	// Make a user-image appear semi-transparent
	if( that->getStyle().val != profileIcon )
	{
		for (_u32 i = 0; i != bP.getWidth(); i++ )
		{
			for (_u32 j = 0; j != bP.getHeight() ; j++ )
			{
				_pixel color = bP.getPixel( i , j );
				
				// Extract individual components
				u8 r = ( color >>  0 ) & 31;
				u8 g = ( color >>  5 ) & 31;
				u8 b = ( color >> 10 ) & 31;
				
				r = ( r + RGB_GETR( RGBHEX( 0x5A7EDC ) ) ) >> 1;
				g = ( g + RGB_GETG( RGBHEX( 0x5A7EDC ) ) ) >> 1;
				b = ( b + RGB_GETB( RGBHEX( 0x5A7EDC ) ) ) >> 1;
				
				color = RGB( r , g , b );
				
				// Plot the pixel
				bP.drawPixel( i , j , color );
			}
		}
	}
	
	return use_default;
}


_callbackReturn _scSetup::stateChangeButtonHandler( _event e )
{
	_gadget* that = e.getGadget();
	
	// Event must be from "next"- or "prev"-buttons
	if( that->getStyle().val == -1 )
	{
		if( state > 0 )
		{
			state--;
			_system::_gadgetHost_->triggerEvent( _event( _internal_ ) );
		}
	}
	else if( state < 3 )
	{
		state++;
		_system::_gadgetHost_->triggerEvent( _event( _internal_ ) );
	}
	else
		_systemController::changeState( _systemController::_systemState::desktop );
	
	return handled;
}

void _scSetup::refreshCounterValue()
{
	if( this->gadgets[12] && this->gadgets[12]->getType() == _gadgetType::counter && this->gadgets[13] && this->gadgets[13]->getType() == _gadgetType::counter )
	{
		_counter* cnt1 = (_counter*) this->gadgets[12];
		_counter* cnt2 = (_counter*) this->gadgets[13];
		_counter* cnt3 = (_counter*) this->gadgets[14];
		
		if( cnt3->getIntValue() == 59 )
		{
			if( cnt2->getIntValue() == 59 )
				cnt1->increase();
			cnt2->increase();
		}
		
		// Increase Seconds
		cnt3->increase();
		
		// Refresh Clock-Image
		this->timeCounterHandler( _none_ );
	}
	else
		this->systemTime = _time( _u32(this->systemTime) + 1 );
}


_callbackReturn _scSetup::refreshStateHandler( _event e )
{
	_gadget* that = e.getGadget();
	
	//! Remove all old gadgets and data
	this->destruct( false );
	
	// Standard
	if( state != 0 )
	{
		_gadget* btnPrev = new _actionButton( _actionButtonType::prev , 4 , 176 , _style::storeInt( -1 ) );
		_label* prev = new _label( 50 , 9 , 17 , 176 , _system::getLocalizedString("lbl_prev") , _style::storeInt( -1 ) );
		prev->setColor( RGB( 30 , 30 , 30 ) );
		prev->setAlign( _align::left );
		prev->registerEventHandler( mouseClick , new _classCallback( this , &_scSetup::stateChangeButtonHandler ) );
		btnPrev->registerEventHandler( onAction , new _classCallback( this , &_scSetup::stateChangeButtonHandler ) );
		
		this->gadgets[1] = btnPrev;
		this->gadgets[3] = prev;
		that->addChild( prev );
		that->addChild( btnPrev );
	}
	else
	{
		// Hint to click continue
		_label* clickNext = new _label( 180 , 9 , 71 , 6 , _system::getLocalizedString("lbl_to_continue") );
		clickNext->setColor( RGB( 17 , 17 , 31 ) );
		clickNext->setAlign( _align::right );
		
		this->gadgets[4] = clickNext;
		that->addChild( clickNext );
	}
	
	_gadget* btnNext = new _actionButton( _actionButtonType::next , 240 , 176 , _style::storeInt( 1 ) );
	_label* next = new _label( 50 , 9 , 188 , 176 , _system::getLocalizedString("lbl_next") , _style::storeInt( 1 ) );
	this->gadgets[0] = btnNext;
	this->gadgets[2] = next;
	next->setColor( RGB( 30 , 30 , 30 ) );
	next->setAlign( _align::right );
	
	// Set Handler
	next->registerEventHandler( mouseClick , new _classCallback( this , &_scSetup::stateChangeButtonHandler ) );
	btnNext->registerEventHandler( onAction , new _classCallback( this , &_scSetup::stateChangeButtonHandler ) );
	that->addChild( btnNext );
	that->addChild( next );
	
	
	// Add this->gadgets according to the current page
	switch( state )
	{
		case 0:
		{
			_label* lbl = new _label( 88 , 52 , _system::getLocalizedString("lbl_choose_language") );
			if( !this->gadgets[5] ) // If not created already
			{
				_select* slc = new _select( 90 , 5 , 87 , 60 , { { 1 , "English" } , { 2 , "Français" } , { 3 , "Deutsch" } , { 4 , "Italiano" } , { 5 , "Español" } } );
				slc->setIntValue( (_u8)_system::getLanguage() );
				slc->registerEventHandler( onChange , new _classCallback( this , &_scSetup::languageSelectHandler ) );
				this->gadgets[5] = slc;
				that->addChild( slc );
			}
			
			//_textarea* txt = new _textarea( 90 , 50 , 10 , 130 , "Hallo\nergergerghalloweltsgarten\nwtfiiiii\ngegr4ergerg" );
			//that->addChild( txt );
			//this->gadgets[7] = txt;
			
			that->addChild( lbl );
			this->gadgets[6] = lbl;
			lbl->setColor( RGB( 30 , 30 , 30 ) );
			break;
		}
		case 1:
		{
			//! Create Label with shadow
			//_label* lbl = new _label( 14 , 34 , _system::getLocalizedString("lbl_welcome_to_winds") );
			//this->gadgets[5] = lbl;
			//lbl->setColor( RGB( 2 , 5 , 15 ) );
			//lbl->setFont( _system::getFont( "ArialBlack13" ) );
			
			_label* lbl2 = new _label( 13 , 33 , _system::getLocalizedString("lbl_welcome_to_winds") );
			lbl2->setColor( RGB( 30 , 30 , 30 ) );
			lbl2->setFont( _system::getFont( "ArialBlack13" ) );
			this->gadgets[6] = lbl2;
			
			_label* lbl3 = new _label( 20 , 60 , _system::getLocalizedString("txt_few_step_setup") );
			lbl3->setColor( RGB( 30 , 30 , 30 ) );
			this->gadgets[7] = lbl3;
			
			_label* lbl4 = new _label( 20 , 70 , _system::getLocalizedString("txt_few_step_setup_2") );
			lbl4->setColor( RGB( 30 , 30 , 30 ) );
			this->gadgets[8] = lbl4;
			
			
			
			//that->addChild( lbl );
			that->addChild( lbl2 );
			that->addChild( lbl3 );
			that->addChild( lbl4 );
			break;
		}
		case 2:
		{
			//! Create Label with shadow
			//_label* lbl = new _label( 14 , 34 , _system::getLocalizedString("lbl_system_preferences") );
			//this->gadgets[5] = lbl;
			//lbl->setColor( RGB( 2 , 5 , 15 ) );
			//lbl->setFont( _system::getFont( "ArialBlack13" ) );
			//that->addChild( lbl );
			
			_label* lbl2 = new _label( 13 , 33 , _system::getLocalizedString("lbl_system_preferences") );
			_label* lbl3 = new _label( 32 , 55 , _system::getLocalizedString("txt_system_clock") );
			_label* lbl4 = new _label( 32 , 140 , _system::getLocalizedString("txt_system_clock_auto_fetch_1") );
			_label* lbl5 = new _label( 32 , 150 , _system::getLocalizedString("txt_system_clock_auto_fetch_2") );
			
			//! Create the Clock image as well as the counters that modify it
			_bitmap bmp = _bitmap( 51 , 51 );
			_imagegadget* clockImg = new _imagegadget( 102 , 65 , bmp );
			_counter* cnt1 = new _counter( 85 , 120 , 25 , true , systemTime.get( _timeAttr::hour ) , 23 , 0 );
			_counter* cnt2 = new _counter( 115 , 120 , 25 , true , systemTime.get( _timeAttr::minute ) , 59 , 0 );
			_counter* cnt3 = new _counter( 145 , 120 , 25 , true , systemTime.get( _timeAttr::second ) , 59 , 0 );
			
			cnt1->registerEventHandler( onChange , new _classCallback( this , &_scSetup::timeCounterHandler ) );
			cnt2->registerEventHandler( onChange , new _classCallback( this , &_scSetup::timeCounterHandler ) );
			cnt3->registerEventHandler( onChange , new _classCallback( this , &_scSetup::timeCounterHandler ) );
			
			_singleValueGroup<_radio>* radgrp = new _singleValueGroup<_radio>();
			_radio* rad1 = new _radio( 20 , 54 , radgrp );
			_radio* rad2 = new _radio( 20 , 139 , radgrp );
			radgrp->enableSelector( rad1 );
			this->radiogroup = radgrp;
			
			this->gadgets[6] = lbl2;
			this->gadgets[7] = lbl3;
			this->gadgets[8] = lbl4;
			this->gadgets[9] = lbl5;
			this->gadgets[10] = rad1;
			this->gadgets[11] = rad2;
			this->gadgets[12] = cnt1;
			this->gadgets[13] = cnt2;
			this->gadgets[14] = cnt3;
			this->gadgets[16] = clockImg;
			
			// Refresh Counter-Value and Clock-Image
			_scSetup::refreshCounterValue();
			
			lbl2->setColor( RGB( 30 , 30 , 30 ) );
			lbl3->setColor( RGB( 30 , 30 , 30 ) );
			lbl4->setColor( RGB( 30 , 30 , 30 ) );
			lbl5->setColor( RGB( 30 , 30 , 30 ) );
			
			lbl2->setFont( _system::getFont( "ArialBlack13" ) );
			
			that->addChild( lbl2 );
			that->addChild( lbl3 );
			that->addChild( lbl4 );
			that->addChild( lbl5 );
			that->addChild( rad1 );
			that->addChild( rad2 );
			that->addChild( cnt1 );
			that->addChild( cnt2 );
			that->addChild( cnt3 );
			
			// Refresh Clock-Image
			cnt1->handleEvent( _event( onChange ) );
			
			// Then add it
			that->addChild( clockImg );
			
			break;
		}
		case 3:
		{
			//! Create Label with shadow
			//this->gadgets[5] = lbl;
			//lbl->setColor( RGB( 2 , 5 , 15 ) );
			//lbl->setFont( _system::getFont( "ArialBlack13" ) );
			//that->addChild( lbl );
			//_label* lbl = new _label( 14 , 34 , _system::getLocalizedString("lbl_profile") );
			
			_label* lbl2 = new _label( 13 , 33 , _system::getLocalizedString("lbl_profile") );
			_label* lbl3 = new _label( 20 , 60 , _system::getLocalizedString("txt_name") );
			_label* lbl4 = new _label( 20 , 90 , _system::getLocalizedString("txt_profile_icon") );
			_textbox* txtName = new _textbox( 21 , 70 , 80 , profileName , _style::storeInt( 4 ) );
			txtName->registerEventHandler( onChange , new _classCallback( this , &_scSetup::profileNameTextboxHandler ) );
			
			//! User Logos
			_imagegadget* image1 = new _imagegadget( 22 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/butterflyl.png" ) ) , _style::storeInt( 1 ) );
			_imagegadget* image2 = new _imagegadget( 42 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/guitarl.png" ) ) , _style::storeInt( 2 ) );
			_imagegadget* image3 = new _imagegadget( 62 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/horsesl.png" ) ) , _style::storeInt( 3 ) );
			_imagegadget* image4 = new _imagegadget( 82 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/red flowerl.png" ) ) , _style::storeInt( 4 ) );
			_imagegadget* image5 = new _imagegadget( 102 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/balll.png" ) ) , _style::storeInt( 5 ) );
			_imagegadget* image6 = new _imagegadget( 122 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/dripl.png" ) ) , _style::storeInt( 6 ) );
			_imagegadget* image7 = new _imagegadget( 142 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/beachl.png" ) ) , _style::storeInt( 7 ) );
			_imagegadget* image8 = new _imagegadget( 162 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/astronautl.png" ) ) , _style::storeInt( 8 ) );
			this->gadgets[6] = lbl2;
			this->gadgets[7] = lbl3;
			this->gadgets[8] = lbl4;
			this->gadgets[9] = txtName;
			this->gadgets[10] = image1;
			this->gadgets[11] = image2;
			this->gadgets[12] = image3;
			this->gadgets[13] = image4;
			this->gadgets[14] = image5;
			this->gadgets[15] = image6;
			this->gadgets[16] = image7;
			this->gadgets[17] = image8;
			for( int i = 10 ; i < 18 ; i++ )
			{
				this->gadgets[i]->registerEventHandler( refresh , new _classCallback( this , &_scSetup::imagegadgetProfileIconHandler ) );
				this->gadgets[i]->registerEventHandler( onFocus , new _classCallback( this , &_scSetup::imagegadgetProfileIconHandler ) );
			}
			lbl2->setColor( RGB( 30 , 30 , 30 ) );
			lbl3->setColor( RGB( 30 , 30 , 30 ) );
			lbl4->setColor( RGB( 30 , 30 , 30 ) );
			lbl2->setFont( _system::getFont( "ArialBlack13" ) );
			that->addChild( lbl2 );
			that->addChild( lbl3 );
			that->addChild( lbl4 );
			that->addChild( txtName );
			that->addChild( image1 );
			that->addChild( image2 );
			that->addChild( image3 );
			that->addChild( image4 );
			that->addChild( image5 );
			that->addChild( image6 );
			that->addChild( image7 );
			that->addChild( image8 );
			break;
		}
	}
	return handled;
}