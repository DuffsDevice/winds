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
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	bP.copy( bP.getWidth() - 6 , 0 , BMP_TaskInfoRightPart() ); // Right Side
	bP.copyHorizontalStretch( 9 , 0 , bP.getWidth() - 15 , BMP_TaskInfoMiddlePart() ); // Middle Part
	bP.copy( 0 , 0 , BMP_TaskInfoLeftPart() ); // Left Side
	
	that->time->setStrValue( _time::now().toString("%H:%M") );
	
	return use_default;
}


_windowsTaskInfo::_windowsTaskInfo( _coord x , _coord y , _style style ) :
	_gadget( _gadgetType::imagegadget , 25 , 10 , x - 25 , y , style | _styleAttr::canNotReceiveFocus | _styleAttr::canNotTakeFocus )
	, time( new _label( 24 , 10 , 0 , 0 , "00:00" ) )
{
	// Adjust Label
	this->time->setAlign( _align::right );
	this->time->setColor( COLOR_WHITE );
	
	// Register Event-Handler
	this->registerEventHandler( refresh , new _staticCallback( &_windowsTaskInfo::refreshHandler ) );
	
	// Make the clock update itself
	_system::executeTimer( new _gadget::eventForwardRefreshGadget( this ) , 10000 , true );
	
	// Add our time
	this->addEnhancedChild( this->time );
	
	// Refresh
	this->refreshBitmap();
}

_windowsTaskInfo::~_windowsTaskInfo()
{
	delete this->time;
}