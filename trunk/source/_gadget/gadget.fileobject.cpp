#include "_gadget/gadget.fileObject.h"
#include "_type/type.mime.h"

_gadgetEventReturnType _fileobject::mouseHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_fileobject* that = (_fileobject*)event.getGadget();
	
	//printf("Hallo!!!");
	
	return not_handled;//use_default;
}

_gadgetEventReturnType _fileobject::dragHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_fileobject* that = (_fileobject*)event.getGadget();
	
	//printf("Hallo!!!");
	
	return not_handled;//use_default;
}

_fileobject::_fileobject( _coord x , _coord y , _file* fl , _fileviewType viewtype , _gadgetStyle style ) :
	_gadget( fileobject , 50 , 10 , x , y , style ) , file( fl ) , viewType( viewtype )
{
	// Reset Bitamp
	this->bitmap->reset( NO_COLOR );
	
	// Register Handlers
	//this->registerEventHandlers( refresh , &_fileobject::refreshHandler );
	this->registerEventHandler( dragging , &_fileobject::dragHandler );
	this->registerEventHandler( dragStart , &_fileobject::dragHandler );
	this->registerEventHandler( dragStop , &_fileobject::dragHandler );
	this->registerEventHandler( mouseDown , &_fileobject::mouseHandler );
	this->registerEventHandler( mouseUp , &_fileobject::mouseHandler );
	this->registerEventHandler( mouseClick , &_fileobject::mouseHandler );
	
	switch( this->viewType )
	{
		case _fileviewType::list:{
			// Generate...
			string 		ext 	= file->getExtension();
			_mimeType 	mime 	= { ext };
			// Certain Files do not have an .extension
			if( !_defaultRuntimeAttributes_.showFileExtension || file->isDirectory() || !ext.length() )
				ext = "";
			else
				ext = "." + ext;
			
			this->label = new _label( 11 , 0 , file->getName() + ext );
			const _bitmap* fileIcon = mime.getFileImage();
			
			// Set Icon
			this->icon = new _imagegadget( 5 - ( fileIcon->getWidth() >> 1 ) , 0 , fileIcon );
			
			// Resize
			_rect dim = this->getDimensions();
			dim.setWidth( this->label->getWidth() + 12 );
			this->setDimensions( dim );
			
			// Add Child
			this->addChild( this->label );
			this->addChild( this->icon );
			break;
		}
		default:
			break;
	};
	
	// Refresh...
	this->refreshBitmap();
}