#include "_gadget/gadget.programswitcher.h"
#include "_controller/controller.gui.h"

void _programSwitcher::taskChangeHandler( _window* ref )
{
	_imageGadget* img = nullptr;
	
	for( _gadget* g : this->getChildren( false ) )
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


_callbackReturn _programSwitcher::keyHandler( _event event )
{
	_programSwitcher* that = event.getGadget<_programSwitcher>();
	
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

_callbackReturn _programSwitcher::updateHandler( _event event )
{
	_programSwitcher* that = event.getGadget<_programSwitcher>();
	
	_gadget* focusedChild = _guiController::getHost()->getFocusedChild();
	
	if( event == onOpen && focusedChild && focusedChild->getType() == _gadgetType::window && focusedChild->hasFocus() ){
		that->setOwner( focusedChild );
	}
	
	_list<_window*> activeTasks = _window::getTaskWindows();
	
	_length width = max( 50 , (int) activeTasks.size() * 10 );
	
	// Set Switcher Size
	that->setSize( width + 8 , 23 );
	
	_coord x0 = ( width - activeTasks.size() * 10 ) / 2;
	for( _gadget* g : that->getChildren( false ) ){
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

_callbackReturn _programSwitcher::closeHandler( _event event )
{
	_programSwitcher* that = event.getGadget<_programSwitcher>();
	
	_window* wnd = (_window*)that->getOwner();
	
	if( wnd && wnd->isMinimized() )
		wnd->restore();
	
	return handled;
}

_callbackReturn _programSwitcher::refreshHandler( _event event )
{
	_programSwitcher* that = event.getGadget<_programSwitcher>();
	
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myW = bP.getWidth();
	_length myH = bP.getHeight();
	
	bP.fill( _guiController::getControlBg() );
	
	bP.drawHorizontalLine( 0 , 0 , myW , _color::white );
	bP.drawHorizontalLine( 0 , myH - 1 , myW , _guiController::getControlFg() );
	bP.drawVerticalLine( 0 , 0 , myH , _color::white );
	bP.drawVerticalLine( myW - 1 , 0 , myH , _guiController::getControlFg() );
	
	_imageGadget* img = nullptr;
	_window* wnd = (_window*)that->getOwner();
	
	for( _gadget* g : that->getChildren( false ) )
		if( g->getStyle().data == wnd )
			img = (_imageGadget*)g;
	
	if( img ){
		_coord x = img->getX();
		bP.drawRect( x + 2 , 2 , 10 , 10 , _guiController::getItemBg( true ) );
	}
	
	return use_default;
}

_programSwitcher::_programSwitcher( _style&& style ) :
	_popup( 40 , 40 , nullptr , (_style&&)style )
	, label( new _label( 0 , 9 , ignore , ignore , "----" ) )
{
	_window::addTaskHandler( make_callback( this , &_programSwitcher::taskChangeHandler ) );
	
	this->setPadding( _padding(4,4,4,1) );
	
	this->setInternalEventHandler( onOpen , make_callback( &_programSwitcher::updateHandler ) );
	this->setInternalEventHandler( onClose , make_callback( &_programSwitcher::closeHandler ) );
	this->setInternalEventHandler( onDraw , make_callback( &_programSwitcher::refreshHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_programSwitcher::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_programSwitcher::keyHandler ) );
	
	this->label->setAlign( _align::center );
	this->addChild( this->label );
}

_programSwitcher::~_programSwitcher()
{
	_window::removeTaskHandler( make_callback( this , &_programSwitcher::taskChangeHandler ) );
	
	// First, remove label, thet delete children, because the label would else be deleted twice!
	delete this->label;
	this->removeChildren( true );
}