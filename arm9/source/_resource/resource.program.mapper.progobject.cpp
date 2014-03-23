#include "_resource/resource.program.mapper.progobject.h"
#include "_type/type.gadget.helpers.h"
#include "_type/type.direntry.h"
#include "_type/type.system.h"

_callbackReturn PROG_Mapper_Object::refreshHandler( _event event )
{
	PROG_Mapper_Object* that = event.getGadget<PROG_Mapper_Object>();
	
	// Fetch BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( _system::getRTA().getItemBackground( that->hasFocus() , that->isSelected() ) );
	
	// Copy Program Image
	bP.copyTransparent( 1 , 4 , that->progImage );
	
	_color foreground = _system::getRTA().getItemForeground( that->hasFocus() , that->isSelected() );
	
	// Draw Description
	if( that->progAuthor.empty() ) // Draw Name only
		bP.drawString( 14 , 4 , _system::getFont() , that->progName , foreground );
	else
	{
		// Draw Name & Author
		bP.drawString( 14 , 0 , _system::getFont() , that->progName , foreground );
		bP.drawString( 14 , 8 , _system::getFont() , that->progAuthor , that->isSelected() ? foreground : _color::gray );
	}
	
	return use_default;
}


PROG_Mapper_Object::PROG_Mapper_Object( _length width , string path ) :
	_gadget( _gadgetType::none , ignore , ignore , width , 17 )
	, programFile( path )
{
	// Set Handlers
	this->setUserEventHandler( onDraw , make_callback( &PROG_Mapper_Object::refreshHandler ) );
	this->setUserEventHandler( onFocus , _gadgetHelpers::eventForwardRefresh() );
	this->setUserEventHandler( onBlur , _gadgetHelpers::eventForwardRefresh() );
	this->setUserEventHandler( onSelect , _gadgetHelpers::eventForwardRefresh() );
	this->setUserEventHandler( onDeselect , _gadgetHelpers::eventForwardRefresh() );
	
	// Set Neccesary Information
	_program* progObject = _program::fromFile( programFile.getFileName() );
	if( progObject )
	{
		// Fetch Header of program
		_programHeader& header = progObject->getHeader();
		
		// Set Name to display
		if( header.name && !header.name->empty() )
			this->progName = move( *header.name );
		else if( header.displayName && !header.displayName->empty() )
			this->progName = move( *header.displayName );
		else
			this->progName = programFile.getDisplayName();
		
		// Set Author
		if( header.author && !header.author->empty() )
			progAuthor = move( *header.author );
		
		delete progObject;
	}
	else
		this->progName = programFile.getDisplayName();
	
	// Set Program Image
	this->progImage = programFile.getFileImage();
	
	// Redraw
	this->redraw();
}