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

bool _viewSwitcher::addView( string assocName , _paramAlloc<_view> view )
{
	if( assocName.empty() )
		return false;
	
	// Check if we could overwrfite the currently active view
	bool isActiveView = assocName == this->currentView;
	
	// return false if the current view could not be replaced
	if( isActiveView && !unset() )
		return false;
	
	// Remove any Current Handler
	_uniquePtr<_view>& data = this->views[assocName]; // reference to pointer
	
	// Override the old view with the new View
	data = view;
	
	// Tell it which switcher it belongs to
	data->setSwitcher( this );
	
	// Maybe reenable the old active view if we overwrote that one
	if( isActiveView && viewParent )
	{
		// Set name of currently active view
		this->currentView = move(assocName);
		
		// Let the View do its job
		data->create( viewParent );
	}
	
	return true;
}

void _viewSwitcher::removeView( string assocName )
{
	if( this->currentView == assocName )
		unset();
	
	// Unbind the view from the switcher
	const auto& oldViewIter = this->views.find( assocName );
	
	if( oldViewIter != this->views.end() && oldViewIter->second )
		this->views.erase( oldViewIter );
}

bool _viewSwitcher::setViewParent( _gadget* viewParent )
{
	if( this->viewParent == viewParent )
		return true;
	
	string oldAssocName = this->currentView;
	
	// Remove current view
	if( !unset() )
		return false;
	
	this->viewParent = viewParent;
	
	// Re-initialize
	_view* oldView = oldAssocName.empty() ? nullptr : getViewByName( oldAssocName );
	
	if( !oldView || !viewParent )
		return true;
	
	// Make Sure, the Switcher instance is properly set
	oldView->setSwitcher( this );
	
	// Set Name of currently active view
	this->currentView = move( oldAssocName );
	
	// Repaint that the active one
	oldView->create( this->viewParent );
	
	return true;
}

bool _viewSwitcher::set( string assocName )
{
	// Check if the user wants to activate
	// a view that is already activated
	if( this->currentView == assocName )
		return false;
	
	// Apply new view
	_view* newView = assocName.empty() ? nullptr : this->getViewByName( assocName );
	
	if( newView && viewParent )
	{
		// Inform about view change, and check, if the callback
		// is happy with the supplied new view name
		string modification = this->beforeChange( assocName );
		
		if( modification.empty() || modification == assocName )
		{
			// Can we detroy the old view?
			_view* currentView = this->currentView.empty() ? nullptr : getViewByName( this->currentView );
			if( currentView && !currentView->destroy( viewParent ) )
				return false;
			
			// Make Sure, the Switcher instance is properly set
			newView->setSwitcher( this );
			
			// Let the View do its job
			newView->create( viewParent );
			
			// Set Current View to the assoc name
			// that this view was activated with
			this->currentView = move( assocName );
		}
		else
			return this->set( modification );
	}
	
	return true;
}

bool _viewSwitcher::unset()
{
	_view* currentView = this->currentView.empty() ? nullptr : getViewByName( this->currentView );
	
	if( !currentView || !viewParent )
		return true;
	
	// Clear the content of the view (or at least try to)
	bool deleteSuccess = currentView->destroy( viewParent );
	
	if( deleteSuccess )
		this->currentView.clear();
	
	return deleteSuccess;
}

void _viewSwitcher::update()
{
	// Get current view
	_view* currentView = this->currentView.empty() ? nullptr : getViewByName( this->currentView );
	
	if( !currentView || !viewParent )
		return;
	
	// Update that one
	currentView->update( this->viewParent );
}