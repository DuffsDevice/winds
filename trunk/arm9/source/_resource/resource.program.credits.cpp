#include "_resource/resource.program.credits.h"
#include "_resource/resource.image.logo.computer.h"
#include "_resource/resource.icon.winds.h"
#include "_type/type.gadget.helpers.h"
#include "_controller/controller.font.h"
#include "_controller/controller.localization.h"
#include "_controller/controller.debug.h"
#include "_type/type.windows.h"
#include "_gadget/gadget.window.dialog.h"

PROG_Credits::PROG_Credits()
{
	_programHeader header;
	header.fileIcon = BMP_WinDSIcon();
	header.name = _localizationController::getBuiltInString("lbl_about_winds");
	header.fileName = string("Credits");
	header.author = string("WinDS");
	header.description = string("Information about the used Hardware and the current Operation System");
	this->setHeader( move(header) );
}

void PROG_Credits::main( _programArgs args )
{
	static const _coord startX1 = 63;
	static const _coord startX2 = 70;
	
	// Window
	_mainFrame* mainFrame = _program::getMainFrame( 150 , 150 , _style::notResizeable );
	mainFrame->setUserEventHandler( onClose , make_callback( this , &PROG_Credits::handler ) );
	mainFrame->setUserEventHandler( onKeyDown , make_callback( &_dialogWindow::submitEscapeHandler ) );
	mainFrame->setBgColor( _color::white );
	
	// Logo
	this->winLogo = new _imageGadget( 8 , 20 , BMP_ComputerLogo() );
	
	// OK-Button
	this->okButton = new _button( ignore , ignore , ignore , ignore , _localizationController::getBuiltInString("lbl_ok") );
	this->okButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_Credits::handler ) );
	this->okButton->setUserEventHandler( onParentAdd , _gadgetHelpers::rightBottomAlign( 1 , 1 ) );
	this->okButton->setAutoSelect( true );
	
	// Heading 1
	this->heading1 = new _label( startX1 , 4 , ignore , ignore , _localizationController::getBuiltInString("def_system") );
		string versionStr = "v. ";
		versionStr += int2string( _WIN_VER_MAJOR_ );
		versionStr += '.';
		versionStr += int2string( _WIN_VER_MINOR_ );
		versionStr += " (";
		versionStr += _WIN_VER_STATE_;
		versionStr += ')';
		
		this->value1_1 = new _label( startX2 , 14 , ignore , ignore , "WinDS" );
		this->value1_2 = new _label( startX2 , 23 , ignore , ignore , move(versionStr) );
		this->value1_3 = new _label( startX2 , 32 , ignore , ignore , "MS Windows® XP");
		this->value1_4 = new _label( startX2 , 41 , ignore , ignore , _localizationController::getBuiltInString("lbl_reimpl") );
	
	// Heading 2
	this->heading2 = new _label( startX1 , 52 , ignore , ignore , _localizationController::getBuiltInString("def_hardware") );
		this->value2_1 = new _label( startX2 , 62 , ignore , ignore , "Nintendo® DS" );
		this->value2_2 = new _label( startX2 , 71 , ignore , ignore , "ARM9 @ 67MHz" );
		this->value2_3 = new _label( startX2 , 80 , ignore , ignore , "ARM7 @ 33MHz" );
		this->value2_4 = new _label( startX2 , 89 , ignore , ignore , "4MB of RAM" );
	
	// Heading 3
	this->heading3 = new _label( startX1 , 100 , ignore , ignore , _localizationController::getBuiltInString("def_author") );
		this->value3_1 = new _label( startX2 , 109 , ignore , ignore , "DuffsDevice" );
		this->value3_1->setFont( _fontController::getFont("Handwriting9") );
	
	// Add to the dom tree
	mainFrame->addChild( this->heading1 );
		mainFrame->addChild( this->value1_1 );
		mainFrame->addChild( this->value1_2 );
		mainFrame->addChild( this->value1_3 );
		mainFrame->addChild( this->value1_4 );
	mainFrame->addChild( this->heading2 );
		mainFrame->addChild( this->value2_1 );
		mainFrame->addChild( this->value2_2 );
		mainFrame->addChild( this->value2_3 );
		mainFrame->addChild( this->value2_4 );
	mainFrame->addChild( this->heading3 );
		mainFrame->addChild( this->value3_1 );
	
	mainFrame->addChild( this->okButton );
	mainFrame->addChild( this->winLogo );
}

_callbackReturn PROG_Credits::handler( _event event )
{
	this->terminate();
	
	return handled;
}