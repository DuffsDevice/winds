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
	_gadget( _gadgetType::windows , SCREEN_WIDTH , SCREEN_HEIGHT * 2, 0 , 0 , style , true )
{	
	//! Set my bitmap
	this->bitmap = new _bitmap( BG_GFX , SCREEN_WIDTH, 256 );
	this->bitmap->reset( RGB( 31, 31 , 31 ) );
	
	//! Allocate new taskboard
	this->taskboard = new _keyboard();
	this->desktop = new _desktop();
	this->addChild( this->desktop );
	this->addChild( this->taskboard );
	
	// Refresh Me
	this->refreshBitmap();
}

bool _windows::focusChild( _gadget* child )
{
	//return _gadget::focusChild( child );
	//printf("Trying to Focus %s\n",gadgetType2string[child->getType()].c_str() );
	
	if( !child )
		return false;
	
	_gadgetList::iterator itTemp = find( this->children.begin() , this->children.end() , child );
	
	// Blur the Previously focused gadget
	if( !child->hasFocus() )
		this->blurEventChild();
	else
		return false;
	
	child->focused = true;
	
	if( child->getType() == _gadgetType::desktop || *itTemp == *( ++this->children.rbegin() ) )
		return true;
	
	// Move the gadget to "top"
	this->children.splice( this->children.end() , this->children , itTemp );
	
	// Move the keyboard top the "top", so the child will be at pos No. 2
	this->children.splice( this->children.end() , this->children , find( this->children.begin() , this->children.end() , this->taskboard ) );
	
	// Refresh the Gadget
	child->bubbleRefresh();
	
	return true;
}