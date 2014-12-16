#include <_type/type.view.switcher.h>

_viewSwitcher::~_viewSwitcher()
{
	// Clear active view
	this->unset();
	
	// Delete callbacks
	for( _pair<string,_view*> view : this->views )
		delete view.second;
	
	// Clear list
	this->views.clear();
}

bool _viewSwitcher::addView( string assocName , _view* view )
{
	if( assocName.empty() )
		return false;
	
	// Check if we could overwrfite the currently active view
	bool isActiveView = assocName == this->currentView;
	
	// return false if the current view could not be replaced
	if( isActiveView && !unset() )
		return false;
	
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

void _viewSwitcher::removeView( string assocName )
{
	if( this->currentView == assocName )
		unset();
	
	// Unbind the view from the switcher
	_map<string,_view*>::iterator oldViewIter = this->views.find( assocName );
	
	if( oldViewIter != this->views.end() && oldViewIter->second )
	{
		delete oldViewIter->second;
		this->views.erase( oldViewIter );
	}
}

bool _viewSwitcher::setViewParent( _gadget* viewParent )
{
	if( this->viewParent == viewParent )
		return true;
	
	// Remove current view
	if( !unset() )
		return false;
	
	this->viewParent = viewParent;
	
	// Re initialize
	if( !this->currentView.empty() )
	{
		// Get current view
		_view* view = this->views[this->currentView];
		
		// Repaint that one
		view->create( this->viewParent );
	}
	
	return true;
}

bool _viewSwitcher::set( string assocName )
{
	// Check if the user wants to activate
	// a view that is already activated
	if( this->currentView == assocName )
		return false;
	
	// Can we detroy the old view?
	if( !this->unset() )
		return false;
	
	// Apply new view
	_view* newView = this->getViewByName( assocName );
	
	if( newView && viewParent )
	{
		newView->create( viewParent );
		this->currentView = move( assocName );
	}
	
	return true;
}

bool _viewSwitcher::unset()
{
	if( !this->currentView.empty() && viewParent )
	{
		// Clear the content of the view (or at least try to)
		bool deleteSuccess = this->views[ this->currentView ]->destroy( viewParent );
		
		if( deleteSuccess )
			this->currentView.clear();
		
		return deleteSuccess;
	}
	
	return true;
}

void _viewSwitcher::update()
{
	if( this->currentView.empty() || !viewParent )
		return;
	
	// Get current view
	_view* view = this->views[this->currentView];
	
	if( !view )
		return;
	
	// Update that one
	view->update( this->viewParent );
}