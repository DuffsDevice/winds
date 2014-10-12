#include <_gadget/gadget.calendar.h>
#include <_type/type.font.glyphs.h>
#include <_type/type.color.h>
#include <_controller/controller.font.h>
#include <_controller/controller.localization.h>
#include <_func/func.gridcreator.h>

_calendar::_calendar( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _u16 year , _u8 month , _u8 dayOfMonth , _color bgColor , _style&& style )
	: _gadget( _gadgetType::button , x , y , width , height , (_style&&)style )
	, _singleValueGroup<_stickyButton>()
	, curMonth( month )
	, curYear( year )
	, bgColor( bgColor )
	, monthLabel( nullptr )
	, leftArrow( nullptr )
	, rightArrow( nullptr )
	, resetButton( nullptr )
	, todayButton( nullptr )
{
	this->selectedDate.set( _timeAttr::year , year );
	this->selectedDate.set( _timeAttr::month , month );
	this->selectedDate.set( _timeAttr::day , dayOfMonth );
	
	//! Allocate sticky-buttons
	for( _u32 curSize = 0; curSize < 31 ; curSize++ )
	{
		_stickyButton* btn = new _stickyButton( 15 , 15 , -5 , -5 , int2string( curSize + 1 ) , _style::storeInt( curSize + 1 ) );
		btn->setInternalEventHandler( onMouseClick , make_callback( this , &_calendar::clickHandler ) );
		// Add to singleValueGroup
		this->addSelector( btn , curSize + 1 );
	}
	
	//! Arrows & month label
	_fontHandle symbolFt = _fontController::getFont( "SystemSymbols8" );
	_u8 arrowWidth = this->getArrowWidth();
	_u8 monthSelectorHeight = this->getMonthSelectorHeight();
	
	this->monthLabel	= new _label( this->getWidth() - arrowWidth * 2 - 4 , monthSelectorHeight - 3 , arrowWidth + 2 , 1 , "" );
	this->rightArrow	= new _button( 1 , 1 , 1 , 1 , string( 1 , _glyph::arrowRight ) , _style::clickRepeat );
	this->leftArrow		= new _button( 1 , 1 , 1 , 1 , string( 1 , _glyph::arrowLeft ) , _style::clickRepeat );
	this->todayButton	= new _button( 1 , 1 , 1 , 1 , string( 1 , _glyph::reset ) );
	this->resetButton	= new _button( 1 , 1 , 1 , 1 , string( 1 , _glyph::arrowRotateLeft ) );
	
	// Register my handler
	this->leftArrow->setInternalEventHandler( onMouseClick , make_callback( this , &_calendar::clickHandler ) );
	this->rightArrow->setInternalEventHandler( onMouseClick , make_callback( this , &_calendar::clickHandler ) );
	this->todayButton->setInternalEventHandler( onMouseClick , make_callback( this , &_calendar::clickHandler ) );
	this->resetButton->setInternalEventHandler( onMouseClick , make_callback( this , &_calendar::clickHandler ) );
	
	// Update (that will register all buttons inside the _calendar)
	this->setInternalEventHandler( onUpdate , make_callback( this , &_calendar::updateHandler ) );
	this->updateNow();
	
	
	this->setInternalEventHandler( onDraw , make_callback( &_calendar::refreshHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( this , &_calendar::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( this , &_calendar::keyHandler ) );
	
	// Set Symbol-font
	this->leftArrow->setFont( symbolFt );
	this->rightArrow->setFont( symbolFt );
	this->todayButton->setFont( symbolFt );
	this->resetButton->setFont( symbolFt );
	
	this->monthLabel->setAlign( _align::center );
	
	// Add Children!
	this->addEnhancedChild( this->monthLabel );
	this->addEnhancedChild( this->leftArrow );
	this->addEnhancedChild( this->rightArrow );
	this->addEnhancedChild( this->todayButton );
	this->addEnhancedChild( this->resetButton );
	
	// Fill the calendar
	this->redraw();
}

void _calendar::selectDate( _u16 year , _u8 month , _u8 dayOfMonth )
{
	if( this->selectedDate.get( _timeAttr::day ) == dayOfMonth && this->selectedDate.get( _timeAttr::year ) == year && this->selectedDate.get( _timeAttr::month ) == month )
		return;
	
	this->selectedDate.set( _timeAttr::year , year );
	this->selectedDate.set( _timeAttr::month , month );
	this->selectedDate.set( _timeAttr::day , dayOfMonth );
	
	// refresh
	this->update();
}

_callbackReturn _calendar::keyHandler( _event event )
{
	switch( event.getKeyCode() )
	{
		case _key::right:
			this->selectedDate.add( _timeAttr::day , 1 );
			break;
		case _key::left:
			this->selectedDate.add( _timeAttr::day , -1 );
			break;
		case _key::down:
			this->selectedDate.add( _timeAttr::day , 7 );
			break;
		case _key::up:
			this->selectedDate.add( _timeAttr::day , -7 );
			break;
		default:
			return use_default;
	}
	
	// Set active month
	this->curMonth = this->selectedDate.get( _timeAttr::month );
	this->curYear = this->selectedDate.get( _timeAttr::year );
	
	// Update
	this->update();
	
	// Notify date has changed
	this->triggerEvent( onEdit );
	
	return handled;
}

_callbackReturn _calendar::refreshHandler( _event event )
{
	_calendar* that = event.getGadget<_calendar>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Draw background
	bP.fill( that->bgColor );
	
	// Draw Month label
	bP.drawHorizontalLine( 0 , that->getMonthSelectorHeight() - 1 , bP.getWidth() , _color(that->bgColor).getL() > 70 ? _color::black : _color::white );
	
	return use_default;
}

// For 'onAction'-events
_callbackReturn _calendar::clickHandler( _event event )
{
	if( event.getGadget() == this->leftArrow )
		this->decreaseMonth();
	else if( event.getGadget() == this->rightArrow )
		this->increaseMonth();
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
		// only click onto one of the buttons
		this->selectedDate = _time::date( this->curYear , this->curMonth , event.getGadget()->getStyle().val );
		
		// Notify date has changed
		this->triggerEvent( onEdit );
		
		return handled;
	}
	
	// Refresh buttons
	this->update();
	
	return handled;
}


_u8 _calendar::getWeeksInMonth( _time firstDay , _u32 daysInMonth ) const
{
	// Temp...
	_u8 weekday = firstDay.get( _timeAttr::dayOfWeek ) - 1;
	_u8 used = weekday + daysInMonth;

    return ceil( float(used) / 7 );
}


_callbackReturn _calendar::updateHandler( _event event )
{
	//! Strings!
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
	
	// Do we only have to adjust the selected Date?
	if( this->lastCurMonth == this->curMonth && this->lastCurYear == this->curYear )
	{
		//! Enable the selected stickybutton
		if( this->curMonth == selectedMonth && this->curYear == selectedYear )
			_singleValueGroup::setIntValue( selectedDayOfMonth );
		else
			_singleValueGroup::setIntValue( -1 );
		
		return handled;
	}
	
	// Indicate a complete update of the month and the year
	this->lastCurMonth = this->curMonth;
	this->lastCurYear = this->curYear;
	
	// Compute boundaries
	_time	firstDateInMonth = _time::date( this->curYear , this->curMonth , 1 );
	_time	lastDateInMonth = firstDateInMonth;
	lastDateInMonth.add( _timeAttr::month , 1 );
	
	
	// Compute number of days
	_u32 daysInMonth = ( _int( lastDateInMonth ) - _int( firstDateInMonth ) ) / 86400 /* Seconds per Day */;
	
	
	// Store Height of the Month Selector and the title in temporary
	int monthSelectorHeight = this->getMonthSelectorHeight();
	
	// Sizes of each table element
	_vector<int> rowHeights = computeGrid( this->getHeight() - 1 - monthSelectorHeight , this->getWeeksInMonth( firstDateInMonth , daysInMonth ) );
	_vector<int> colWidths = computeGrid( this->getWidth() - 1 , 7 );
	
	// Choose font to be used
	_fontHandle ft;
	
	if( rowHeights[0] > 12 && colWidths[0] > 15 )
		ft = _fontController::getFont( "CourierNew10" );
	else
		ft = _fontController::getFont( "System7" );
	
	
	// Some counter Variables
	int x = 0;
	int y = monthSelectorHeight;
	int curRow = 0;
	int arrowWidth = this->getArrowWidth();
	
	
	// Process Offset for weekdays (Mon=1, Sun=7)
	int	weekday = firstDateInMonth.get( _timeAttr::dayOfWeek ) - 1;
	
	int i = 0;
	while( i < weekday )
		x += colWidths[i++];
	
	// Arrows & month label
	this->leftArrow->setDimensions( _rect( 1 , 1 , arrowWidth , monthSelectorHeight - 3 ) );
	this->rightArrow->setDimensions( _rect( this->getWidth() - 1 - arrowWidth , 1 , arrowWidth , monthSelectorHeight - 3 ) );
	this->todayButton->setDimensions( _rect( arrowWidth + 2 , 1 , 9 , monthSelectorHeight - 3 ) );
	this->resetButton->setDimensions( _rect( this->getWidth() - 12 - arrowWidth , 1 , 10 , monthSelectorHeight - 3 ) );
	this->rightArrow->setDimensions( _rect( this->getWidth() - 1 - arrowWidth , 1 , arrowWidth , monthSelectorHeight - 3 ) );
	
	this->monthLabel->setStrValue( _localizationController::getMonth( this->curMonth - 1 ).substr( 0 , 3 ) + " " + int2string( this->curYear ) );
	this->monthLabel->setDimensions( _rect( arrowWidth + 2 , 1 , this->getWidth() - arrowWidth * 2 - 4 , monthSelectorHeight - 3 ) );
	this->monthLabel->setFont( ft );
	this->monthLabel->setColor( _color(this->bgColor).getL() > 70 ? _color::black : _color::white );
	
	
	// Remove Buttons again
	this->removeChildren();
	
	// Flag indicating that there is not enough room for spaces in between the buttons
	bool noSpaces = false;
	if( colWidths[0] * rowHeights[0] < 64 )
		noSpaces = true;
	
	//! Move days to right position!
	for( _u8 curDay = 1; curDay <= daysInMonth ; curDay++ )
	{
		_stickyButton* btn = this->registeredSelectors[curDay];
		
		if( noSpaces )
			btn->setDimensions( _rect( x , y , colWidths[weekday] + 1 , rowHeights[curRow] + 1 ) );
		else
			btn->setDimensions( _rect( x , y , colWidths[weekday] + 1 , rowHeights[curRow] + 1 ) );
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
		_singleValueGroup::setIntValue( selectedDayOfMonth );
	else
		_singleValueGroup::setIntValue( -1 );
	
	
	//! Make 'today' appear blue
	if( todayYear == this->curYear && this->curMonth == todayMonth )
	{
		selectorListType::iterator it = this->registeredSelectors.find( todayDayOfMonth );
		if( it != this->registeredSelectors.end() )
			it->second->setAutoSelect( true );
	}
	
	return handled;
}

_calendar::~_calendar()
{
	this->removeChildren();
	this->removeEnhancedChildren( true );
	
	// Free _stickybuttons
	for( selectorListType::value_type btn : this->registeredSelectors )
		delete btn.second;
}