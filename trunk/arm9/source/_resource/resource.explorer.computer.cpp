#include <_resource/resource.explorer.computer.h>
#include <_gadget/gadget.fileobject.drive.h>
#include <_type/type.gadget.helpers.h>
#include <_controller/controller.filesystem.h>

_explorerPageComputer::_explorerPageComputer() :
	scrollArea(
		new _scrollArea(
			ignore , ignore , ignore , ignore , _scrollType::meta 
			, _scrollType::meta , _style::rightClickable | _style::noTransparentParts
		)
	)
{}

_callbackReturn _explorerPageComputer::mouseClickHandler( _event event )
{
	// Change Path of view-switcher
	this->getSwitcher().set( event.getGadget<_driveFileObject>()->getDirentry().getFileName() );
	
	return use_internal;
}

void _explorerPageComputer::create( _gadget* viewParent )
{
	_padding padding = this->getSwitcher().getViewPadding();
	
	// Adjust Size of scrollArea
	this->scrollArea->setInternalEventHandler( onParentAdd , _gadgetHelpers::dimensionsParent( padding ) );
	this->scrollArea->setInternalEventHandler( onParentResize , _gadgetHelpers::dimensionsParent( padding ) );
	
	// Add to viewparent
	viewParent->addChild( this->scrollArea );
	
	
	// Create all Drive-Objects
	auto parentAddCallback = _gadgetHelpers::moveBesidePrecedent( _direction::down , 30 , 2 , false , 1 , 1 , false );
	auto clickCallback = make_callback( this , &_explorerPageComputer::mouseClickHandler );
	
	for( const string& drive : _filesystemController::getDrives() )
	{
		// Create _fileDriveObject
		_driveFileObject* fO = new _driveFileObject( ignore , ignore , ignore , ignore , drive + "/" , _fileViewType::list );
		fO->setInternalEventHandler( onParentAdd , parentAddCallback );
		fO->setInternalEventHandler( onMouseDblClick , clickCallback );
		
		// Add to viewparent and to the list of gadgets to destroy
		scrollArea->addChild( fO , true );
	}
}

bool _explorerPageComputer::destroy( _gadget* viewParent )
{
	// Remove all drive objects
	this->scrollArea->removeChildren( true );
	
	// Unbind the Scroll-Area from the explorer window
	this->scrollArea->setParent( nullptr );
	
	return true;
}