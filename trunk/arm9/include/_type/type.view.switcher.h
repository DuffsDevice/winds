#ifndef _WIN_T_VIEWSWITCHER_
#define _WIN_T_VIEWSWITCHER_

#include <_type/type.h>
#include <_type/type.gadget.h>
#include <_type/type.view.h>

class _viewSwitcher
{
	private:
	
		_gadget*						viewParent;
		_padding						viewPadding;
		string							currentView;
		_map<string,_uniquePtr<_view>>	views;
		
		//! Function to be overloaded from a child class, will be called, in case the view changes
		virtual string beforeChange( string newViewName ) const { return string(); }
	
	public:
	
		//! Ctor
		_viewSwitcher( _gadget* viewParent = nullptr , _padding viewPadding = _padding(0) ) :
			viewParent( viewParent )
			, viewPadding( viewPadding )
		{}
		
		//! Dtor
		~_viewSwitcher();
		
		//! Set view parent
		bool setViewParent( _gadget* viewParent );
		
		//! Get view parent
		_gadget* getViewParent() const {
			return this->viewParent;
		}
		
		//! Set view padding
		void setViewPadding( _padding viewPadding ){
			if( this->viewPadding == viewPadding )
				return;
			this->viewPadding = viewPadding;
			update();
		}
		
		//! Get view padding
		_padding getViewPadding() const {
			return this->viewPadding;
		}
		
		//! Adds an instance of the supplied maybe derived _view-class
		//! It can later be activated by a call to set, given the associated name you already passed to this function
		bool addView( string assocName , _paramAlloc<_view> view );
		
		//! Removes the view with the supplied associated name
		void removeView( string assocName );
		
		//! Get the associated Name of the currently active view
		string getActiveView() const {
			return this->currentView;
		}
		
		//! Checks if the supplied view is the currently active view
		bool isActive( _view* view ) const {
			return this->getViewByName( this->currentView ) == view;
		}
		
		//! Returns the _view object having the supplied associated name
		virtual _view* getViewByName( string assocName ) const {
			const auto& iter = this->views.find(assocName);
			return iter != this->views.end() ? iter->second.get() : nullptr ;
		}
		
		//! Updates the currently active view
		void update();
		
		/**
		 * Activate a view given the associated name of the view
		 * @note The view must have been added to the _viewSwitcher by a previous call to 'addView'
		 * @return whether the new view could be applied (the current view did not prevent switching)
		 */
		bool set( string assocName );
		
		/**
		 * Deactivate the currently active view
		 * This function does nothing if no view is currently activated
		 * @return whether the view could be unset (the current view did not prevent switching)
		 **/
		bool unset();
};

#endif