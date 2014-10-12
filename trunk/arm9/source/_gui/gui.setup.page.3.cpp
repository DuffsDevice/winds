#include <_gui/gui.setup.page.3.h>
#include <_controller/controller.localization.h>
#include <_controller/controller.font.h>

#include <math.h>
#define M_PI		3.14159265358979323846

void _guiSetupPage3::increaseTime()
{
	// If we are not active, we increase the time by one second as this does not do the _clockGadget
	if( !this->getSwitcher().isActive(this) )
		this->systemTime = _time( _u32(this->systemTime) + 1 );
}

_callbackReturn _guiSetupPage3::calendarEditHandler( _event event )
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

_callbackReturn _guiSetupPage3::clockEditHandler( _event event )
{
	_time time = this->imgClock->getTime();
	
	// Set counter values
	this->cntHours->setIntValue( time.get( _timeAttr::hour ) );
	this->cntMinutes->setIntValue( time.get( _timeAttr::minute ) );
	this->cntSeconds->setIntValue( time.get( _timeAttr::second ) );
	
	return handled;
}

_callbackReturn _guiSetupPage3::clockCounterHandler( _event event )
{
	// Activate manual time adjust
	this->radioGroup->setIntValue( 0 );
	
	// Set time
	this->imgClock->setTime( this->cntHours->getIntValue() , this->cntMinutes->getIntValue() , this->cntSeconds->getIntValue() );
	
	return handled;
}

void _guiSetupPage3::create( _gadget* viewParent )
{
	// Title
	this->lblTitle = new _label( 13 , 28 , ignore , ignore , _localizationController::getBuiltInString("lbl_system_preferences") );
	this->lblTitle->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	this->lblTitle->setFont( _fontController::getFont( "ArialBlack13" ) );
	viewParent->addChild( this->lblTitle );
	
	// Adjust the system clock label
	this->lblAdjustSystemClock = new _label( 32 , 48 , ignore , ignore , _localizationController::getBuiltInString("txt_system_clock") );
	this->lblAdjustSystemClock->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	viewParent->addChild( this->lblAdjustSystemClock );
	
	// Auto fetch time from internet label
	this->lblFetchTime1 = new _label( 32 , 142 , ignore , ignore , _localizationController::getBuiltInString("txt_system_clock_auto_fetch_1") );
	this->lblFetchTime1->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	viewParent->addChild( this->lblFetchTime1 );
	
	// Auto fetch time from internet label (line 2)
	this->lblFetchTime2 = new _label( 32 , 152 , ignore , ignore , _localizationController::getBuiltInString("txt_system_clock_auto_fetch_2") );
	this->lblFetchTime2->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	viewParent->addChild( this->lblFetchTime2 );
	
	
	// Create the Clock image...
	this->imgClock = new _clockGadget( 55 , 64 , 50 , 50 , this->systemTime , true );
	this->imgClock->setOuterBgColor( _color::fromHex( 0x5A7EDC ) );
	this->imgClock->setInnerBgColor( _color::fromHex( 0x6082E3 ) );
	this->imgClock->setMarkingsColor( _color::white );
	this->imgClock->setSecondsColor( _color::fromRGB8( 255 , 48 , 53 ) );
	this->imgClock->setMinutesColor( _color::fromRGB( 21 , 24 , 31 ) );
	this->imgClock->setHoursColor( _color::fromRGB( 0 , 6 , 15 ) );
	this->imgClock->setInternalEventHandler( onEdit , make_callback( this , &_guiSetupPage3::clockEditHandler ) );
	viewParent->addChild( this->imgClock );
	
	// ..as well as the counters that modify it
	this->cntHours = new _counter( 38 , 121 , 26 , true , systemTime.get( _timeAttr::hour ) , 23 , 0 );
	this->cntHours->setInternalEventHandler( onEdit , make_callback( this , &_guiSetupPage3::clockCounterHandler ) );
	viewParent->addChild( this->cntHours );
	
	this->cntMinutes = new _counter( 68 , 121 , 26 , true , systemTime.get( _timeAttr::minute ) , 59 , 0 );
	this->cntMinutes->setInternalEventHandler( onEdit , make_callback( this , &_guiSetupPage3::clockCounterHandler ) );
	viewParent->addChild( this->cntMinutes );
	
	this->cntSeconds = new _counter( 98 , 121 , 26 , true , systemTime.get( _timeAttr::second ) , 59 , 0 );
	this->cntSeconds->setInternalEventHandler( onEdit , make_callback( this , &_guiSetupPage3::clockCounterHandler ) );
	viewParent->addChild( this->cntSeconds );
	
	// Two colons between the counters
	this->colon1 = new _label( 65 , 120 , ignore , ignore , ":" );
	this->colon1->setFont( _fontController::getFont("CourierNew10") );
	this->colon1->setColor( _color::white );
	this->colon2 = new _label( 95 , 120 , ignore , ignore , ":" );
	this->colon2->setFont( _fontController::getFont("CourierNew10") );
	this->colon2->setColor( _color::white );
	viewParent->addChild( this->colon1 );
	viewParent->addChild( this->colon2 );
	
	
	// Calendar
	this->calendar = new _calendar( 140 , 62 , 85 , 73 , systemTime , _color::fromHex( 0x6082E3 ) );
	this->calendar->setUserEventHandler( onEdit , make_callback( this , &_guiSetupPage3::calendarEditHandler ) );
	viewParent->addChild( this->calendar );
	
	
	// Radio buttons
	this->radioGroup = new _singleValueGroup<_radio>();
	this->radManual = new _radio( 20 , 47 , this->radioGroup );
	this->radAuto = new _radio( 20 , 141 , this->radioGroup );
	this->radioGroup->enableSelector( this->radManual );
	viewParent->addChild( this->radManual );
	viewParent->addChild( this->radAuto );
}