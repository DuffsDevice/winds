#include "_gadget/gadget.windows.programswitcher.h"
#include "_type/type.system.h"

void _windowsProgramSwitcher::taskChangeHandler( _window* ref )
{
	_imageGadget* img = nullptr;
	
	for( _gadget* g : this->children )
		if( g->getStyle().data == ref )
			img = (_imageGadget*)g;
	
	// Still need an image connected to the task?
	if( ref->isTask() )
	{
		// Allocate a new imagegadget to hold the new task window icon
		if( !img ){
			this->addChild( new _imageGadget( 0 , 0 , ref->getIcon() , ignore , ignore , _style::storeHandle(ref) ) , true );
			this->update();
		}
		// Modify the already existing icon
		else{
			img->setImage( ref->getIcon() );
			if( ref->hasFocus() ) // if needed adjust the label
				this->label->setStrValue( ref->getStrValue() );
		}
	}
	else // Not needed
		delete img;
}


_callbackReturn _windowsProgramSwitcher::keyHandler( _event event )
{
	_windowsProgramSwitcher* that = event.getGadget<_windowsProgramSwitcher>();
	
	if( event.getKeyCode() == _key::r )
	{
		auto end = _window::getTaskWindows().end();
		auto begin = _window::getTaskWindows().begin();
		
		_gadget* active = that->getOwner();
		
		if( !active )
			that->setOwner( _window::getTaskWindows().front() );
		else
		{
			_list<_window*>::const_iterator it = find_if( begin , end , [active]( _window* wnd )->bool{ return wnd == active; } );
			if( it == end || ++it == end )
				that->setOwner( _window::getTaskWindows().front() );
			else
				that->setOwner( *it );
		}
		_window* wnd = (_window*)that->getOwner();
		that->label->setStrValue( wnd ? wnd->getStrValue() : "" );
	}
	else
		return use_default;
	
	that->redraw();
	
	return handled;
}

_callbackReturn _windowsProgramSwitcher::updateHandler( _event event )
{
	_windowsProgramSwitcher* that = event.getGadget<_windowsProgramSwitcher>();
	
	if( event == onOpen ){
		that->setOwner( _system::_gadgetHost_->getFocusedChild() );
	}
	
	_list<_window*> activeTasks = _window::getTaskWindows();
	
	_length width = max( 50 , (int) activeTasks.size() * 10 );
	
	// Set Switcher Size
	that->setSize( width + 8 , 23 );
	
	_coord x0 = ( width - activeTasks.size() * 10 ) / 2;
	for( _gadget* g : that->children ){
		if( g->getType() == _gadgetType::label )
			continue;
		g->setX( x0 + 1 );
		x0 += 10;
	}
	
	_window* wnd = (_window*)that->getOwner();
	
	that->label->setStrValue( wnd ? wnd->getStrValue() : "----" );
	that->label->setWidth( width );
	
	return handled;
}

_callbackReturn _windowsProgramSwitcher::refreshHandler( _event event )
{
	_windowsProgramSwitcher* that = event.getGadget<_windowsProgramSwitcher>();
	
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myW = bP.getWidth();
	_length myH = bP.getHeight();
	
	bP.fill( _system::getRTA().getControlBackground() );
	
	bP.drawHorizontalLine( 0 , 0 , myW , COLOR_WHITE );
	bP.drawHorizontalLine( 0 , myH - 1 , myW , _system::getRTA().getControlForeground() );
	bP.drawVerticalLine( 0 , 0 , myH , COLOR_WHITE );
	bP.drawVerticalLine( myW - 1 , 0 , myH , _system::getRTA().getControlForeground() );
	
	_imageGadget* img = nullptr;
	_window* wnd = (_window*)that->getOwner();
	
	for( _gadget* g : that->children )
		if( g->getStyle().data == wnd )
			img = (_imageGadget*)g;
	
	if( img ){
		_coord x = img->getX();
		bP.drawRect( x + 2 , 2 , 10 , 10 , _system::getRTA().getItemBackground( true ) );
	}
	
	return use_default;
}

_windowsProgramSwitcher::_windowsProgramSwitcher( _style&& style ) :
	_popup( 40 , 40 , nullptr , (_style&&)style )
	, label( new _label( 0 , 9 , ignore , ignore , "----" ) )
{
	_window::addTaskHandler( make_callback( this , &_windowsProgramSwitcher::taskChangeHandler ) );
	
	this->setPadding( _padding(4,4,4,1) );
	
	this->setInternalEventHandler( onOpen , make_callback( &_windowsProgramSwitcher::updateHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_windowsProgramSwitcher::refreshHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_windowsProgramSwitcher::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_windowsProgramSwitcher::keyHandler ) );
	
	this->label->setAlign( _align::center );
	this->addChild( this->label );
}

_windowsProgramSwitcher::~_windowsProgramSwitcher(){
	_window::removeTaskHandler( make_callback( this , &_windowsProgramSwitcher::taskChangeHandler ) );
	delete this->label;
}