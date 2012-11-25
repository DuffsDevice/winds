#include "_gadget/gadget.fileObject.h"
#include "_type/type.mime.h"
#include "_type/type.system.h"

_callbackReturn _fileobject::doubleClickHandler( _event event ){
	// Receive Gadget
	_fileobject* that = event.getGadget<_fileobject>();
	
	that->file.execute();
	
	return handled;
}

_callbackReturn _fileobject::focusHandler( _event event )
{	
	// Receive Gadget
	_fileobject* that = event.getGadget<_fileobject>();
	
	if( event.getType() == onFocus )
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

_callbackReturn _fileobject::dragHandler( _event event )
{	
	// Receive Gadget
	//_fileobject* that = event.getGadget<_fileobject>();
	
	return not_handled;
}

_fileobject::_fileobject( _coord x , _coord y , string fl , _fileviewType viewtype , _style style ) :
	_gadget( _gadgetType::fileobject , 50 , _system::_runtimeAttributes_->user->fOH , x , y , style ) , file( fl ) , viewType( viewtype ) , pressed( false )
{
	// Reset Bitamp
	this->bitmap.reset( NO_COLOR );
	
	// Register Handlers
	this->registerEventHandler( dragging , &_fileobject::dragHandler );
	this->registerEventHandler( dragStart , &_fileobject::dragHandler );
	this->registerEventHandler( dragStop , &_fileobject::dragHandler );
	this->registerEventHandler( onFocus , &_fileobject::focusHandler );
	this->registerEventHandler( onBlur , &_fileobject::focusHandler );
	this->registerEventHandler( mouseDoubleClick , &_fileobject::doubleClickHandler );
	
	switch( this->viewType )
	{
		case _fileviewType::liste:{
			// Generate...
			string ext = file.getExtension();
			
			// Certain Files do not have an .extension
			if( !_system::_runtimeAttributes_->user->sFE || file.isDirectory() || !ext.length() )
				ext = "";
			else
				ext = "." + ext;
			
			this->label = new _label( 11 , 0 , file.getName() + ext );
			this->label->setHeight( _system::_runtimeAttributes_->user->fOH );
			this->label->setVAlign( _valign::middle );
			
			const _bitmap& fileIcon = file.getFileImage();
			
			// Set Icon
			this->icon = new _imagegadget( 5 - ( fileIcon.getWidth() >> 1 ) , ( _system::_runtimeAttributes_->user->fOH >> 1 ) - ( fileIcon.getHeight() >> 1 ) , fileIcon );
			
			// Resize
			_rect dim = this->getDimensions();
			dim.width = this->label->getWidth() + 12;
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