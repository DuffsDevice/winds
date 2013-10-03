#ifndef _WIN_T_GADGTEHELPERS_
#define _WIN_T_GADGTEHELPERS_

#include "_type/type.h"
#include "_type/type.gadget.h"

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
			struct{
				bool proceedX : 1;
				bool proceedY : 1;
				_length smallerX : 15;
				_length smallerY : 15;
			} PACKED ;
			_callbackReturn executor( _event event ) const ;
		public:
			// Ctor
			sizeParent( _optValue<_length>&& smallerX = 0 , _optValue<_length>&& smallerY = 0 );
	};
	
	//! Class to move the gadget to either below or dexterwise of its precedent child
	class moveBesidePrecedent : public _dummyCallback<_eventHandler>
	{
		private:
			struct{
				_u8		dimension : 1;
				bool	breakLine : 1;
				_length	spaceX;
				_length	spaceY;
				_length	offsetX;
				_length	offsetY;
			} PACKED ; 
			_callbackReturn executor( _event event ) const ;
		public:
			// Ctor
			moveBesidePrecedent( _dimension dim = _dimension::vertical , _length spaceX = 0 , _length spaceY = 0 , bool breakLine = false , _length offsetX = 1 , _length offsetY = 1 );
	};
}

#endif