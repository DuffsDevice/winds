#ifndef _WIN_RESOURCE_EXPLORER_COMPUTER_
#define _WIN_RESOURCE_EXPLORER_COMPUTER_

#include <_type/type.h>
#include <_type/type.view.h>

class _explorerPageComputer : public _view
{
	private:
		
		_vector<_gadget*> gadgetsToDestroy;
		
	public:
		
		void create( _gadget* viewParent );
		bool destroy( _gadget* viewParent );
};

#endif