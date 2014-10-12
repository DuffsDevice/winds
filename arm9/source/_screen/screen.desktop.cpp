#include <_screen/screen.desktop.h>
#include <_controller/controller.gui.h>
#include <_func/func.gridcreator.h>

void _desktopScreen::taskHandler( _window* ref )
{
	auto it = find_if( this->tasks.begin() , this->tasks.end() , [ref]( _desktopScreenTaskButton* ptr )->bool{ return ptr->getReference() == ref; } );
	
	if( ref->isTask() )
	{
		if( it != this->tasks.end() )
			(*it)->redraw();
		else
		{
			_desktopScreenTaskButton* ptr = new _desktopScreenTaskButton( 0 , SCREEN_HEIGHT - 10 , ref );
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

void _desktopScreen::refreshTaskWidths()
{
	int x = this->startButton->getWidth() + 2;
	int i = 0;
	int availableSpace = SCREEN_WIDTH - this->startButton->getWidth() - this->taskInfo->getWidth() - 3;
	
	_vector<int> widths = computeGrid( availableSpace , this->tasks.size() );
	
	for( _desktopScreenTaskButton* tb : this->tasks )
	{
		int taskWidth = min( 41 , widths[i++] );
		tb->setX( x );
		tb->setWidth( taskWidth - 1 );
		x += taskWidth;
	}
}

_callbackReturn _desktopScreen::keyHandler( _event event )
{
	// Receive Gadget
	_desktopScreen* that = event.getGadget<_desktopScreen>();
	
	if( event.getKeyCode() != _key::l )
		return not_handled;
	
	if( event == onKeyDown )
		that->switcher->showCentered( SCREEN_WIDTH / 2 , ( SCREEN_HEIGHT - 12 ) / 2 );
	else if( event == onKeyUp )
		that->switcher->shelve( true );
	
	return handled;
}

_callbackReturn _desktopScreen::taskInfoMoveHandler( _event event ){
	this->refreshTaskWidths();
	return use_internal;
}

_callbackReturn _desktopScreen::refreshHandler( _event event )
{
	// Receive Gadget
	_desktopScreen* that = event.getGadget<_desktopScreen>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );

	// Taskbar
	bP.copyHorizontalStretch( 0 , SCREEN_HEIGHT - 10 , SCREEN_WIDTH , _guiController::getWindowsDesign() );
	
	return use_default;
}

//! Constructor
_desktopScreen::_desktopScreen( _u8 bgId , _style&& style ) :
	_gadgetScreen( bgId , _gadgetScreenType::desktop , SCREEN_WIDTH , SCREEN_HEIGHT , (_style&&)style )
{
	// Register Event-Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_desktopScreen::refreshHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_desktopScreen::keyHandler ) );
	this->setInternalEventHandler( onKeyUp , make_callback( &_desktopScreen::keyHandler ) );
	
	// Set Padding
	this->setPadding( _padding( 0 , 0 , 0 , 10 ) );
	
	
	// Add FileView and Wallpaper
	this->addChild( this->desktop = new _desktopScreenView() );
	
	// Add TaskInfo
	this->addEnhancedChild( this->taskInfo = new _desktopScreenInfoArea( SCREEN_WIDTH , SCREEN_HEIGHT - 10 ) );
	this->taskInfo->setUserEventHandler( onMove , make_callback( this , &_desktopScreen::taskInfoMoveHandler ) );
	
	// Add startButton
	this->addEnhancedChild( this->startButton = new _desktopScreenStartButton( 0 , SCREEN_HEIGHT - 10 ) );
	
	// Add a Task-Switcher
	this->switcher = new _programSwitcher();
	
	
	// Refresh Me
	this->redraw();
	
	// Get notified, when a "task" in the taskbar needs to be refreshed
	_window::addTaskHandler( make_callback( this , &_desktopScreen::taskHandler ) );
}

_desktopScreen::~_desktopScreen()
{
	// Remove Handler
	_window::removeTaskHandler( make_callback( this , &_desktopScreen::taskHandler ) );
	
	delete this->desktop;
	delete this->switcher;
	delete this->taskInfo;
	delete this->startButton;
	
	for( _desktopScreenTaskButton* button : this->tasks )
		delete button;
}