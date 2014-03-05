#include "_gadget/gadget.windows.h"
#include "_type/type.system.h"
#include "func.gridcreator.h"

void _windows::taskHandler( _window* ref )
{
	auto it = find_if( this->tasks.begin() , this->tasks.end() , [ref]( _windowsTaskButton* ptr )->bool{ return ptr->getReference() == ref; } );
	
	if( ref->isTask() )
	{
		if( it != this->tasks.end() )
			(*it)->redraw();
		else
		{
			_windowsTaskButton* ptr = new _windowsTaskButton( 0 , SCREEN_HEIGHT - 10 , ref );
			this->tasks.push_back( ptr );
			this->addEnhancedChild( ptr );
			this->refreshTaskWidths();
		}
	}
	else if( it != this->tasks.end() ){
		delete (*it); // Delete taskbutton
		this->tasks.erase( it );
		this->refreshTaskWidths();
	}
}

void _windows::refreshTaskWidths()
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

_callbackReturn _windows::keyHandler( _event event )
{
	// Receive Gadget
	_windows* that = event.getGadget<_windows>();
	
	if( event.getKeyCode() != _key::l )
		return not_handled;
	
	if( event == onKeyDown )
		that->switcher->showCentered( SCREEN_WIDTH / 2 , ( SCREEN_HEIGHT - 12 ) / 2 );
	else if( event == onKeyUp )
		that->switcher->shelve( true );
	
	return handled;
}

_callbackReturn _windows::taskInfoMoveHandler( _event event ){
	this->refreshTaskWidths();
	return use_internal;
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
	this->setInternalEventHandler( onKeyDown , make_callback( &_windows::keyHandler ) );
	this->setInternalEventHandler( onKeyUp , make_callback( &_windows::keyHandler ) );
	
	// Set Padding
	this->setPadding( _padding( 0 , 0 , 0 , 10 ) );
	
	// Add startButton
	this->startButton = new _windowsStartButton( 0 , SCREEN_HEIGHT - 10 );
	
	// Add TaskInfo
	this->taskInfo = new _windowsTaskInfoController( SCREEN_WIDTH , SCREEN_HEIGHT - 10 );
	this->taskInfo->setUserEventHandler( onMove , make_callback( this , &_windows::taskInfoMoveHandler ) );
	
	// Add a Task-Switcher
	this->switcher = new _windowsProgramSwitcher();
	
	// Allocate new _desktop and bind the _startButton
	this->addChild( this->desktop = new _desktop() );
	this->addEnhancedChild( this->startButton );
	this->addEnhancedChild( this->taskInfo );
	
	// Refresh Me
	this->redraw();
	
	// Get notified, when a "task" in the taskbar needs to be refreshed
	_window::addTaskHandler( make_callback( this , &_windows::taskHandler ) );
}

_windows::~_windows(){
	// Remove Handler
	_window::removeTaskHandler( make_callback( this , &_windows::taskHandler ) );
}