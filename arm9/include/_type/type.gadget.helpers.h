#ifndef _WIN_T_GADGTEHELPERS_
#define _WIN_T_GADGTEHELPERS_

#include <_type/type.h>
#include <_type/type.gadget.h>
#include <_type/type.menu.h>
#include <_type/type.direction.h>

class _contextMenu;

namespace _gadgetHelpers
{
	//! Class to forward any event to an refresh-event
	class eventForwardRefresh : public _staticCallback<_eventHandler>
	{
		private:
			static _callbackReturn executor(_event);
		public:
			// Ctor
			eventForwardRefresh() :
				_staticCallback( &eventForwardRefresh::executor )
			{}
	};
	
	//! Class to forward any event to an refresh-event thrown on a specific gadget
	class eventForwardRefreshGadget : public _dummyCallback<_eventHandler>
	{
		private:
			_gadget* newGadget;
			_callbackReturn executor(_event) const ;
		public:
			// Ctor
			eventForwardRefreshGadget( _gadget* gadget ) :
				_dummyCallback<_eventHandler>( &eventForwardRefreshGadget::executor )
				, newGadget( gadget )
			{}
	};
	
	//! Class to forward any event to any other
	class eventForward : public _dummyCallback<_eventHandler>
	{
		private:
			_eventType newType;
			_callbackReturn executor(_event) const ;
		public:
			// Ctor
			eventForward( _eventType newType ) :
				_dummyCallback<_eventHandler>( &eventForward::executor )
				, newType( newType )
			{}
	};
	
	//! Class to forward any event to another gadget
	class eventForwardGadget : public _dummyCallback<_eventHandler>
	{
		private:
			_gadget* destination;
			_callbackReturn executor( _event event ) const ;
		public:
			// Ctor
			eventForwardGadget( _gadget* dest ) :
				_dummyCallback<_eventHandler>( &eventForwardGadget::executor )
				, destination( dest )
			{}
	};
	
	//! Class to resize the gadget to the size of the parent
	class sizeParent : public _dummyCallback<_eventHandler>
	{
		private:
			_length		smallerX;
			_length		smallerY;
			struct{
				_u8		proceedX : 1;
				_u8		proceedY : 1;
			}PACKED;
			_callbackReturn executor( _event event ) const ;
		public:
			// Ctor
			sizeParent( _optValue<_length> smallerX = 0 , _optValue<_length> smallerY = 0 );
	};
	
	//! Class to resize the gadget to the size of the parent and to move it at a certain position within the parent
	class dimensionsParent : public _dummyCallback<_eventHandler>
	{
		private:
			_padding padding;
			_callbackReturn executor( _event event ) const ;
		public:
			// Ctor
			dimensionsParent( _padding padding = _padding(0) );
	};
	
	//! Class to move the gadget to a position relative to the parents right and/or bottom border
	class rightBottomAlign : public _dummyCallback<_eventHandler>
	{
		private:
			_length		distanceX;
			_length		distanceY;
			struct{
				_u8		proceedX : 1;
				_u8		proceedY : 1;
			}PACKED;
			_callbackReturn executor( _event event ) const ;
		public:
			// Ctor
			rightBottomAlign( _optValue<_length> distanceX = 0 , _optValue<_length> distanceY = 0 );
	};
	
	//! Class to move the gadget to either below or dexterwise of its precedent child
	class moveBesidePrecedent : public _dummyCallback<_eventHandler>
	{
		private:
			struct{
				_length		spaceX;
				_length		spaceY;
				_length		offsetX;
				_length		offsetY;
				_direction	direction;
				bool		breakLine : 1;
				bool		respectAutoPosition : 1;
				bool		skipHidden : 1;
			}PACKED;
			_callbackReturn executor( _event event ) const ;
		public:
			// Ctor
			moveBesidePrecedent( _direction dim = _direction::down , _length spaceX = 0 , _length spaceY = 0 , bool breakLine = false , _length offsetX = 1 , _length offsetY = 1 , bool respectAutoPosition = true , bool skipHidden = true );
	};
	
	//! Opens a context menu upon call
	class openContextMenu : public _dummyCallback<_eventHandler>
	{
		private:
			_menu*				menuToOpen;
			bool				wasAllocated;
			static _uniquePtr<
				_contextMenu>	contextMenu;
			_callbackReturn executor( _event event ) const ;
			
			static _gadget*		currentSubject;
		public:
		
			// Get the gadget that last opened the context menu
			static _gadget*		getCurrentSubject(){ return openContextMenu::currentSubject; }
			
			// Ctors
			openContextMenu( _paramAlloc<_menu> menu ) :
				_dummyCallback<_eventHandler>( &openContextMenu::executor )
				, menuToOpen( menu )
				, wasAllocated( true )
			{}
			openContextMenu( _menu* menu ) :
				_dummyCallback<_eventHandler>( &openContextMenu::executor )
				, menuToOpen( menu )
				, wasAllocated( false )
			{}
			
			// Move Ctor
			openContextMenu( openContextMenu&& menu ) :
				_dummyCallback<_eventHandler>( &openContextMenu::executor )
				, menuToOpen( menu.menuToOpen )
				, wasAllocated( menu.wasAllocated )
			{
				menu.wasAllocated = false;
			}
			
			// No Copy Ctor
			openContextMenu( const openContextMenu& menu ) = delete;
			
			// Dtor
			~openContextMenu(){
				if( this->wasAllocated ){
					delete this->menuToOpen;
					this->menuToOpen = nullptr;
					this->wasAllocated = false;
				}
			}
	};
}

#endif