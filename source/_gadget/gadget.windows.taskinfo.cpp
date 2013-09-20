#include "_gadget/gadget.windows.startbutton.h"
#include "_gadget/gadget.windows.taskinfo.h"
#include "_type/type.system.h"
#include "_type/type.time.h"

//! Graphics
#include "_resource/BMP_Taskinfo.h"


_callbackReturn _windowsTaskInfo::refreshHandler( _event event )
{
	static string sBT = _system::getLocalizedString("lbl_startButtonText");
	
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


_windowsTaskInfo::_windowsTaskInfo( _coord x , _coord y , _style&& style ) :
	_gadget( _gadgetType::imagegadget , 25 , 10 , x - 25 , y , style | _styleAttr::canNotReceiveFocus | _styleAttr::canNotTakeFocus )
	, time( new _label( 24 , 10 , 0 , 0 , "00:00" ) )
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