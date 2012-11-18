#include "_gadget/gadget.windows.h"

#include "_type/type.system.h"

//void _windows::optimizeEvents(){
	//! Optimize refresh-Events
	//map<_gadget*,_event*);
	/*deque<_event> tempEvents = this->events;
	
	sort( tempEvents.begin() , tempEvents.end() , [](_event e1 , _event e2)->bool{ return ( e1.getDestination() < e2.getDestination() ); } );
	
	_gadget* dest = 0;
	_gadget* tempDest = 0;
	_area refresh,damaged;
	refresh.clear();
	damaged.clear();
	
	this->events.clear();
	
	for( auto it = tempEvents.begin() ; it != tempEvents.end() ; it++ )
	{
		tempDest = (_gadget*) it->getDestination();
		
		damaged.push_back( it->getRefreshRects() );
		damaged.push_back( it->getDamagedRects() );
		
		if( dest != 0 && dest != tempDest ){
			it->setDamagedRects( damaged );
			it->setRefreshRects( refresh );
			this->events.push_back( *it );
		}
		dest = tempDest;
	}
	
	this->events = tempEvents;*/
//}

_callbackReturn _windows::refreshHandler( _event event )
{	
	// Receive Gadget
	_windows* that = event.getGadget<_windows>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
		
	bP.copyHorizontalStretch( 33 , SCREEN_HEIGHT - 9 , SCREEN_WIDTH - 33 , _system_->_runtimeAttributes_->windowBar );
	
	return use_default;
}

//! Constructor
_windows::_windows( _u8 bgId , _style style ) :
	_gadgetScreen( bgId , style )
{
	//! Set Padding
	this->setPadding( _padding( 0 , 0 , 0 , 9 ) );
	
	//! Add startButton
	this->startButton = new _windowsStartButton( 0 , SCREEN_HEIGHT - 9 );
	
	//! Allocate new _desktop and bind the _startButton
	this->addChild( this->desktop = new _desktop() );
	this->addEnhancedChild( this->startButton );
	
	//! Register Event-Handlers
	this->registerEventHandler( refresh , &_windows::refreshHandler );
	
	// Refresh Me
	this->refreshBitmap();
}