#include <_gadget/gadget.window.mainframe.h>
#include <_type/type.program.h>
#include <_controller/controller.gui.h>

wstring _mainFrame::getDisplayName( _programHandle handle )
{
	_program* prog = handle.get();
	if( !prog )
		return "";
	
	_programHeader& header = prog->getHeader();
	if( header.windowName )
		return header.windowName;
	if( header.name )
		return header.name;
	
	return "";
}

_bitmap _mainFrame::getIcon( _programHandle handle )
{
	_program* prog = handle.get();
	
	if( prog && prog->getHeader().windowIcon )
		return prog->getHeader().windowIcon;
	
	return _bitmap();
}

_callbackReturn _mainFrame::closeHandler( _event event )
{
	_mainFrame* that = event.getGadget<_mainFrame>();
	
	if( that->programHandle.isValid() )
		that->programHandle->terminate();
	
	return handled;
}

_mainFrame::_mainFrame( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _programHandle progHandle , _style&& style ) :
	_window( x , y , width , height , _mainFrame::getDisplayName( progHandle ) , _mainFrame::getIcon( progHandle ) , true , true , (_style&&)style )
	, programHandle( move(progHandle) )
{
	_window::label->setEllipsis( true );
	this->setType( _gadgetType::mainframe );
	
	// Register Close-Handler
	this->setInternalEventHandler( onDelete , make_callback( &_mainFrame::closeHandler ) );
	this->setInternalEventHandler( onParentRemove , make_callback( &_mainFrame::closeHandler ) );
	
	// Add To Host
	this->setParent( _guiController::getHost() );
}

void _mainFrame::setTitle( wstring title )
{
	if( !this->programHandle.isValid() )
		return;
	
	wstring progName = _mainFrame::getDisplayName( programHandle );
	
	if( title.empty() ){
		_window::label->setEllipsis( true );
		this->setStrValue( progName );
	}
	else{
		this->setStrValue( title + " - " + progName );
		_window::label->setEllipsis( true , progName.length() + 3 );
	}
}