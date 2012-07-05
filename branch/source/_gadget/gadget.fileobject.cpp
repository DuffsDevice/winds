#include "_gadget/gadget.fileObject.h"
#include "_type/type.mime.h"
#include "_type/type.system.h"

_gadgetEventReturnType _fileobject::doubleClickHandler( _gadgetEvent event ){
	// Receive Gadget
	_fileobject* that = (_fileobject*)event.getGadget();
	
	that->file.execute();
	
	return handled;
}

_gadgetEventReturnType _fileobject::focusHandler( _gadgetEvent event )
{	
	// Receive Gadget
	_fileobject* that = (_fileobject*)event.getGadget();
	
	if( event.getType() == "focus" )
	{
		that->label->setBgColor( RGB255( 10 , 36 , 106 ) );
		that->label->setColor( COLOR_WHITE );
	}
	else
	{
		that->label->setBgColor( COLOR_WHITE );
		that->label->setColor( COLOR_BLACK );
	}
	
	return use_default;
}

_gadgetEventReturnType _fileobject::dragHandler( _gadgetEvent event )
{	
	// Receive Gadget
	//_fileobject* that = (_fileobject*)event.getGadget();
	
	//printf("Hallo!!!");
	
	return not_handled;
}

_fileobject::_fileobject( _coord x , _coord y , string fl , _fileviewType viewtype , _gadgetStyle style ) :
	_gadget( fileobject , 50 , _system_->_runtimeAttributes_->user->getIntAttr( "fileObjectHeight" ) , x , y , style ) , file( fl ) , viewType( viewtype ) , pressed( false )
{
	// Reset Bitamp
	this->bitmap->reset( NO_COLOR );
	
	// Register Handlers
	this->registerEventHandler( "dragging" , &_fileobject::dragHandler );
	this->registerEventHandler( "dragStart" , &_fileobject::dragHandler );
	this->registerEventHandler( "dragStop" , &_fileobject::dragHandler );
	this->registerEventHandler( "focus" , &_fileobject::focusHandler );
	this->registerEventHandler( "blur" , &_fileobject::focusHandler );
	this->registerEventHandler( "mouseDoubleClick" , &_fileobject::doubleClickHandler );
	
	switch( this->viewType )
	{
		case _fileviewType::liste:{
			// Generate...
			string ext = file.getExtension();
			
			// Certain Files do not have an .extension
			if( !_system_->_runtimeAttributes_->user->getIntAttr( "showFileExtension" ) || file.isDirectory() || !ext.length() )
				ext = "";
			else
				ext = "." + ext;
			
			this->label = new _label( 11 , 0 , file.getName() + ext );
			this->label->setHeight( _system_->_runtimeAttributes_->user->getIntAttr( "fileObjectHeight" ) );
			this->label->setVAlign( _valign::middle );
			
			const _bitmap* fileIcon = file.getFileImage();
			
			// Set Icon
			this->icon = new _imagegadget( 5 - ( fileIcon->getWidth() >> 1 ) , ( _system_->_runtimeAttributes_->user->getIntAttr( "fileObjectHeight" ) >> 1 ) - ( fileIcon->getHeight() >> 1 ) , fileIcon );
			
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