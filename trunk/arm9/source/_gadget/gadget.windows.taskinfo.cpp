#include "_gadget/gadget.windows.startbutton.h"
#include "_gadget/gadget.windows.taskinfo.h"
#include "_type/type.system.h"
#include "_type/type.time.h"

//! Graphics
#include "_resource/resource.image.windows.taskinfo.h"


_callbackReturn _windowsTaskInfo::refreshHandler( _event event )
{
	// Receive Gadget
	_windowsTaskInfo* that = event.getGadget<_windowsTaskInfo>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.copy( bP.getWidth() - 6 , 0 , BMP_TaskInfoRightPart() ); // Right Side
	bP.copyHorizontalStretch( 9 , 0 , bP.getWidth() - 15 , BMP_TaskInfoMiddlePart() ); // Middle Part
	bP.copy( 0 , 0 , BMP_TaskInfoLeftPart() ); // Left Side
	
	that->time->setStrValue( _time::now().toString("%H:%M") );
	
	return use_default;
}


_windowsTaskInfo::_windowsTaskInfo( _optValue<_coord> x , _optValue<_coord> y , _style&& style ) :
	_gadget( _gadgetType::imagegadget , x - 27 , y , 27 , 10 , style | _styleAttr::canNotReceiveFocus | _styleAttr::canNotTakeFocus )
	, time( new _label( 1 , 0 , 25 , 10 , "" ) )
	, timer( make_inline_callback<void()>( [this]{ this->redraw(); } ) , 10000 , true ) // Make the clock update itself
{
	// Adjust Label
	this->time->setAlign( _align::right );
	this->time->setColor( COLOR_WHITE );
	
	// Register Event-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_windowsTaskInfo::refreshHandler ) );
	
	// Add our time
	this->addEnhancedChild( this->time );
	
	// Refresh
	this->redraw();
}

_windowsTaskInfo::~_windowsTaskInfo(){
	delete this->time;
}