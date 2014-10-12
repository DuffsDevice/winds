#include <_screen/screen.desktop.startbutton.h>
#include <_screen/screen.desktop.infoarea.h>
#include <_type/type.time.h>
#include <_type/type.gadget.helpers.h>
#include <_controller/controller.interrupt.h>

//! Graphics
#include <_resource/resource.image.windows.infoarea.h>

void _desktopScreenInfoArea::timerHandler()
{
	this->time->setStrValue( _time::now().toString("%H:%M") );
	this->time->update();
}

_callbackReturn _desktopScreenInfoArea::refreshHandler( _event event )
{
	// Receive Gadget
	_desktopScreenInfoArea* that = event.getGadget<_desktopScreenInfoArea>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.copy( 0 , 0 , BMP_InfoAreaLeftPart() ); // Left Side
	bP.copyHorizontalStretch( 9 , 0 , bP.getWidth() - 15 , BMP_InfoAreaMiddlePart() ); // Middle Part
	bP.copy( bP.getWidth() - 6 , 0 , BMP_InfoAreaRightPart() ); // Right Side
	
	return use_default;
}

void _desktopScreenInfoArea::taskInfoHandler( _taskInfo* info )
{
	if( info->isValid() )
	{
		info->requestAutoX();
		info->setY(2);
		
		// Register additional handlers
		info->setInternalEventHandler( onParentAdd , _gadgetHelpers::eventForward(onPreMove) );
		info->setInternalEventHandler( onPreMove , _gadgetHelpers::moveBesidePrecedent( _direction::right , 2 ) );
		
		// Add it
		this->addChild( info , true );
	}
	this->setWidth( _taskInfo::getTaskInfos().size() * 8 + 27 );
}

_desktopScreenInfoArea::_desktopScreenInfoArea( _optValue<_coord> x , _optValue<_coord> y , _style&& style ) :
	_gadget( _gadgetType::imagegadget , ignore , ignore , 27 , 10 , style | _style::notFocusable | _style::canNotTakeFocus )
	, time( new _label( ignore , 0 , ignore , 10 , "--:--" ) )
	, timer( make_callback<void>( this , &_desktopScreenInfoArea::timerHandler ) , 10000 , true ) // Make the clock update itself
{
	this->setPadding( {3, 0, 22, 0} );
	
	
	// Register Event-Handler
	this->setInternalEventHandler( onDraw , make_callback( &_desktopScreenInfoArea::refreshHandler ) );
	this->setInternalEventHandler( onResize , _gadgetHelpers::rightBottomAlign( 0 , 0 ) );
	
	
	// Construct Time Label
	this->time->setAlign( _align::right );
	this->time->setColor( _color::white );
	this->time->setUserEventHandler( onUpdate , _gadgetHelpers::rightBottomAlign( 2 , ignore ) );
	this->time->setUserEventHandler( onParentResize , _gadgetHelpers::rightBottomAlign( 2 , ignore ) );
	this->addEnhancedChild( this->time );
	this->timerHandler();
	
	
	// Register inside _taskInfo
	_taskInfo::addTaskInfoHandler( make_callback( this , &_desktopScreenInfoArea::taskInfoHandler ) );
	
	
	// Update the position
	this->triggerEvent( onResize );
	
	
	// Start Updating the clock
	this->timer.start();
	
	
	// Refresh
	this->redraw();
}

_desktopScreenInfoArea::~_desktopScreenInfoArea(){
	_taskInfo::removeTaskInfoHandler( make_callback( this , &_desktopScreenInfoArea::taskInfoHandler ) );
}