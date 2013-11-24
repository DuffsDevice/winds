#include "_gadget/gadget.windows.h"
#include "_type/type.system.h"
#include "func.gridcreator.h"

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
	int x = this->startButton->getWidth() + 2;
	int i = 0;
	int availableSpace = SCREEN_WIDTH - this->startButton->getWidth() - this->taskInfo->getWidth() - 3;
	
	_vector<int> widths = computeGrid( availableSpace , this->tasks.size() );
	
	for( _windowsTaskButton* tb : this->tasks )
	{
		int taskWidth = min( 41 , widths[i++] );
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
			tb->redraw();
			return;
		}
	}
}


_callbackReturn _windows::refreshHandler( _event event )
{
	// Receive Gadget
	_windows* that = event.getGadget<_windows>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Taskbar
	bP.copyHorizontalStretch( 33 , SCREEN_HEIGHT - 10 , SCREEN_WIDTH - 33 , _system::getRTA().getWindowsDesignActive() );
	
	return use_default;
}


//! Constructor
_windows::_windows( _u8 bgId , _style&& style ) :
	_gadgetScreen( bgId , _gadgetScreenType::windows , (_style&&)style )
{
	// Register Event-Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_windows::refreshHandler ) );
	
	// Set Padding
	this->setPadding( _padding( 0 , 0 , 0 , 10 ) );
	
	// Add startButton
	this->startButton = new _windowsStartButton( 0 , SCREEN_HEIGHT - 10 );
	
	// Add TaskInfo
	this->taskInfo = new _windowsTaskInfo( SCREEN_WIDTH , SCREEN_HEIGHT - 10 );
	
	// Allocate new _desktop and bind the _startButton
	this->addChild( this->desktop = new _desktop() );
	this->addEnhancedChild( this->startButton );
	this->addEnhancedChild( this->taskInfo );
	
	// Register Event-Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_windows::refreshHandler ) );

	// Refresh Me
	this->redraw();
}