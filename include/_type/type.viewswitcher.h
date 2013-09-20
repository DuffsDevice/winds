#ifndef _WIN_T_VIEWSWITCHER_
#define _WIN_T_VIEWSWITCHER_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_type/type.view.h"
#include "_type/type.system.h"

class _viewSwitcher
{
	private:
	
		_gadget*			viewParent;
		string				currentView;
		_map<string,_view*>	views;
	
	public:
	
		//! Ctor
		_viewSwitcher( _gadget* viewParent = nullptr ) :
			viewParent( viewParent )
		{}
		
		//! Dtor
		~_viewSwitcher();
		
		//! Set view parent
		bool setViewParent( _gadget* viewParent );
		
		//! Get view parent
		_gadget* getViewParent(){
			return this->viewParent;
		}
		
		//! Adds an instance of the supplied maybe derived _view-class
		//! It can later be activated by a call to set, given the associated name you already passed to this function
		template<typename T>
		bool addView( string assocName , T&& view )
		{
			if( assocName.empty() )
				return false;
			
			// Check if we could overwrfite the currently active view
			bool isActiveView = assocName == this->currentView;
			
			// return false if the current view could not be replaced
			if( isActiveView && !unset() )
				return false;
			
			typedef typename std::remove_reference<T>::type T2;
			typedef typename T2::_view def;
			
			// Remove any Current Handler
			_view* &data = this->views[assocName]; // reference to pointer
			
			if( data )
				delete data; // Delete Current associated view
			
			// Insert the new View
			data = new T2( move(view) );
			
			// Tell it which switcher it belongs to
			data->setSwitcher( this );
			
			// Maybe reenable the old active view if we overwrote that one
			if( isActiveView && viewParent ){
				data->create( viewParent );
				this->currentView = assocName;
			}
			
			return true;
		}
		
		//! Adds an instance of the supplied maybe derived _view-class
		//! It can later be activated by a call to set, given the associated name you already passed to this function
		template<typename T>
		bool addView( string assocName , T* view )
		{
			if( assocName.empty() )
				return false;
			
			// Check if we could overwrfite the currently active view
			bool isActiveView = assocName == this->currentView;
			
			// return false if the current view could not be replaced
			if( isActiveView && !unset() )
				return false;
			
			typedef typename T::_view def;
			
			// Remove any Current Handler
			_view* &data = this->views[assocName]; // reference to pointer
			
			if( data )
				delete data; // Delete Current associated view
			
			// Insert the new View
			data = view;
			
			// Tell it which switcher it belongs to
			data->setSwitcher( this );
			
			// Maybe reenable the old active view if we overwrote that one
			if( isActiveView && viewParent ){
				data->create( viewParent );
				this->currentView = assocName;
			}
			
			return true;
		}
		
		//! Removes the view with the supplied associated name
		void removeView( string assocName );
		
		/**
		 * Deactivate the currently active view
		 * This function does nothing if no view is currently activated
		 * @return whether the view could be unset (the current view did not prevent switching)
		 **/
		bool unset();
		
		//! Get the associated Name of the currently active view
		string getActiveView(){
			return this->currentView;
		}
		
		//! Checks if the supplied view is the currently active view
		bool isActive( _view* view ){
			return this->views[this->currentView] == view;
		}
		
		//! Returns the _view object having the supplied associated name
		_view* getViewByName( string assocName ){
			return this->views[assocName];
		}
		
		//! Upadets the currently active view
		void update();
		
		/**
		 * Activate a view given the associated name of the view
		 * @note The view must have been added to the _viewSwitcher by a previous call to 'addView'
		 * @return whether the new view could be applied (the current view did not prevent switching)
		 */
		bool set( string assocName );
};

#endif