#include "_scenario/scenario.setup.page3.h"
#include <math.h>
#define M_PI		3.14159265358979323846

void _scSetupPage3::increaseTime()
{
	// If we are not active, we increase the time by one second as this does not do the _clockGadget
	if( !this->getSwitcher().isActive(this) )
		this->systemTime = _time( _u32(this->systemTime) + 1 );
}

_callbackReturn _scSetupPage3::calendarEditHandler( _event event )
{
	// Activate manual time adjust
	this->radioGroup->setIntValue( 0 );
	
	// Fetch date from calendar
	_time calTime = this->calendar->getSelectedDate();
	
	// Set system date
	this->systemTime.set( _timeAttr::day , calTime.get( _timeAttr::day ) );
	this->systemTime.set( _timeAttr::month , calTime.get( _timeAttr::month ) );
	this->systemTime.set( _timeAttr::year , calTime.get( _timeAttr::year ) );
	
	return handled;
}

_callbackReturn _scSetupPage3::clockEditHandler( _event event )
{
	_time time = this->imgClock->getTime();
	
	// Set counter values
	this->cntHours->setIntValue( time.get( _timeAttr::hour ) );
	this->cntMinutes->setIntValue( time.get( _timeAttr::minute ) );
	this->cntSeconds->setIntValue( time.get( _timeAttr::second ) );
	
	return handled;
}

_callbackReturn _scSetupPage3::clockCounterHandler( _event event )
{
	// Activate manual time adjust
	this->radioGroup->setIntValue( 0 );
	
	// Set time
	this->imgClock->setTime( this->cntHours->getIntValue() , this->cntMinutes->getIntValue() , this->cntSeconds->getIntValue() );
	
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
	this->imgClock = new _clockgadget( 50 , 50 , 55 , 64 , this->systemTime , true );
	this->imgClock->setOuterBgColor( RGBHEX( 0x5A7EDC ) );
	this->imgClock->setInnerBgColor( RGBHEX( 0x6082E3 ) );
	this->imgClock->setMarkingsColor( COLOR_WHITE );
	this->imgClock->setSecondsColor( RGB255( 255 , 48 , 53 ) );
	this->imgClock->setMinutesColor( RGB( 21 , 24 , 31 ) );
	this->imgClock->setHoursColor( RGB( 0 , 6 , 15 ) );
	this->imgClock->setInternalEventHandler( onEdit , make_callback( this , &_scSetupPage3::clockEditHandler ) );
	viewParent->addChild( this->imgClock );
	
	// ..as well as the counters that modify it
	this->cntHours = new _counter( 38 , 121 , 26 , true , systemTime.get( _timeAttr::hour ) , 23 , 0 );
	this->cntHours->setInternalEventHandler( onEdit , make_callback( this , &_scSetupPage3::clockCounterHandler ) );
	viewParent->addChild( this->cntHours );
	
	this->cntMinutes = new _counter( 68 , 121 , 26 , true , systemTime.get( _timeAttr::minute ) , 59 , 0 );
	this->cntMinutes->setInternalEventHandler( onEdit , make_callback( this , &_scSetupPage3::clockCounterHandler ) );
	viewParent->addChild( this->cntMinutes );
	
	this->cntSeconds = new _counter( 98 , 121 , 26 , true , systemTime.get( _timeAttr::second ) , 59 , 0 );
	this->cntSeconds->setInternalEventHandler( onEdit , make_callback( this , &_scSetupPage3::clockCounterHandler ) );
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