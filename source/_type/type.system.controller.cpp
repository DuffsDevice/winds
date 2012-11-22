#include "_type/type.system.h"
#include "_gadget/gadget.windows.h"
#include "_gadget/gadget.select.h"
#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.startupScreen.h"
#include "_gadget/gadget.bootupScreen.h"
#include "_gadget/gadget.actionButton.h"
#include <nds/interrupts.h>

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
	}	
}

void _systemController::setupPage()
{
	// Clean up
	_system::deleteGadgetHost();
	_system::deleteKeyboard();
	
	_system::_gadgetHost_ = new _startupScreen( _system::_bgIdBack_ );
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ , 117 );
	
	_system::_gadgetHost_->registerEventHandler( _internal_ , _systemController::setupHandler );
	_system::_gadgetHost_->triggerEvent( _event( _internal_ ) );
}

_callbackReturn _systemController::setupHandler( _event e )
{
	static int state = 0;
	static _gadget* gadgets[10] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
	
	_gadget* that = e.getGadget();
	
	if( that->getType() != _gadgetType::screen )
	{
		if( that->getType() == _gadgetType::selectbox )
		{
			_system::setLanguage( (_language)e.getGadget<_select>()->getIntValue() );
			_system::_gadgetHost_->triggerEvent( _event( _internal_ ) );
			return handled;
		}
		if( that->getStyle().data == -1 )
		{
			if( state > 0 )
			{
				state--;
				_system::_gadgetHost_->triggerEvent( _event( _internal_ ) );
			}
		}
		else if( state < 2 )
		{
			state++;
			_system::_gadgetHost_->triggerEvent( _event( _internal_ ) );
		}
		return handled;
	}
	
	for( int i = 0; i < 10 ; i++ )
		if( gadgets[i] )
		{
			delete gadgets[i];
			gadgets[i] = 0;
		}
	
	// Standard
	if( state != 0 )
	{
		_gadget* btnPrev = new _actionButton( _actionButtonType::prev , 4 , 176 , _style::storeData( -1 ) );
		_label* prev = new _label( 50 , 9 , 17 , 177 , _system::getLocalizedString("lbl_prev") , _style::storeData( -1 ) );
		prev->setColor( RGB( 30 , 30 , 30 ) );
		prev->setAlign( _align::left );
		prev->registerEventHandler( mouseClick , setupHandler );
		btnPrev->registerEventHandler( onAction , setupHandler );
		
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
	
	_gadget* btnNext = new _actionButton( _actionButtonType::next , 240 , 176 , _style::storeData( 1 ) );
	_label* next = new _label( 50 , 9 , 188 , 177 , _system::getLocalizedString("lbl_next") , _style::storeData( 1 ) );
	gadgets[0] = btnNext;
	gadgets[2] = next;
	next->setColor( RGB( 30 , 30 , 30 ) );
	next->setAlign( _align::right );
	
	// Set Handler
	next->registerEventHandler( mouseClick , setupHandler );
	btnNext->registerEventHandler( onAction , setupHandler );
	that->addChild( btnNext );
	that->addChild( next );
	
	switch( state )
	{
		case 0:
		{
			_label* lbl = new _label( 79 , 52 , _system::getLocalizedString("lbl_choose_language") );
			_select* slc = new _select( 90 , 5 , 78 , 60 , { { 1 , "English" } , { 2 , "Français" } , { 3 , "Deutsch" } , { 4 , "Italiano" } , { 5 , "Español" } } );
			slc->setIntValue( (_u8)_system::getLanguage() );
			slc->registerEventHandler( onAction , setupHandler );
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
			_label* lbl = new _label( 14 , 34 , _system::getLocalizedString("lbl_profile") );
			_label* lbl2 = new _label( 13 , 33 , _system::getLocalizedString("lbl_profile") );
			_label* lbl3 = new _label( 20 , 60 , _system::getLocalizedString("txt_name") );
			_label* lbl4 = new _label( 20 , 90 , _system::getLocalizedString("txt_profile_icon") );
			_textbox* txtName = new _textbox( 18 , 70 , 80 , "Admin" );
			gadgets[5] = lbl;
			gadgets[6] = lbl2;
			gadgets[7] = lbl3;
			gadgets[8] = lbl4;
			gadgets[9] = txtName;
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
			break;
		}
	}
	return handled;
}

void _systemController::loginPage()
{
	// After the Boot up finishes,
	// see if this is a new Setup
	if( _system::_registry_->readIndex( "_global_" , "firstTimeUse" ).length() )
	{
		_systemController::changeState( _systemState::setup );
		return;
	}
	
	
	// Clean up
	_system::deleteGadgetHost();
	
	_system::_gadgetHost_ = new _startupScreen( _system::_bgIdBack_ );
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ );
}

void _systemController::bootupPage()
{	
	// Clean up	
	_system::deleteGadgetHost();
	_system::deleteKeyboard();
	
	// Create BootupScreen
	_system::_gadgetHost_ = new _bootupScreen( _system::_bgIdBack_ );
	
	static _animation anim = _animation( 0 , 1 , 2000 );
	anim.finish( [&]( _u32 ){ _systemController::changeState( _systemState::login ); } );
	anim.start();
}

void _systemController::desktopPage()
{
	
}


_systemController::_systemState 	
				_systemController::state = _systemController::_systemState::empty; // I know, that one is quite long ;)
bool 			_systemController::dirty = false;