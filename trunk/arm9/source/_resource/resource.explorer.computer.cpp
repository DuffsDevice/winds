#include <_resource/resource.explorer.computer.h>
#include <_gadget/gadget.fileobject.drive.h>
#include <_type/type.gadget.helpers.h>
#include <_controller/controller.filesystem.h>

void _explorerPageComputer::create( _gadget* viewParent )
{
	auto cb = _gadgetHelpers::moveBesidePrecedent( _direction::down , 30 , 2 , false , 1 , 1 , false );
	
	_vector<string> drives = _filesystemController::getDrives();
	
	for( const string& drive : drives )
	{
		// Create _fileDriveObject
		_driveFileObject* fO = new _driveFileObject( ignore , ignore , ignore , ignore , drive , _fileViewType::list );
		fO->setInternalEventHandler( onParentAdd , cb );
		
		// Add to viewparent and to the list of gadgets to destroy
		gadgetsToDestroy.push_back( fO );
		viewParent->addChild( fO , true );
	}
}

bool _explorerPageComputer::destroy( _gadget* viewParent )
{
	for( _gadget* gadget : gadgetsToDestroy )
		delete gadget;
	return true;
}