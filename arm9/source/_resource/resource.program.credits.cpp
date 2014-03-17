#include "_resource/resource.program.credits.h"
#include "_resource/resource.image.logo.computer.h"
#include "_resource/resource.icon.winds.h"
#include "_type/type.gadget.helpers.h"

PROG_Credits::PROG_Credits()
{
	_programHeader header;
	header.fileIcon = BMP_WinDSIcon();
	header.name = string("WinDS Credentials");
	header.displayName = string("Credits");
	header.author = string("DuffsDevice");
	header.description = string("Information about the used Hardware and the current Operation System");
	this->setHeader( header );
}

void PROG_Credits::main( _programArgs args )
{
	static const _coord startX1 = 63;
	static const _coord startX2 = 70;
	
	// Window
	this->window = new _window( 40 , 15 , 150 , 150 , _system::getLocalizedString("lbl_about_winds") , true , true , _style::draggable | _style::notResizeable );
	this->window->setUserEventHandler( onClose , make_callback( this , &PROG_Credits::handler ) );
	this->window->setBgColor( _color::white );
	
	// Logo
	this->winLogo = new _imageGadget( 8 , 20 , BMP_ComputerLogo() );
	
	// OK-Button
	this->okButton = new _button( ignore , ignore , ignore , ignore , _system::getLocalizedString("lbl_ok") );
	this->okButton->setUserEventHandler( onMouseClick , make_callback( this , &PROG_Credits::handler ) );
	this->okButton->setUserEventHandler( onParentAdd , _gadgetHelpers::rightBottomAlign( 1 , 1 ) );
	this->okButton->setAutoSelect( true );
	
	// Heading 1
	this->heading1 = new _label( startX1 , 4 , ignore , ignore , _system::getLocalizedString("def_system") );
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
		this->value1_4 = new _label( startX2 , 41 , ignore , ignore , _system::getLocalizedString("lbl_reimpl") );
	
	// Heading 2
	this->heading2 = new _label( startX1 , 52 , ignore , ignore , _system::getLocalizedString("def_hardware") );
		this->value2_1 = new _label( startX2 , 62 , ignore , ignore , "Nintendo® DS" );
		this->value2_2 = new _label( startX2 , 71 , ignore , ignore , "ARM9 @ 67MHz" );
		this->value2_3 = new _label( startX2 , 80 , ignore , ignore , "ARM7 @ 33MHz" );
		this->value2_4 = new _label( startX2 , 89 , ignore , ignore , "4MB of RAM" );
	
	// Heading 3
	this->heading3 = new _label( startX1 , 100 , ignore , ignore , _system::getLocalizedString("def_author") );
		this->value3_1 = new _label( startX2 , 109 , ignore , ignore , "DuffsDevice" );
		this->value3_1->setFont( _system::getFont("Handwriting9") );
	
	// Add to the dom tree
	this->window->addChild( this->heading1 );
		this->window->addChild( this->value1_1 );
		this->window->addChild( this->value1_2 );
		this->window->addChild( this->value1_3 );
		this->window->addChild( this->value1_4 );
	this->window->addChild( this->heading2 );
		this->window->addChild( this->value2_1 );
		this->window->addChild( this->value2_2 );
		this->window->addChild( this->value2_3 );
		this->window->addChild( this->value2_4 );
	this->window->addChild( this->heading3 );
		this->window->addChild( this->value3_1 );
	
	this->window->addChild( this->okButton );
	this->window->addChild( this->winLogo );
	this->gadgetHost->addChild( this->window );
}

void PROG_Credits::destruct()
{
	if( this->window )			delete this->window;
	if( this->okButton )		delete this->okButton;
	if( this->winLogo )			delete this->winLogo;
	if( this->heading1 )		delete this->heading1;
		if( this->value1_1 )	delete this->value1_1;
		if( this->value1_2 )	delete this->value1_2;
		if( this->value1_3 )	delete this->value1_3;
		if( this->value1_4 )	delete this->value1_4;
	if( this->heading2 )		delete this->heading2;
		if( this->value2_1 )	delete this->value2_1;
		if( this->value2_2 )	delete this->value2_2;
		if( this->value2_3 )	delete this->value2_3;
		if( this->value2_4 )	delete this->value2_4;
	if( this->heading3 )		delete this->heading3;
		if( this->value3_1 )	delete this->value3_1;
}

_callbackReturn PROG_Credits::handler( _event event )
{
	this->terminate();
	
	return handled;
}