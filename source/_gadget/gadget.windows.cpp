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

void _windows::registerTask( _window* w )
{
	if( count_if( this->tasks.begin() , this->tasks.end() , [&]( _windowsTaskButton* ptr )->bool{ return ptr->getReference() == w; } ) >= 1 )
		return;
	_windowsTaskButton* ptr;
	this->tasks.push_back( ptr = new _windowsTaskButton( 0 , SCREEN_HEIGHT - 10 , w ) );
	this->addEnhancedChild( ptr );
	this->refreshTasks();
}

void _windows::removeTask( _window* w )
{
	this->tasks.remove_if( [&]( _windowsTaskButton* ptr )->bool{ if( ptr->getReference() == w ){ delete ptr; return true; } return false; } );
	this->refreshTasks();
}

void _windows::refreshTasks()
{
	int x = 40;
	int availableSpace = SCREEN_WIDTH - 40;
	int taskWidth = min( 41.f , float( float( availableSpace ) / this->tasks.size() + 0.5f ) );
	for( _windowsTaskButton* tb : this->tasks )
	{
		tb->setX( x );
		tb->setWidth( taskWidth - 1 );
		x += taskWidth;
	}
}

void _windows::refreshTask( _window* w )
{
	for( _windowsTaskButton* tb : this->tasks )
	{
		if( tb->getReference() == w )
		{
			tb->bubbleRefresh( true );
			return;
		}
	}
}

_callbackReturn _windows::refreshHandler( _event event )
{	
	// Receive Gadget
	_windows* that = event.getGadget<_windows>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
		
	bP.copyHorizontalStretch( 33 , SCREEN_HEIGHT - 10 , SCREEN_WIDTH - 33 , _system::_runtimeAttributes_->windowBar );
	
	return use_default;
}

//! Constructor
_windows::_windows( _u8 bgId , _style style ) :
	_gadgetScreen( bgId , _gadgetScreenType::windows , style )
{
	//! Set Padding
	this->setPadding( _padding( 0 , 0 , 0 , 9 ) );
	
	//! Add startButton
	this->startButton = new _windowsStartButton( 0 , SCREEN_HEIGHT - 10 );
	
	//! Allocate new _desktop and bind the _startButton
	this->addChild( this->desktop = new _desktop() );
	this->addEnhancedChild( this->startButton );
	
	//! Register Event-Handlers
	this->registerEventHandler( refresh , new _staticCallback( &_windows::refreshHandler ) );
	
	// Refresh Me
	this->refreshBitmap();
}