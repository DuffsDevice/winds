#include <_resource/resource.program.console.h>
#include <_controller/controller.localization.h>
#include <_type/type.gadget.helpers.h>

PROG_Console::PROG_Console()
{
	_programHeader header;
	header.name = _localizationController::getBuiltInString("lbl_command_line");
	header.fileName = string("CMD");
	header.author = string("WinDS");
	header.description = string("Command Line");
	this->setHeader( move(header) );
}

void PROG_Console::main( _args args )
{
	// Window
	_mainFrame* mainFrame = _program::getMainFrame( 125 , 80 , _style::notResizeable );
	
	this->codepointMatrix = new _codepointMatrix( 0 , 0 , ignore , ignore , false );
	this->codepointMatrix->setUserEventHandler( onParentResize , _gadgetHelpers::sizeParent( 0 , 0 ) );
	this->codepointMatrix->setUserEventHandler( onParentAdd , _gadgetHelpers::eventForward(onParentResize) );
	mainFrame->addChild( this->codepointMatrix );
}

_callbackReturn PROG_Console::handler( _event event )
{
	_gadget* that = event.getGadget();
	
	this->terminate();
	
	return handled;
}