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
			static _callbackReturn refreshForwardHandler( _event event );
		public:
			// Ctor
			eventForwardRefresh() :
				_staticCallback( &eventForwardRefresh::refreshForwardHandler )
			{}
	};
	
	//! Class to forward any event to an refresh-event thrown on a specific gadget
	class eventForwardRefreshGadget : public _classCallback<_eventHandler>
	{
		private:
			_callbackReturn refreshForwardHandler(_event);
		public:
			// Ctor
			eventForwardRefreshGadget( _gadget* g ) :
				_classCallback( (eventForwardRefreshGadget*)g , &eventForwardRefreshGadget::refreshForwardHandler )
			{}
	};
	
	//! Class to forward any event to any other
	class eventForward : public _classCallback<_eventHandler>
	{
		private:
			_callbackReturn eventForwardHandler( _event event );
		public:
			// Ctor
			eventForward( _eventType newType ) :
				_classCallback( (eventForward*)newType , &eventForward::eventForwardHandler )
			{}
	};
	
	//! Class to forward any event to another gadget
	class eventForwardGadget : public _classCallback<_eventHandler>
	{
		private:
			_callbackReturn eventForwardHandler( _event event );
		public:
			// Ctor
			eventForwardGadget( _gadget* dest ) :
				_classCallback( (eventForwardGadget*)dest , &eventForwardGadget::eventForwardHandler )
			{}
	};
	
	//! Class to resize the gadget to the size of the parent
	class sizeParent : public _classCallback<_eventHandler>
	{
		private:
			_callbackReturn eventHandler( _event event );
			
		public:
			// Ctor
			sizeParent( _optValue<_length>&& smallerX = 0 , _optValue<_length>&& smallerY = 0 );
	};
	
	//! Class to move the gadget to either below or dexterwise of its precedent child
	class moveBesidePrecedent : public _classCallback<_eventHandler>
	{
		private:
			_callbackReturn eventHandler( _event event );
				struct internalDataFormatStruct{
					_u8		dim : 1;
					bool	skipHidden : 1;
					_u8		spaceX : 8;
					_u8		spaceY : 8;
					_u16	lBOffset : 15;
					
					operator moveBesidePrecedent*(){
						return *(reinterpret_cast<moveBesidePrecedent**>(this));
					}
				};
			
		public:
			// Ctor
			moveBesidePrecedent( _dimension dim = _dimension::vertical , _u8 spaceX = 0 , _u8 spaceY = 0 , bool skipHidden = true , _optValue<_u16>&& lbOffset = ignore );
	};
}

#endif