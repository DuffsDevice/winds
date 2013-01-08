#include "_type/type.system.h"
#include "_type/type.radiogroup.h"
#include "_type/type.time.h"
#include "_type/type.dialog.h"
#include "_gadget/gadget.windows.h"
#include "_gadget/gadget.select.h"
#include "_gadget/gadget.counter.h"
#include "_gadget/gadget.radio.h"
#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.imagegadget.h"
#include "_gadget/gadget.startupScreen.h"
#include "_gadget/gadget.bootupScreen.h"
#include "_gadget/gadget.actionButton.h"
#include <nds/interrupts.h>
#include <math.h>

#define M_PI		3.14159265358979323846

void _systemController::main()
{
	changeState( _systemState::bootup );
	
	while( true )
	{
		if( dirty )
		{
			// Fade down Screen
			_system::fadeMainScreen( false );
			
			// Go back to start
			start:
			
			_systemController::dirty = false;
			
			switch( _systemController::state )
			{
				case _systemState::bootup:
					bootupPage();
					break;
				case _systemState::login:
					loginPage();
					break;
				case _systemState::setup:
					setupPage();
					break;
				case _systemState::desktop:
					desktopPage();
					break;
				default:
					break;
			}
			
			if( _systemController::dirty )
				goto start;
			
			// Fade up Screen
			if( _systemController::state != _systemState::empty )
				_system::fadeMainScreen( true );
		}
		
		// Run Programs
		_system::runPrograms();
		
		// Refresh System-Usage
		_system::_cpuUsageTemp_ = ( _system::_cpuUsageTemp_ + _system::_cpuUsageTemp_ + REG_VCOUNT - 192 ) / 3;
		
		// wait until line 0 
		swiIntrWait( 0, IRQ_VCOUNT );
		
		// Wait for VBlank
		swiWaitForVBlank();
		
		printf("\x1b[2J");
		//if( _system::_currentFocus_ )
		//	printf("cF: %s\n",gadgetType2string[_system::_currentFocus_->getType()].c_str());
		//printf("CF:%p",_system::_gadgetHost_->focusedChild);
		for( _gadget* g : _system::_gadgetHost_->children )
			printf("- %s, %d\n",gadgetType2string[g->getType()].c_str(),g->hasFocus() );
		for( _gadget* g : _system::_gadgetHost_->enhancedChildren )
			printf("- %s, %d\n",gadgetType2string[g->getType()].c_str(),g->hasFocus() );
	}	
}

void _systemController::setupPage()
{
	// Clean up
	_system::deleteGadgetHost();
	_system::deleteKeyboard();
	
	_system::_gadgetHost_ = new _startupScreen( _system::_bgIdBack_ );
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ , 117 );
	
	_system::_gadgetHost_->registerEventHandler( _internal_ , new _staticCallback( _systemController::setupHandler ) );
	_system::_gadgetHost_->triggerEvent( _event( _internal_ ) );
}

_callbackReturn _systemController::setupHandler( _event e )
{
	static int state = 0;
	static _gadget* gadgets[20] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
	static string profileName = "Admin";
	static _u8 profileIcon = 0;
	static _time systemTime = _time::now();
	
	_gadget* that = e.getGadget();
	
	if( that->getType() != _gadgetType::screen )
	{
		if( that->getType() == _gadgetType::selectbox )
		{
			_system::setLanguage( (_language)e.getGadget<_select>()->getIntValue() );
			_system::_gadgetHost_->triggerEvent( _event( _internal_ ) );
			return handled;
		}
		//
		// For writing the profile-name
		//
		else if( that->getType() == _gadgetType::label )
		{
			if( that->getStyle().val > 2 )
			{
				profileName = e.getGadget<_textbox>()->getStrValue();
				return handled;
			}
		}
		//
		// For modifying the clock-icon
		//
		else if( that->getType() == _gadgetType::counter )
		{
			systemTime.hour = ((_counter*)gadgets[12])->getIntValue();
			systemTime.minute = ((_counter*)gadgets[13])->getIntValue();
			
			_imagegadget* clkImage = (_imagegadget*)(gadgets[16]);
			_bitmap& clkBmp = clkImage->getModifyableImage();
			
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
			
			// Minute
			clkBmp.drawLine( 25 , 25
				, 25 - cos( float( systemTime.minute + 15 ) * 6 / 180 * M_PI ) * 20 + 0.5 
				, 25 - sin( float( systemTime.minute + 15 ) * 6 / 180 * M_PI ) * 20 + 0.5 
				, RGB( 21 , 24 , 31 )
			);
			// Hour
			clkBmp.drawLine( 25 , 25 
				, 25 - cos( ( float( systemTime.hour + 3 ) * 30 + float( systemTime.minute ) / 2 ) / 180 * M_PI ) * 12 + 0.5
				, 25 - sin( ( float( systemTime.hour + 3 ) * 30 + float( systemTime.minute ) / 2 ) / 180 * M_PI ) * 12 + 0.5
				, RGB( 0 , 6 , 15 )
			);
			
			clkImage->bubbleRefresh( true );
			return handled;
		}
		//
		// For choosing the profile-Icon
		//
		else if( that->getType() == _gadgetType::imagegadget )
		{
			if( e.getType() == onFocus )
			{
				profileIcon = that->getStyle().val;
				e.getGadget()->bubbleRefresh( true );
				for( int i = 10 ; i < 18 ; i++ )
					if( i != profileIcon )
						gadgets[i]->bubbleRefresh( true );
				return handled;
			}
			
			// Receive Gadget
			_imagegadget* that = e.getGadget<_imagegadget>();
			
			_bitmapPort bP = that->getBitmapPort();
			
			if( e.hasClippingRects() )
				bP.addClippingRects( e.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
			else
				bP.normalizeClippingRects();
			
			if( that->getImage().isValid() )
				bP.copyTransparent( 0 , 0 , that->getImage() );
			
			if( that->getStyle().val != profileIcon )
			{
				for (_u32 i = 0; i != bP.getWidth(); i++ )
				{
					for (_u32 j = 0; j != bP.getHeight() ; j++ )
					{
						_pixel color = bP(i, j);
						
						// Extract individual components
						u8 r = (color >> 0) & 31;
						u8 g = (color >> 5) & 31;
						u8 b = (color >> 10) & 31;
						
						r = ( r + RGB_GETR( RGBHEX( 0x5A7EDC ) ) ) >> 1;
						g = ( g + RGB_GETG( RGBHEX( 0x5A7EDC ) ) ) >> 1;
						b = ( b + RGB_GETB( RGBHEX( 0x5A7EDC ) ) ) >> 1;
						
						color = RGB(r,g,b);
						
						// Plot the pixel
						bP.drawPixel(i, j, color);
					}
				}
			}
			
			return use_default;
		}
		
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
			changeState( _systemState::desktop );
		return handled;
	}
	
	for( int i = 0; i < 20 ; i++ )
		if( gadgets[i] )
		{
			delete gadgets[i];
			gadgets[i] = 0;
		}
	
	// Standard
	if( state != 0 )
	{
		_gadget* btnPrev = new _actionButton( _actionButtonType::prev , 4 , 176 , _style::storeInt( -1 ) );
		_label* prev = new _label( 50 , 9 , 17 , 177 , _system::getLocalizedString("lbl_prev") , _style::storeInt( -1 ) );
		prev->setColor( RGB( 30 , 30 , 30 ) );
		prev->setAlign( _align::left );
		prev->registerEventHandler( mouseClick , new _staticCallback( setupHandler ) );
		btnPrev->registerEventHandler( onAction , new _staticCallback( setupHandler ) );
		
		gadgets[1] = btnPrev;
		gadgets[3] = prev;
		that->addChild( prev );
		that->addChild( btnPrev );
	}
	else
	{
		_label* clickNext = new _label( 180 , 9 , 71 , 6 , _system::getLocalizedString("lbl_to_continue") );
		clickNext->setColor( RGB( 17 , 17 , 31 ) );
		clickNext->setAlign( _align::right );
		
		gadgets[4] = clickNext;
		that->addChild( clickNext );
	}
	
	_gadget* btnNext = new _actionButton( _actionButtonType::next , 240 , 176 , _style::storeInt( 1 ) );
	_label* next = new _label( 50 , 9 , 188 , 177 , _system::getLocalizedString("lbl_next") , _style::storeInt( 1 ) );
	gadgets[0] = btnNext;
	gadgets[2] = next;
	next->setColor( RGB( 30 , 30 , 30 ) );
	next->setAlign( _align::right );
	
	// Set Handler
	next->registerEventHandler( mouseClick , new _staticCallback( setupHandler ) );
	btnNext->registerEventHandler( onAction , new _staticCallback( setupHandler ) );
	that->addChild( btnNext );
	that->addChild( next );
	
	switch( state )
	{
		case 0:
		{
			_label* lbl = new _label( 79 , 52 , _system::getLocalizedString("lbl_choose_language") );
			_select* slc = new _select( 90 , 5 , 78 , 60 , { { 1 , "English" } , { 2 , "Français" } , { 3 , "Deutsch" } , { 4 , "Italiano" } , { 5 , "Español" } } );
			slc->setIntValue( (_u8)_system::getLanguage() );
			slc->registerEventHandler( onAction , new _staticCallback( setupHandler ) );
			gadgets[5] = slc;
			gadgets[6] = lbl;
			lbl->setColor( RGB( 30 , 30 , 30 ) );
			that->addChild( slc );
			that->addChild( lbl );
			break;
		}
		case 1:
		{
			// Create Label with shadow
			_label* lbl = new _label( 14 , 34 , _system::getLocalizedString("lbl_welcome_to_winds") );
			_label* lbl2 = new _label( 13 , 33 , _system::getLocalizedString("lbl_welcome_to_winds") );
			_label* lbl3 = new _label( 20 , 60 , _system::getLocalizedString("txt_few_step_setup") );
			_label* lbl4 = new _label( 20 , 70 , _system::getLocalizedString("txt_few_step_setup_2") );
			gadgets[5] = lbl;
			gadgets[6] = lbl2;
			gadgets[7] = lbl3;
			gadgets[8] = lbl4;
			lbl->setColor( RGB( 2 , 5 , 15 ) );
			lbl2->setColor( RGB( 30 , 30 , 30 ) );
			lbl3->setColor( RGB( 30 , 30 , 30 ) );
			lbl4->setColor( RGB( 30 , 30 , 30 ) );
			lbl->setFont( _system::getFont( "ArialBlack10" ) );
			lbl2->setFont( _system::getFont( "ArialBlack10" ) );
			that->addChild( lbl );
			that->addChild( lbl2 );
			that->addChild( lbl3 );
			that->addChild( lbl4 );
			break;
		}
		case 2:
		{
			// Create Label with shadow
			_label* lbl = new _label( 14 , 34 , _system::getLocalizedString("lbl_system_preferences") );
			_label* lbl2 = new _label( 13 , 33 , _system::getLocalizedString("lbl_system_preferences") );
			_label* lbl3 = new _label( 32 , 55 , _system::getLocalizedString("txt_system_clock") );
			_label* lbl4 = new _label( 32 , 140 , _system::getLocalizedString("txt_system_clock_auto_fetch_1") );
			_label* lbl5 = new _label( 32 , 150 , _system::getLocalizedString("txt_system_clock_auto_fetch_2") );
			
			_bitmap bmp = _bitmap( 51 , 51 );
			
			_imagegadget* clockImg = new _imagegadget( 102 , 65 , bmp );
			_counter* cnt1 = new _counter( 100 , 120 , 25 , true , systemTime.hour , 23 , 0 );
			_counter* cnt2 = new _counter( 130 , 120 , 25 , true , systemTime.minute , 59 , 0 );
			
			cnt1->registerEventHandler( onChange , new _staticCallback( setupHandler ) );
			cnt2->registerEventHandler( onChange , new _staticCallback( setupHandler ) );
			
			_radiogroup* radgrp = new _radiogroup();
			_radio* rad1 = new _radio( 20 , 54 , radgrp );
			_radio* rad2 = new _radio( 20 , 139 , radgrp );
			radgrp->enableRadio( rad1 );
			
			gadgets[5] = lbl;
			gadgets[6] = lbl2;
			gadgets[7] = lbl3;
			gadgets[8] = lbl4;
			gadgets[9] = lbl5;
			gadgets[10] = rad1;
			gadgets[11] = rad2;
			gadgets[12] = cnt1;
			gadgets[13] = cnt2;
			
			gadgets[15] = (_gadget*)radgrp;
			gadgets[16] = clockImg;
			lbl->setColor( RGB( 2 , 5 , 15 ) );
			lbl2->setColor( RGB( 30 , 30 , 30 ) );
			lbl3->setColor( RGB( 30 , 30 , 30 ) );
			lbl4->setColor( RGB( 30 , 30 , 30 ) );
			lbl5->setColor( RGB( 30 , 30 , 30 ) );
			lbl->setFont( _system::getFont( "ArialBlack10" ) );
			lbl2->setFont( _system::getFont( "ArialBlack10" ) );
			that->addChild( lbl );
			that->addChild( lbl2 );
			that->addChild( lbl3 );
			that->addChild( lbl4 );
			that->addChild( lbl5 );
			that->addChild( rad1 );
			that->addChild( rad2 );
			that->addChild( cnt1 );
			that->addChild( cnt2 );
			
			// Refresh Clock-Image
			cnt1->handleEvent( _event( onChange ) );
			
			// Then add it
			that->addChild( clockImg );
			
			break;
		}
		case 3:
		{
			// Create Label with shadow
			_label* lbl = new _label( 14 , 34 , _system::getLocalizedString("lbl_profile") );
			_label* lbl2 = new _label( 13 , 33 , _system::getLocalizedString("lbl_profile") );
			_label* lbl3 = new _label( 20 , 60 , _system::getLocalizedString("txt_name") );
			_label* lbl4 = new _label( 20 , 90 , _system::getLocalizedString("txt_profile_icon") );
			_textbox* txtName = new _textbox( 21 , 70 , 80 , profileName , _style::storeInt( 4 ) );
			txtName->registerEventHandler( onChange , new _staticCallback( setupHandler ) );
			
			_imagegadget* image1 = new _imagegadget( 22 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/butterflyl.png" ) ) , _style::storeInt( 1 ) );
			_imagegadget* image2 = new _imagegadget( 42 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/guitarl.png" ) ) , _style::storeInt( 2 ) );
			_imagegadget* image3 = new _imagegadget( 62 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/horsesl.png" ) ) , _style::storeInt( 3 ) );
			_imagegadget* image4 = new _imagegadget( 82 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/red flowerl.png" ) ) , _style::storeInt( 4 ) );
			_imagegadget* image5 = new _imagegadget( 102 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/balll.png" ) ) , _style::storeInt( 5 ) );
			_imagegadget* image6 = new _imagegadget( 122 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/dripl.png" ) ) , _style::storeInt( 6 ) );
			_imagegadget* image7 = new _imagegadget( 142 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/beachl.png" ) ) , _style::storeInt( 7 ) );
			_imagegadget* image8 = new _imagegadget( 162 , 102 , _user::getUserLogoFromImage( _user::getUserImage( "%APPDATA%/astronautl.png" ) ) , _style::storeInt( 8 ) );
			gadgets[5] = lbl;
			gadgets[6] = lbl2;
			gadgets[7] = lbl3;
			gadgets[8] = lbl4;
			gadgets[9] = txtName;
			gadgets[10] = image1;
			gadgets[11] = image2;
			gadgets[12] = image3;
			gadgets[13] = image4;
			gadgets[14] = image5;
			gadgets[15] = image6;
			gadgets[16] = image7;
			gadgets[17] = image8;
			for( int i = 10 ; i < 18 ; i++ )
			{
				gadgets[i]->registerEventHandler( refresh , new _staticCallback( setupHandler ) );
				gadgets[i]->registerEventHandler( onFocus , new _staticCallback( setupHandler ) );
			}
			lbl->setColor( RGB( 2 , 5 , 15 ) );
			lbl2->setColor( RGB( 30 , 30 , 30 ) );
			lbl3->setColor( RGB( 30 , 30 , 30 ) );
			lbl4->setColor( RGB( 30 , 30 , 30 ) );
			lbl->setFont( _system::getFont( "ArialBlack10" ) );
			lbl2->setFont( _system::getFont( "ArialBlack10" ) );
			that->addChild( lbl );
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

_dialog dia;

//int hello ( int i )
//{
//	printf("Dialog Finish: %d\n",i);
//	printf("Str-Value: %s\n",dia.getStrResult().c_str());
//}

void _systemController::loginPage()
{
	// After the Boot up finishes,
	// see if this is a new Setup
	//if( _system::_registry_->readIndex( "_global_" , "firstTimeUse" ).length() )
	//{
	//	_systemController::changeState( _systemState::setup );
	//	return;
	//}
	
	// Clean up
	_system::deleteGadgetHost();
	_system::deleteKeyboard();
	
	_system::_gadgetHost_ = new _startupScreen( _system::_bgIdBack_ );
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ );
	
	_system::getBuiltInProgram( "explorer.exe" )->execute({{"path","/LUA"}});
	
	//_dialog::errorDialog( dia , "Evil Monkeys!!!" , "Error" , "Dismiss!" , "Retry" );
	//dia.execute();
	//dia.onExit( new _staticCallback( & hello ) );
	
	//_direntry entry = _direntry("/LUA/Noel.lua");
	//entry.execute();
}

void _systemController::bootupPage()
{	
	// Clean up	
	_system::deleteGadgetHost();
	_system::deleteKeyboard();
	
	// Create BootupScreen
	_system::_gadgetHost_ = new _bootupScreen( _system::_bgIdBack_ );
	
	static _animation anim = _animation( 0 , 1 , 2000 );
	anim.finish( 
		new _inlineCallback(
			static_cast<function<void()>>
				( [&](){ _systemController::changeState( _systemState::desktop ); } )
		)
	);
	anim.start();
}

void _systemController::desktopPage()
{
	// Clean up	
	_system::deleteGadgetHost();
	_system::deleteKeyboard();
	
	// Create BootupScreen
	_system::_gadgetHost_ = new _windows( _system::_bgIdBack_ );
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ );
	
	_system::getBuiltInProgram( "explorer.exe" )->execute({{"path","/LUA"}});
}


_systemController::_systemState 	
				_systemController::state = _systemController::_systemState::empty; // I know, that one is quite long ;)
bool 			_systemController::dirty = false;