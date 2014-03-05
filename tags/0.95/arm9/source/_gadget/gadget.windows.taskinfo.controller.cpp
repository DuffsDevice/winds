#include "_gadget/gadget.windows.startbutton.h"
#include "_gadget/gadget.windows.taskinfo.controller.h"
#include "_type/type.system.h"
#include "_type/type.time.h"

//! Graphics
#include "_resource/resource.image.windows.taskinfo.h"

_callbackReturn _windowsTaskInfoController::refreshHandler( _event event )
{
	// Receive Gadget
	_windowsTaskInfoController* that = event.getGadget<_windowsTaskInfoController>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.copy( bP.getWidth() - 6 , 0 , BMP_TaskInfoRightPart() ); // Right Side
	bP.copyHorizontalStretch( 9 , 0 , bP.getWidth() - 15 , BMP_TaskInfoMiddlePart() ); // Middle Part
	bP.copy( 0 , 0 , BMP_TaskInfoLeftPart() ); // Left Side
	
	that->time->setStrValue( _time::now().toString("%H:%M") );
	that->time->update();
	
	return use_default;
}

void _windowsTaskInfoController::taskInfoHandler( _windowsTaskInfo* info )
{
	if( info->isValid() )
	{
		info->requestAutoX();
		info->setY(2);
		
		// Register additional handlers
		info->setInternalEventHandler( onParentSet , _gadgetHelpers::moveBesidePrecedent( _direction::right , 2 ) );
		info->setInternalEventHandler( onPreMove , _gadgetHelpers::moveBesidePrecedent( _direction::right , 2 ) );
		
		// Add it
		this->addChild( info , true );
	}
	this->setWidth( _windowsTaskInfo::taskInfos.size() * 8 + 27 );
}

_windowsTaskInfoController::_windowsTaskInfoController( _optValue<_coord> x , _optValue<_coord> y , _style&& style ) :
	_gadget( _gadgetType::imagegadget , ignore , ignore , 27 , 10 , style | _style::notFocusable | _style::canNotTakeFocus )
	, time( new _label( ignore , 0 , ignore , 10 , "" ) )
	, timer( make_callback<void>( (_gadget*)this , &_gadget::redraw ) , 10000 , true ) // Make the clock update itself
{
	this->setPadding(_padding(3,0,22,0));
	
	// Adjust Label
	this->time->setAlign( _align::right );
	this->time->setColor( _color::white );
	this->time->setUserEventHandler( onUpdate , _gadgetHelpers::rightBottomAlign( 1 , ignore ) );
	this->time->update();
	
	// Register Event-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_windowsTaskInfoController::refreshHandler ) );
	this->setInternalEventHandler( onResize , _gadgetHelpers::rightBottomAlign( 0 , 0 ) );
	
	// Add our time
	this->addEnhancedChild( this->time );
	
	// Refresh
	this->redraw();
	
	// Register inside _windowsTaksInfo
	_windowsTaskInfo::addTaskInfoHandler( make_callback( this , &_windowsTaskInfoController::taskInfoHandler ) );
	
	// Update the position
	this->triggerEvent( onResize );
	
	// Start Updating the clock
	this->timer.start();
}

_windowsTaskInfoController::~_windowsTaskInfoController(){
	delete this->time;
}