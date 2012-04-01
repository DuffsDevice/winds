#include "_gadget/gadget.windows.h"

// For Console
#include "nds/arm9/console.h"
#include "nds/arm9/input.h"

//void _windows::optimizeEvents(){
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
//}


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