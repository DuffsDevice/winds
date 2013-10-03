#include "_scenario/scenario.setup.page3.h"
#include <math.h>
#define M_PI		3.14159265358979323846

void _scSetupPage3::increaseTime()
{
	// If we are active, we increase the counter that then will then update the time automatically
	if( this->getSwitcher().isActive(this) )
	{
		// Check seconds overflow
		if( this->cntSeconds->getIntValue() == 59 )
		{
			// Check minutes overflow
			if( this->cntMinutes->getIntValue() == 59 )
			{
				this->cntHours->increase();
				this->systemTime.set( _timeAttr::hour , this->cntHours->getIntValue() , false ); // Update system clock
			}
			this->cntMinutes->increase();
			this->systemTime.set( _timeAttr::minute , this->cntMinutes->getIntValue() , false ); // Update system clock
		}
		this->cntSeconds->increase();
		this->systemTime.set( _timeAttr::second , this->cntSeconds->getIntValue() , false ); // Update system clock
		
		// Update clock image
		this->imgClock->update();
	}
	else
		this->systemTime = _time( _u32(this->systemTime) + 1 );
}

_callbackReturn _scSetupPage3::calendarEditHandler( _event event )
{
	// Activate manual time adjust
	this->radioGroup->setIntValue( 0 );
	
	// Fetch date from calendar
	_time calTime = this->calendar->getSelectedDate();
	
	// Set system date
	this->systemTime.set( _timeAttr::day , calTime.get( _timeAttr::day ) , false );
	this->systemTime.set( _timeAttr::month , calTime.get( _timeAttr::month ) , false );
	this->systemTime.set( _timeAttr::year , calTime.get( _timeAttr::year ) , false );
	
	return handled;
}

_callbackReturn _scSetupPage3::clockEditHandler( _event event )
{
	// Activate manual time adjust
	this->radioGroup->setIntValue( 0 );
	
	// Set system time
	if( event.getGadget() == this->cntSeconds )
		this->systemTime.set( _timeAttr::second , this->cntSeconds->getIntValue() , false );
	else if( event.getGadget() == this->cntMinutes )
		this->systemTime.set( _timeAttr::minute , this->cntMinutes->getIntValue() , false );
	else if( event.getGadget() == this->cntHours )
		this->systemTime.set( _timeAttr::hour , this->cntHours->getIntValue() , false );
	
	// Update clock image
	this->imgClock->update();
	
	return handled;
}

void _scSetupPage3::create( _gadget* viewParent )
{
	// Title
	this->lblTitle = new _label( ignore , ignore , 13 , 28 , _system::getLocalizedString("lbl_system_preferences") );
	this->lblTitle->setColor( RGB( 30 , 30 , 30 ) );
	this->lblTitle->setFont( _system::getFont( "ArialBlack13" ) );
	viewParent->addChild( this->lblTitle );
	
	// Adjust the system clock label
	this->lblAdjustSystemClock = new _label( ignore , ignore , 32 , 48 , _system::getLocalizedString("txt_system_clock") );
	this->lblAdjustSystemClock->setColor( RGB( 30 , 30 , 30 ) );
	viewParent->addChild( this->lblAdjustSystemClock );
	
	// Auto fetch time from internet label
	this->lblFetchTime1 = new _label( ignore , ignore , 32 , 142 , _system::getLocalizedString("txt_system_clock_auto_fetch_1") );
	this->lblFetchTime1->setColor( RGB( 30 , 30 , 30 ) );
	viewParent->addChild( this->lblFetchTime1 );
	
	// Auto fetch time from internet label (line 2)
	this->lblFetchTime2 = new _label( ignore , ignore , 32 , 152 , _system::getLocalizedString("txt_system_clock_auto_fetch_2") );
	this->lblFetchTime2->setColor( RGB( 30 , 30 , 30 ) );
	viewParent->addChild( this->lblFetchTime2 );
	
	
	// Create the Clock image...
	this->imgClock = new _imagegadget( 55 , 64 , _bitmap( 51 , 51 ) );
	this->imgClock->setInternalEventHandler( onDraw , make_callback( this , &_scSetupPage3::drawClockImageHandler ) );
	this->imgClock->setInternalEventHandler( onUpdate , _gadgetHelpers::eventForwardRefresh() );
	this->imgClock->redraw();
	viewParent->addChild( this->imgClock );
	
	// ..as well as the counters that modify it
	this->cntHours = new _counter( 38 , 121 , 26 , true , systemTime.get( _timeAttr::hour ) , 23 , 0 );
	this->cntHours->setInternalEventHandler( onEdit , make_callback( this , &_scSetupPage3::clockEditHandler ) );
	viewParent->addChild( this->cntHours );
	
	this->cntMinutes = new _counter( 68 , 121 , 26 , true , systemTime.get( _timeAttr::minute ) , 59 , 0 );
	this->cntMinutes->setInternalEventHandler( onEdit , make_callback( this , &_scSetupPage3::clockEditHandler ) );
	viewParent->addChild( this->cntMinutes );
	
	this->cntSeconds = new _counter( 98 , 121 , 26 , true , systemTime.get( _timeAttr::second ) , 59 , 0 );
	this->cntSeconds->setInternalEventHandler( onEdit , make_callback( this , &_scSetupPage3::clockEditHandler ) );
	viewParent->addChild( this->cntSeconds );
	
	// Two colons between the counters
	this->colon1 = new _label( ignore , ignore , 65 , 120 , ":" );
	this->colon1->setFont( _system::getFont("CourierNew10") );
	this->colon1->setColor( COLOR_WHITE );
	this->colon2 = new _label( ignore , ignore , 95 , 120 , ":" );
	this->colon2->setFont( _system::getFont("CourierNew10") );
	this->colon2->setColor( COLOR_WHITE );
	viewParent->addChild( this->colon1 );
	viewParent->addChild( this->colon2 );
	
	
	// Calendar
	this->calendar = new _calendar( 85 , 73 , 140 , 62 , systemTime , RGBHEX( 0x6082E3 ) );
	this->calendar->setUserEventHandler( onEdit , make_callback( this , &_scSetupPage3::calendarEditHandler ) );
	viewParent->addChild( this->calendar );
	
	
	// Radio buttons
	this->radioGroup = new _singleValueGroup<_radio>();
	this->radManual = new _radio( 20 , 47 , this->radioGroup );
	this->radAuto = new _radio( 20 , 141 , this->radioGroup );
	this->radioGroup->enableSelector( this->radManual );
	viewParent->addChild( this->radManual );
	viewParent->addChild( this->radAuto );
}

_callbackReturn _scSetupPage3::drawClockImageHandler( _event event )
{
	_int seconds = systemTime.get( _timeAttr::second );
	_int minutes = systemTime.get( _timeAttr::minute );
	_int hours = systemTime.get( _timeAttr::hour );
	
	// Get bitmap Port
	_bitmapPort bP = this->imgClock->getBitmapPort( event );
	
	// Reset Clock-Image
	bP.fill( RGBHEX( 0x5A7EDC ) );
	bP.drawFilledCircle( 25 , 25 , 25 , RGBHEX( 0x6082E3 ) );
	
	// Dots
	for( float i = 0 ; i < 4 ; i+= 1 )
	{
		int c = cos( i * 30 / 180 * M_PI ) * 25;
		int s = sin( i * 30 / 180 * M_PI ) * 25;
		int c2 = c * 25/24;
		int s2 = s * 25/24;
		bP.drawPixel( 25 - c , 25 - s , COLOR_WHITE );
		bP.drawPixel( 25 + c , 25 - s , COLOR_WHITE );
		bP.drawPixel( 25 - c , 25 + s , COLOR_WHITE );
		bP.drawPixel( 25 + c , 25 + s , COLOR_WHITE );
		bP.drawPixel( 25 - c2 , 25 - s2 , COLOR_WHITE );
		bP.drawPixel( 25 + c2 , 25 - s2 , COLOR_WHITE );
		bP.drawPixel( 25 - c2 , 25 + s2 , COLOR_WHITE );
		bP.drawPixel( 25 + c2 , 25 + s2 , COLOR_WHITE );
	}
	
	// Second
	bP.drawLine( 25 , 25
		, 25 - cos( float( seconds + 15 ) * 6 / 180 * M_PI ) * 23 + 0.5
		, 25 - sin( float( seconds + 15 ) * 6 / 180 * M_PI ) * 23 + 0.5
		, RGB255( 255 , 48 , 53 )
	);
	
	float minAngleInRad = ( float( minutes + 15 ) * 6 + float( seconds ) / 10 ) / 180 * M_PI;
	
	// Minute
	bP.drawLine( 25 , 25
		, 25 - cos( float( minAngleInRad ) ) * 20 + 0.5
		, 25 - sin( float( minAngleInRad ) ) * 20 + 0.5
		, RGB( 21 , 24 , 31 )
	);
	
	float hrAngleInRad = ( float( hours + 3 ) * 30 + float( minutes ) / 2 ) / 180 * M_PI;
	
	// Hour
	bP.drawLine( 25 , 25 
		, 25 - cos( hrAngleInRad ) * 12 + 0.5
		, 25 - sin( hrAngleInRad ) * 12 + 0.5
		, RGB( 0 , 6 , 15 )
	);
	
	return handled;
}