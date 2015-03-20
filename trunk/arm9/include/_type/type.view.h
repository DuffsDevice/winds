#ifndef _WIN_T_VIEW_
#define _WIN_T_VIEW_

#include <_type/type.h>
#include <_type/type.gadget.h>

class _viewSwitcher;

class _view
{
	private:
		
		friend class _viewSwitcher;
		
		_viewSwitcher* viewSwitcher;
		
		// Used to set the handle to the switcher
		void setSwitcher( _viewSwitcher* switcher ){
			viewSwitcher = switcher;
		}
		
	public:
		
		//! Virtual Dtor
		virtual ~_view(){}
		
		//! Default Copy and Move ctors
		_view() : viewSwitcher( nullptr ) {}
		_view( _view&& ) = default;
		_view( const _view& ) = default;
		
		/**
		 * Applies things that are necessary for one view to show up inside the 'viewParent'
		 */
		virtual void create( _gadget* viewParent ) = 0;
		
		/**
		 * Upates the scenery (this function can be overwritten to reduce computations)
		 */
		virtual void update( _gadget* viewParent )
		{
			this->destroy( viewParent );
			this->create( viewParent );
		}
		
		/**
		 * Function that will destroy what the _view has
		 * modified on the 'viewParent' due to a previous call to 'create'
		 */
		virtual bool destroy( _gadget* viewParent ) = 0;
		
		//! Returns a reference to the _viewSwitcher it belongs to
		_viewSwitcher& getSwitcher(){
			return *this->viewSwitcher;
		}
};

#include <_type/type.view.switcher.h>

#endif