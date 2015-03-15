#ifndef _WIN_T_RADIOGROUP_
#define _WIN_T_RADIOGROUP_

#include <_type/type.h>
#include <_type/type.gadget.h>

template<class _selector>
class _singleValueGroup
{	
	protected:
		
		typedef _map<_s32,_selector*> selectorListType;
		
		_selector*	activeSelector;
		selectorListType registeredSelectors;
		
	public:
		
		/**
		 * Add a _radio to the list
		 * @return the Index to identify the radio later
		 */
		void addSelector( _selector* radio , _s32 assocValue );
		
		//! Remove a selector (e.g a _radio) from the _singleValueGroup
		void removeSelector( _selector* radio );
		
		//! Tick the supplied _selector, and uncheck the currently ticked
		void enableSelector( _selector* );
		
		//! Tick the next registered _selector, and uncheck the currently ticked
		void enableNext();
		
		//! Tick the previous registered _selector, and uncheck the currently ticked
		void enablePrev();
		
		//! Enable the _selector being registered under the supplied id
		void setIntValue( _s32 id );
		
		//! Get the id of the currently selected _radio/_stickyButton... etc.
		_s32 getIntValue();
		
		//! Get a pointer to the currently selected _radio/_stickyButton... etc.
		_selector*	getActiveSelector(){ return this->activeSelector; }
		
};

#endif