#include "_gadget/gadget.windows.h"

// For Console
#include "nds/arm9/console.h"
#include "nds/arm9/input.h"

void _windows::optimizeEvents(){
	//! Optimize refresh-Events
	//map<_gadget*,_gadgetEvent*);
	/*deque<_gadgetEvent> tempEvents = this->events;
	
	sort( tempEvents.begin() , tempEvents.end() , [](_gadgetEvent e1 , _gadgetEvent e2)->bool{ return ( e1.getArgs().getDestination() < e2.getArgs().getDestination() ); } );
	
	_gadget* dest = 0;
	_gadget* tempDest = 0;
	_area refresh,damaged;
	refresh.clear();
	damaged.clear();
	
	this->events.clear();
	
	for( auto it = tempEvents.begin() ; it != tempEvents.end() ; it++ )
	{
		tempDest = (_gadget*) it->getArgs().getDestination();
		
		damaged.push_back( it->getArgs().getRefreshRects() );
		damaged.push_back( it->getArgs().getDamagedRects() );
		
		if( dest != 0 && dest != tempDest ){
			it->getArgs().setDamagedRects( damaged );
			it->getArgs().setRefreshRects( refresh );
			this->events.push_back( *it );
		}
		dest = tempDest;
	}
	
	this->events = tempEvents;*/
}

void _windows::enableEventThrowing( void )
{
	// Copy new Events
	this->events.insert( this->events.end() , this->newEvents.begin() , this->newEvents.end() );
	
	// Erase temporary events
	this->newEvents.clear();	
	
	// Reenable Events!
	this->eventThrowable = true;
}

_u16 _windows::getCurrentKeys(){
	return keysHeld() & (~(KEY_TOUCH|KEY_LID));
}

void _windows::disableEventThrowing( void )
{
	// Disable Throwing of Events!
	this->eventThrowable = false;
}

void _windows::processInput()
{
	// Refresh Input
	scanKeys();
	
	
	// Touch
	static touchPosition startTouch; // For Dragging
	static touchPosition lastTouch;
	static touchPosition touchBefore;
	static _u32 touchCycles = 0;
	static _u8 cyclesLastClick = 0;
	static bool touchDragging = false;
	// Keys
	static _u16 lastKeys = 0; // 0 = No Keys
	static _u32 heldCycles[16] = {
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	static _u16 bits[16] = {
			1, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, 1 << 7,
			1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12, 1 << 13,
			1 << 14, 1 << 15 };
	
	
	// Temp...
	_gadgetEventArgs args;
	_u16 newKeys = keysHeld();
	touchPosition newTouch;
	touchRead( &newTouch );
	
	if( cyclesLastClick && !( cyclesLastClick >> 7 ) )
		cyclesLastClick++;
	
	/*!
	 * Just Handle the Buttons, not the Pen!
	 **/
	for( u8 i = 0 ; i < 16 ; ++i )
	{
		//! Again: We do not handle Pen (as well as the lid)
		if( BIT(i) == KEY_TOUCH || BIT(i) == KEY_LID ) continue;
		
		
		// Increase Cycles
		if( GETBIT( newKeys , i ) )
		{
			if( heldCycles[i] == 0 )
			{
				// Set the Args
				args.reset();
				args.setKeyCode( BIT(i) );
				args.setCurrentKeyCodes( lastKeys );
				
				
				// Trigger the Event
				this->handleEvent( _gadgetEvent( keyDown , args ) );
			}
			
			
			// Increase Cycles
			heldCycles[i]++;
		}
		
		
		// Erase Cycles (but only if the Button of the Bit was pressed previously!)
		else if( heldCycles[i] > 0 )
		{
			// Set the Args
			args.reset();
			args.setKeyCode( bits[i] );
			args.setCurrentKeyCodes( lastKeys );
			
			
			// Trigger the Event
			this->handleEvent( _gadgetEvent( keyUp , args ) );
			
			
			// If keyup is fast enough, trigger keyClick (only if the "button" wasn't the mouse
			if( heldCycles[i] < _defaultRuntimeAttributes_.maxClickCycles )
				this->triggerEvent( _gadgetEvent( keyClick , args ) );
			
			
			// Reset Cycles
			heldCycles[i] = 0;
		}
	}

	/*!
	 * Now we'll handle the Pen!
	 **/
	if( newKeys & KEY_TOUCH )
	{
		
		//! Check if this is the first Cycle the pen is down
		if( touchCycles == 0 )
		{
			// Set the Args
			args.reset();
			args.setCurrentKeyCodes( newKeys );
			args.setPosX( newTouch.px );
			args.setPosY( newTouch.py );
			
			
			// Trigger the Event
			this->handleEvent( _gadgetEvent( mouseDown , args ) );
			
			
			// Set the starting Point of the mouseDown here!
			startTouch = newTouch;
			lastTouch = newTouch;
		}

		//! Check if the Pen is dragging something
		if( touchDragging )
		{
			// Set the Args
			args.reset();
			args.setCurrentKeyCodes( newKeys );
			args.setDeltaX( newTouch.px - lastTouch.px );
			args.setDeltaY( newTouch.py - lastTouch.py );
			args.setPosX( newTouch.px );
			args.setPosY( newTouch.py );
			
			// Trigger the Event
			this->handleEvent( _gadgetEvent( dragging , args ) );
			
			lastTouch = newTouch;
		}

		//! Check if the Stylus has moved
		if( !touchDragging )
		{
			// Compute the dragged Distance
			// -> Pythagoras!
			u32 xd = newTouch.px - startTouch.px;
			u32 yd = newTouch.py - startTouch.py;
			u32 dragDistance = xd * xd + yd * yd;
			
			
			// Check if Pen has moved the distance already
			if( dragDistance > _defaultRuntimeAttributes_.minDragDistance * _defaultRuntimeAttributes_.minDragDistance )
			{
				// Set the Args
				args.reset();
				args.setCurrentKeyCodes( newKeys );
				args.setPosX( startTouch.px );
				args.setPosY( startTouch.py );
				
				// Trigger the Event!
				this->handleEvent( _gadgetEvent( dragStart , args ) );
				
				lastTouch = startTouch;
				
				// Set touchDragging to true!
				touchDragging = true;
				
			}
		}
		
		
		// Increase Cycles
		touchCycles++;
	}
	//! Pen is not down
	else if( touchCycles > 0 )
	{
		// Set the Args
		args.reset();
		args.setCurrentKeyCodes( newKeys );
		args.setPosX( lastTouch.px );
		args.setPosY( lastTouch.py );
		
		
		//! Maybe DragStop-Event?
		if( touchDragging )
		{
			// Set touchDragging to false!
			touchDragging = false;
			
			// Trigger the Event!
			this->handleEvent( _gadgetEvent( dragStop , args ) );
		}
		//! Maybe Click-Event?
		else if( touchCycles < _defaultRuntimeAttributes_.maxClickCycles )
		{
			_s16	deltaX = touchBefore.px - lastTouch.px;
			_s16	deltaY = touchBefore.py - lastTouch.py;
			_s16 	deltaTouch = deltaX * deltaX + deltaY * deltaY;
			// Trigger the mouseClick-Event!
			if( cyclesLastClick && cyclesLastClick < _defaultRuntimeAttributes_.maxDoubleClickCycles && deltaTouch < _defaultRuntimeAttributes_.maxDoubleClickArea * _defaultRuntimeAttributes_.maxDoubleClickArea ){
				this->handleEvent( _gadgetEvent( mouseDoubleClick , args ) );
				cyclesLastClick = 0;
			}
			else{
				this->handleEvent( _gadgetEvent( mouseClick , args ) );
				cyclesLastClick = 1;
			}
		}
		
		// Trigger the mouseUp-Event!
		this->handleEvent( _gadgetEvent( mouseUp , args ) );
		touchBefore = lastTouch;
		
		// Reset touch Cycles
		touchCycles = 0;
		
	}

	// Refresh the old inputs
	lastKeys = newKeys;
}

void _windows::generateEvent( _gadgetEvent event , bool works )
{
	if( this->eventThrowable )
		this->events.push_back( event );
	else 
		this->newEvents.push_back( event );
}

void _windows::processEvents()
{
	optimizeEvents();
	// Do not throw any Events until we finished iterating through Events!!!!
	// -> This was a big Problem - Hours of finding that out!
	disableEventThrowing();
	
	// Temp...
	_gadget* gadget;
	_gadgetEventReturnType ret;

	for( _gadgetEvent event : this->events )
	{
		gadget = (_gadget*)event.getArgs().getDestination();		
		
		// Default: Say its not handled
		// It can be set if Gadget != nullptr
		ret = not_handled;
		
		// Make the Gadget ( if one is specified ) react on the event
		if( gadget != nullptr )
			ret = gadget->handleEvent( event );
		
		
		// Check if the Event was Handled
		// -> If it can be deleted afterwards: Delete It!!!
		if( ret == not_handled_no_delete || ret == handled_no_delete )
			this->generateEvent( event );
	}

	// Erase all Events
	this->events.clear();
	
	
	// Copy all new Events into events Vector...
	enableEventThrowing();
	
}


//! Constructor
_windows::_windows( _gadgetStyle style ) :
	_gadget( _gadgetType::windows , SCREEN_WIDTH , SCREEN_HEIGHT , 0 , 0 , style , true )
{	
	//! Set my bitmap
	this->bitmap = new _bitmap( BG_GFX , SCREEN_WIDTH , SCREEN_HEIGHT );
	
	//! Allocate new taskboard
	this->taskboard = new _windowsTaskBoard();
	this->desktop = new _desktop();
	this->addChild( this->desktop );
	this->addChild( this->taskboard );
	
	
	// Refresh Me
	this->refreshBitmap();
}


bool _windows::focusChild( _gadget* child )
{	
	_gadgetList::iterator itTemp = find( this->children.begin() , this->children.end() , child );
	
	if( child == nullptr || child->getType() == _gadgetType::taskboard || child->getType() == _gadgetType::desktop || *itTemp == *((this->children.rbegin())++))
		return false;
	
	_gadgetList::iterator itKeyboard = find( this->children.begin() , this->children.end() , this->taskboard );
	
	this->children.erase( itTemp );
	this->children.erase( itKeyboard );
	this->children.push_back( child );
	this->children.push_back( this->taskboard );
	
	// Refresh the Window
	child->bubbleRefresh();
	
	return true;
}


void _windows::VBlank(){
	this->processInput();
}