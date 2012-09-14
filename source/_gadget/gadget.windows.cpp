#include "_gadget/gadget.windows.h"

//! For Console
#include <nds/arm9/console.h>
#include <nds/arm9/input.h>

#include "_type/type.system.h"

//! Graphics
#include "_resource/BMP_StartButton.h"

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

_gadgetEventReturnType _windows::refreshHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_windows* that = event.getGadget<_windows>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.resetClippingRects();
		
	bP.copyHorizontalStretch( 33 , SCREEN_HEIGHT - 9 , SCREEN_WIDTH - 33 , _system_->_runtimeAttributes_->windowBar );
	
	return use_default;
}

//! Constructor
_windows::_windows( _u8 bgId , _gadgetStyle style ) :
	_gadgetScreen( bgId , style )
{
	//! Set Padding
	this->setPadding( _padding( 0 , 0 , 0 , 9 ) );
	
	//! Add startButton
	this->startButton = new _windowsStartButton( 0 , SCREEN_HEIGHT - 9 );
	this->startButton->setEnhanced( true );
	
	//! Allocate new _desktop
	this->desktop = new _desktop();
	this->addChild( this->desktop );
	this->addChild( this->startButton );
	
	//! Register Event-Handlers
	this->registerEventHandler( "refresh" , &_windows::refreshHandler );
	
	// Refresh Me
	this->refreshBitmap();
}

/*bool _windows::focusChild( _gadget* child )
{
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
	//this->children.splice( this->children.end() , this->children , find( this->children.begin() , this->children.end() , this->taskboard ) );
	
	// Refresh the Gadget
	child->bubbleRefresh();
	
	return true;
}*/