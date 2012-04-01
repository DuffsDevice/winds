#include "_gadget/gadget.fileObject.h"
#include "_type/type.mime.h"
#include "_type/type.system.h"

_gadgetEventReturnType _fileobject::doubleClickHandler( _gadgetEvent event ){
	// Receive Gadget
	_fileobject* that = (_fileobject*)event.getGadget();
	
	that->file->execute();
}

_gadgetEventReturnType _fileobject::focusHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_fileobject* that = (_fileobject*)event.getGadget();
	
	if( event.getType() == focus ){
		that->label->setBgColor( RGB255( 10 , 36 , 106 ) );
		that->label->setColor( RGB( 31 , 31 , 31 ) );
	}
	else{
		that->label->setBgColor( RGB( 31 , 31 , 31 ) );
		that->label->setColor( RGB( 0 , 0 , 0 ) );
	}
	
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
	_gadget( fileobject , 50 , _system_->_runtimeAttributes_->fileObjectHeight , x , y , style ) , file( fl ) , viewType( viewtype ) , pressed( false )
{
	// Reset Bitamp
	this->bitmap->reset( NO_COLOR );
	
	// Register Handlers
	//this->registerEventHandlers( refresh , &_fileobject::refreshHandler );
	this->registerEventHandler( dragging , &_fileobject::dragHandler );
	this->registerEventHandler( dragStart , &_fileobject::dragHandler );
	this->registerEventHandler( dragStop , &_fileobject::dragHandler );
	this->registerEventHandler( focus , &_fileobject::focusHandler );
	this->registerEventHandler( blur , &_fileobject::focusHandler );
	this->registerEventHandler( mouseDoubleClick , &_fileobject::doubleClickHandler );
	
	switch( this->viewType )
	{
		case _fileviewType::list:{
			// Generate...
			string 		ext 	= file->getExtension();
			_mimeType 	mime 	= { ext };
			// Certain Files do not have an .extension
			if( !_system_->_runtimeAttributes_->showFileExtension || file->isDirectory() || !ext.length() )
				ext = "";
			else
				ext = "." + ext;
			
			this->label = new _label( 11 , 0 , file->getName() + ext );
			this->label->setHeight( _system_->_runtimeAttributes_->fileObjectHeight );
			this->label->setVAlign( middle );
			
			const _bitmap* fileIcon;
			
			if( file->isDirectory() )
				fileIcon = mime.getFolderImage();
			else
				fileIcon = mime.getFileImage();
			
			// Set Icon
			this->icon = new _imagegadget( 5 - ( fileIcon->getWidth() >> 1 ) , ( _system_->_runtimeAttributes_->fileObjectHeight >> 1 ) - ( fileIcon->getHeight() >> 1 ) , fileIcon );
			
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