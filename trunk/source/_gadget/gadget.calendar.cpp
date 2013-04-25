#include "_gadget/gadget.calendar.h"
#include "_type/type.system.h"
#include "func.gridcreator.h"

_calendar::_calendar( _length width , _length height , _coord x , _coord y , _u16 year , _u8 month , _u8 dayOfMonth , _style style )
	: _gadget( _gadgetType::button , width , height , x , y , style )
	, _singleValueGroup<_stickybutton>()
	, curMonth( month )
	, curYear( year )
	, monthLabel( nullptr )
	, leftArrow( nullptr )
	, rightArrow( nullptr )
	, resetButton( nullptr )
	, todayButton( nullptr )
{
	this->selectedDate.set( _timeAttr::year , year , false );
	this->selectedDate.set( _timeAttr::month , month , false );
	this->selectedDate.set( _timeAttr::day , dayOfMonth );
	
	this->registerEventHandler( refresh , new _classCallback( this , &_calendar::handler ) );
	this->registerEventHandler( keyClick , new _classCallback( this , &_calendar::handler ) );
	
	
	//! Allocate sticky-buttons
	for( _u32 curSize = 0; curSize < 31 ; curSize++ )
	{
		_stickybutton* btn = new _stickybutton( 15 , 15 , -5 , -5 , int2string( curSize + 1 ) , _style::storeInt( curSize + 1 ) );
		btn->registerEventHandler( onAction , new _classCallback( this , &_calendar::handler ) );
		// Add to singleValueGroup
		this->addSelector( btn , curSize + 1 );
	}
	
	//! Arrows & month label
	const _font* symbolFt = _system::getFont( "SystemSymbols8" );
	_u8 arrowWidth = this->getArrowWidth();
	_u8 monthSelectorHeight = this->getMonthSelectorHeight();
	
	this->monthLabel	= new _label( this->getWidth() - arrowWidth * 2 - 4 , monthSelectorHeight - 3 , arrowWidth + 2 , 1 , "" );
	this->rightArrow	= new _button( 1 , 1 , 1 , 1 , _string( 1 , glyph::arrowRight ) , _styleAttr() | _styleAttr::mouseClickRepeat );
	this->leftArrow		= new _button( 1 , 1 , 1 , 1 , _string( 1 , glyph::arrowLeft ) , _styleAttr() | _styleAttr::mouseClickRepeat );
	this->todayButton	= new _button( 1 , 1 , 1 , 1 , _string( 1 , glyph::reset ) );
	this->resetButton	= new _button( 1 , 1 , 1 , 1 , _string( 1 , glyph::arrowRotateLeft ) );
	
	this->leftArrow->registerEventHandler( onAction , new _classCallback( this , &_calendar::handler ) );
	this->rightArrow->registerEventHandler( onAction , new _classCallback( this , &_calendar::handler ) );
	this->todayButton->registerEventHandler( onAction , new _classCallback( this , &_calendar::handler ) );
	this->resetButton->registerEventHandler( onAction , new _classCallback( this , &_calendar::handler ) );
	
	// Set Symbol-font
	this->leftArrow->setFont( symbolFt );
	this->rightArrow->setFont( symbolFt );
	this->todayButton->setFont( symbolFt );
	this->resetButton->setFont( symbolFt );
	
	this->addEnhancedChild( this->monthLabel );
	this->addEnhancedChild( this->leftArrow );
	this->addEnhancedChild( this->rightArrow );
	this->addEnhancedChild( this->todayButton );
	this->addEnhancedChild( this->resetButton );
	
	
	// Fill the calendar
	this->populateGUI();
}

void _calendar::selectDate( _u16 year , _u8 month , _u8 dayOfMonth )
{
	bool refreshFlag = false;
	
	if( this->selectedDate.get( _timeAttr::year ) != year || this->selectedDate.get( _timeAttr::month ) != month )
		refreshFlag = true;
	
	this->selectedDate.set( _timeAttr::year , year , false );
	this->selectedDate.set( _timeAttr::month , month , false );
	this->selectedDate.set( _timeAttr::day , dayOfMonth );
	
	if( refreshFlag )
		this->populateGUI();
}

_callbackReturn _calendar::handler( _event event )
{	
	if( event.getType() == refresh )
	{
		_bitmapPort bP = this->getBitmapPort();
		
		if( event.hasClippingRects() )
			bP.addClippingRects( event.getDamagedRects().toRelative( this->getAbsolutePosition() ) );
		else
			bP.normalizeClippingRects();
			
		bP.fill( COLOR_WHITE );
		
		bP.drawHorizontalLine( 0 , this->getMonthSelectorHeight() - 1 , bP.getWidth() , COLOR_BLACK );
	}
	else if( event.getType() == onAction )
	{
		if( event.getGadget() == this->leftArrow )
		{
			if( this->curMonth > 1 )
				this->curMonth--;
			else
			{
				this->curYear--;
				this->curMonth = 12;
			}
		}
		else if( event.getGadget() == this->rightArrow )
		{
			if( this->curMonth < 12 )
				this->curMonth++;
			else
			{
				this->curYear++;
				this->curMonth = 1;
			}
		}
		else if( event.getGadget() == this->todayButton )
		{
			_time now = _time::now();
			
			_u8 tempM = this->curMonth;
			_u16 tempY = this->curYear;
			this->curYear = now.get( _timeAttr::year );
			this->curMonth = now.get( _timeAttr::month );
			
			if( tempM == this->curMonth && tempY == this->curYear )
				return handled;
		}
		else if( event.getGadget() == this->resetButton )
		{
			_u8 tempM = this->curMonth;
			_u16 tempY = this->curYear;
			
			this->curYear = this->selectedDate.get( _timeAttr::year );
			this->curMonth = this->selectedDate.get( _timeAttr::month );
			
			if( tempM == this->curMonth && tempY == this->curYear )
				return handled;
		}
		else
		{
			this->selectedDate = _time::date( this->curYear , this->curMonth , event.getGadget()->getStyle().val );
			this->triggerEvent( onChange );
			
			return handled;
		}
		
		// Refresh!
		this->populateGUI();
	}
	else
		this->bubbleRefresh( true );
	
	return handled;
}


_u8 _calendar::getWeeksInMonth( _time firstDay , _u32 daysInMonth ) const
{
	// Temp...
	_u8 weekday = firstDay.get( _timeAttr::dayOfWeek );
	_u8 used = weekday + daysInMonth;

    return ceil( float(used) / 7 );
}


void _calendar::populateGUI()
{
	//! Strings!
	//string months[] = { "Jan" , "Feb" , "Mär" , "Apr" , "Mai" , "Jun" , "Jul" , "Aug" , "Sep" , "Okt" , "Nov" , "Dez" };
	//string weekdays[] = { "Mo" , "Di" , "Mi" , "Do" , "Fr" , "Sa" , "So" };
	//!
	
	
	//! Extract some information from the date
	_u16 selectedYear = this->selectedDate.get( _timeAttr::year );
	_u16 selectedMonth = this->selectedDate.get( _timeAttr::month );
	_u16 selectedDayOfMonth = this->selectedDate.get( _timeAttr::day );
	
	_time now = _time::now();
	
	_u16 todayYear = now.get( _timeAttr::year );
	_u16 todayMonth = now.get( _timeAttr::month );
	_u16 todayDayOfMonth = now.get( _timeAttr::day );
	
	//! Compute excesses
	_time	firstDateInMonth = _time::date( this->curYear , this->curMonth , 1 );
	_time	lastDateInMonth = firstDateInMonth;
	lastDateInMonth.add( _timeAttr::month , 1 );
	
	
	//! Compute number of days
	_u32 daysInMonth = ( _int( lastDateInMonth ) - _int( firstDateInMonth ) ) / 86400 /* Seconds per Day */;
	
	
	//! Month Selector
	int monthSelectorHeight = this->getMonthSelectorHeight();
	
	// Sizes of each table element
	vector<int> rowHeights = computeGrid( this->getHeight() - 1 - monthSelectorHeight , this->getWeeksInMonth( firstDateInMonth , daysInMonth ) );
	vector<int> colWidths = computeGrid( this->getWidth() - 1 , 7 );
	
	//! Choose font to be used
	const _font* ft;
	
	if( rowHeights[0] > 12 && colWidths[0] > 15 )
		ft = _system::getFont( "CourierNew10" );
	else
		ft = _system::getFont( "System7" );
	
	
	//! Some counter Variables
	int x = 0;
	int y = monthSelectorHeight;
	int curRow = 0;
	int arrowWidth = this->getArrowWidth();
	
	
	//! Process Offset for weekdays
	int	weekday = firstDateInMonth.get( _timeAttr::dayOfWeek );
	
	int i = 0;
	while( i < weekday )
		x += colWidths[i++];
	
	//! Arrows & month label
	this->leftArrow->setDimensions( _rect( 1 , 1 , arrowWidth , monthSelectorHeight - 3 ) );
	this->rightArrow->setDimensions( _rect( this->getWidth() - 1 - arrowWidth , 1 , arrowWidth , monthSelectorHeight - 3 ) );
	this->todayButton->setDimensions( _rect( arrowWidth + 2 , 1 , 9 , monthSelectorHeight - 3 ) );
	this->resetButton->setDimensions( _rect( this->getWidth() - 12 - arrowWidth , 1 , 10 , monthSelectorHeight - 3 ) );
	this->rightArrow->setDimensions( _rect( this->getWidth() - 1 - arrowWidth , 1 , arrowWidth , monthSelectorHeight - 3 ) );
	
	this->monthLabel->setStrValue( _system::getLocalizedMonth( this->curMonth - 1 ).substr( 0 , 3 ) + " " + int2string( this->curYear ) );
	this->monthLabel->setDimensions( _rect( arrowWidth + 2 , 1 , this->getWidth() - arrowWidth * 2 - 4 , monthSelectorHeight - 3 ) );
	this->monthLabel->setFont( ft );
	
	// Remove Buttons again
	this->removeChildren();
	
	//! Move days to right position!
	for( _u8 curDay = 1; curDay <= daysInMonth ; curDay++ )
	{
		_stickybutton* btn = this->registeredSelectors[curDay];
		
		btn->setDimensions( _rect( x + 1 , y + 1 , colWidths[weekday] - 1 , rowHeights[curRow] - 1 ) );
		btn->setFont( ft );
		btn->setAutoSelect( false );
		
		// Add the button
		this->addChild( btn );
		
		x += colWidths[weekday++];
		if( weekday >= 7 )
		{
			y += rowHeights[curRow++];
			x = 0;
			weekday = 0;
		}
	}
	
	
	//! Enable the selected stickybutton
	if( this->curMonth == selectedMonth && this->curYear == selectedYear )
		this->setIntValue( selectedDayOfMonth );
	else
		this->setIntValue( -1 );
	
	
	//! Make 'today' appear blue
	if( todayYear == this->curYear && this->curMonth == todayMonth )
	{
		selectorListType::iterator it = this->registeredSelectors.find( todayDayOfMonth );
		if( it != this->registeredSelectors.end() )
			it->second->setAutoSelect( true );
	}
}

_calendar::~_calendar()
{
	this->removeChildren();
	this->removeEnhancedChildren( true );
	
	// Free _stickybuttons
	for( selectorListType::value_type btn : this->registeredSelectors )
		delete btn.second;
}