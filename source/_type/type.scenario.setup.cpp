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


const string basepath = "%APPDATA%/usericons/";

_map<_u8,string> imageId2filename = {
	{ 1 , basepath + "airplane.bmp" },
	{ 2 , basepath + "astronaut.bmp" },
	{ 3 , basepath + "ball.bmp" },
	{ 4 , basepath + "beach" },
	{ 5 , basepath + "butterfly.bmp" },
	{ 6 , basepath + "car.bmp" },
	{ 7 , basepath + "cat.bmp" },
	{ 8 , basepath + "chess.bmp" },
	{ 9 , basepath + "dirt_bike.bmp" },
	{ 10 , basepath + "dog.bmp" },
	{ 11 , basepath + "drip.bmp" },
	{ 12 , basepath + "duck.bmp" },
	{ 13 , basepath + "fish.bmp" },
	{ 14 , basepath + "frog.bmp" },
	{ 15 , basepath + "guest.bmp" },
	{ 16 , basepath + "guitar.bmp" },
	{ 17 , basepath + "horses.bmp" },
	{ 18 , basepath + "kick.bmp" },
	{ 19 , basepath + "lift_off.bmp" },
	{ 20 , basepath + "palm_tree.bmp" },
	{ 21 , basepath + "pink_flower.bmp" },
	{ 22 , basepath + "red_flower.bmp" },
	{ 23 , basepath + "skater.bmp" },
	{ 24 , basepath + "snowflake.bmp" },
};


_scSetup::_scSetup() :
	state( 0 )
	, profileName( "Admin" )
	, profileIconNumber( 0 )
	, systemTime( _time::now() )
{
	// Reset all gadgets to nullptr
	for( int i = 0; i < 20 ; this->gadgets[i++] = nullptr );
	
	_system::executeTimer( _classCallback( this , &_scSetup::refreshCounterValue ) , 1000 , true );
	
	this->radiogroup = nullptr;
	
	_system::_gadgetHost_ = new _startupScreen( _system::_bgIdBack_ );
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ , 117 );
	
	_system::_gadgetHost_->setInternalEventHandler( _internal_ , _classCallback( this , &_scSetup::refreshStateHandler ) );
	_system::_gadgetHost_->triggerEvent( _internal_ );
}


_scSetup::~_scSetup()
{
	this->destruct();
}


void _scSetup::destruct( bool removeLanguagSelect )
{
	for( _gadget*& g : this->gadgets )
	{
		if( g && ( g->getType() != _gadgetType::selectbox || removeLanguagSelect || this->state != 0 ) )
		{
			delete g;
			g = nullptr;
		}
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
	
	// Refresh
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
		profileIconNumber = that->getStyle().val;
		for( _gadget* g : this->gadgets )
		{
			if( g && g->getType() == _gadgetType::imagegadget )
				g->bubbleRefresh( true );
		}
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
	if( that->getStyle().val != profileIconNumber )
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
	{
		if( profileName.empty() )
			this->gadgets[9]->blink();
		else
		{
			_user* newUser = new _user( profileName );
			newUser->setUsername( profileName );
			newUser->setUsericon( imageId2filename[ profileIconNumber ] );
			_systemController::changeState( _systemController::_systemState::desktop );
		}
	}
	
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
		prev->setInternalEventHandler( mouseClick , _classCallback( this , &_scSetup::stateChangeButtonHandler ) );
		btnPrev->setInternalEventHandler( onAction , _classCallback( this , &_scSetup::stateChangeButtonHandler ) );
		
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
	_label* next = new _label( 50 , 9 , 188 , 176 ,
		state == 3
			? _system::getLocalizedString("lbl_finish")
			: _system::getLocalizedString("lbl_next")
		, _style::storeInt( 1 )
	);
	this->gadgets[0] = btnNext;
	this->gadgets[2] = next;
	next->setColor( RGB( 30 , 30 , 30 ) );
	next->setAlign( _align::right );
	
	// Set Handler
	next->setInternalEventHandler( mouseClick , _classCallback( this , &_scSetup::stateChangeButtonHandler ) );
	btnNext->setInternalEventHandler( onAction , _classCallback( this , &_scSetup::stateChangeButtonHandler ) );
	that->addChild( btnNext );
	that->addChild( next );
	
	// Add this->gadgets according to the current page
	switch( state )
	{
		case 0:
		{
			_label* lbl = new _label( 86 , 52 , _system::getLocalizedString("lbl_choose_language") );
			if( !this->gadgets[5] ) // If not created already
			{
				_select* slc = new _select( 94 , 5 , 85 , 60 , { { 1 , "English" } , { 2 , "Français" } , { 3 , "Deutsch" } , { 4 , "Italiano" } , { 5 , "Español" } } );
				slc->setIntValue( (_u8)_system::getLanguage() );
				slc->setInternalEventHandler( onChange , _classCallback( this , &_scSetup::languageSelectHandler ) );
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
			
			cnt1->setInternalEventHandler( onChange , _classCallback( this , &_scSetup::timeCounterHandler ) );
			cnt2->setInternalEventHandler( onChange , _classCallback( this , &_scSetup::timeCounterHandler ) );
			cnt3->setInternalEventHandler( onChange , _classCallback( this , &_scSetup::timeCounterHandler ) );
			
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
			txtName->setInternalEventHandler( onChange , _classCallback( this , &_scSetup::profileNameTextboxHandler ) );
			
			this->gadgets[6] = lbl2;
			this->gadgets[7] = lbl3;
			this->gadgets[8] = lbl4;
			this->gadgets[9] = txtName;
			lbl2->setColor( RGB( 30 , 30 , 30 ) );
			lbl3->setColor( RGB( 30 , 30 , 30 ) );
			lbl4->setColor( RGB( 30 , 30 , 30 ) );
			lbl2->setFont( _system::getFont( "ArialBlack13" ) );
			that->addChild( lbl2 );
			that->addChild( lbl3 );
			that->addChild( lbl4 );
			that->addChild( txtName );
			
			//! User Logos
			_s8 iconNumbers[] = { 5 , 16 , 17 , 22 , 3 , 11 , 4 , 2 , -1 };
			for( int i = 0 ; iconNumbers[i] >= 0 ; i++ )
			{
				_imagegadget* image = new _imagegadget(
					22 + i * 20
					, 102
					, _user::getUserLogoFromImage(
						_user::getUserImage(
							imageId2filename[ iconNumbers[i] ]
						) 
					)
					, _style::storeInt( iconNumbers[i] )
				);
				image->setInternalEventHandler( refresh , _classCallback( this , &_scSetup::imagegadgetProfileIconHandler ) );
				image->setInternalEventHandler( onFocus , _classCallback( this , &_scSetup::imagegadgetProfileIconHandler ) );
				this->gadgets[ 10 + i ] = image;
				that->addChild( image );
			}
			break;
		}
	}
	return handled;
}