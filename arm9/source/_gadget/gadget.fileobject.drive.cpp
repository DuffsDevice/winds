#include <_gadget/gadget.fileobject.drive.h>
#include <_gadget/gadget.fileview.h>
#include <_gadget/gadget.image.h>
#include <_type/type.gadget.helpers.h>
#include <_type/type.windows.h>
#include <_controller/controller.font.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.localization.h>
#include <_controller/controller.filesystem.h>
#include <_resource/resource.icon.harddrive.h>
#include <_resource/resource.icon.removabledrive.h>

_callbackReturn _driveFileObject::refreshHandler( _event event )
{
	// Receive Gadget
	_driveFileObject* that = event.getGadget<_driveFileObject>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	switch( that->viewType )
	{
		case _fileViewType::symbol:
		{	
			bP.fill( _color::transparent );
			
			// Receive Font
			_fontHandle		ft = _fontController::getStandardFont();
			_fontSize		ftSize = _fontController::getStandardFontSize();
			_color			ftColor = that->file->isHidden() ? _color::gray : _guiController::getItemFg( true );
			string			name = that->file->getFileName();
			
			_driveStats		stats;
			_filesystemController::getDriveStats( name , stats );
			
			wstring			fullName = _localizationController::getBuiltInString( stats.removable ? "lbl_removable_drive" : "lbl_harddrive" );
			fullName += " (" + name + ")";
			
			// Draw String Vertically middle and left aligned
			bP.drawString( max( 1 , int( myW - ft->getStringWidth( fullName ) ) >> 1 ) , myH - ft->getHeight() , ft , fullName , ftColor , ftSize );
			
			// Copy Icon
			_bitmap fileIcon = stats.removable ? (_bitmap&&) BMP_RemovableDriveIcon() : (_bitmap&&) BMP_HardDriveIcon();
			
			bP.copyTransparent(
				( myW - fileIcon.getWidth() ) >> 1 // X
				, ( myH - ft->getHeight() - fileIcon.getHeight() ) >> 1 // Y
				, move(fileIcon) // Bitmap
			);
			
			// Draw Outer Dotted Line Background
			if( that->hasFocus() )
				bP.drawDottedRect( 0 , 0 , myH , myW , _guiController::getItemBg( true ) );
			
			break;
		}
		case _fileViewType::list:
		default:
		{
			// Draw Background
			bP.fill( _guiController::getItemBg( that->hasFocus() , that->isSelected() ) );
			
			// Font
			_fontHandle		ft = _fontController::getStandardFont();
			_u8				fOH = _guiController::getFileObjectHeight();
			_fontSize		ftSize = _fontController::getStandardFontSize();
			_color			ftColor = that->file->isHidden() ? _color::gray : _guiController::getItemFg( that->hasFocus() , that->isSelected() );
			string			name = that->file->getFileName();
			
			_driveStats		stats;
			_filesystemController::getDriveStats( name , stats );
			
			wstring			fullName = _localizationController::getBuiltInString( stats.removable ? "lbl_removable_drive" : "lbl_harddrive" );
			fullName += " (" + name + ")";
			
			// Draw String Vertically middle and left aligned
			bP.drawString( 12 , ( ( myH - 1 ) >> 1 ) - ( ( ft->getAscent( ftSize ) + 1 ) >> 1 ) , ft , fullName , ftColor , ftSize );
			
			// Copy Icon
			_bitmap fileIcon = stats.removable ? (_bitmap&&) BMP_RemovableDriveIcon() : (_bitmap&&) BMP_HardDriveIcon();
			
			bP.copyTransparent(
				( fOH - fileIcon.getWidth() ) >> 1 // X
				, ( ( fOH + 1 ) >> 1 ) - ( ( fileIcon.getHeight() + 1 ) >> 1 ) // Y
				, move(fileIcon) // Bitmap
			);
			break;
		}
	};
	
	return use_default;
}

void _driveFileObject::initializeMenu()
{
	if( !_driveFileObject::defaultMenu )
	{
		_driveFileObject::defaultMenu = new _menu(
			{
				{ 1 , _localizationController::getBuiltInString("lbl_open") } ,
				{ 2 , _menu::divider } ,
				{ 3 , _localizationController::getBuiltInString("lbl_properties") }
			}
		);
		_driveFileObject::defaultMenu->addHandler( _menuHandlerRule() , make_callback( &_driveFileObject::defaultMenuHandler ) );
	}
}

_callbackReturn _driveFileObject::updateHandler( _event event )
{
	_driveFileObject* that = event.getGadget<_driveFileObject>();
	
	switch( that->viewType )
	{
		case _fileViewType::symbol:
			that->setSizeIfAuto( 27 , 27 );
			break;
		case _fileViewType::list:
		{
			string			name = that->file->getFileName();
			_driveStats		stats;
			_filesystemController::getDriveStats( name , stats );
			wstring			fullName = _localizationController::getBuiltInString( stats.removable ? "lbl_removable_drive" : "lbl_harddrive" );
			fullName += " (" + name + ")";
			
			that->setSizeIfAuto(
				_fontController::getStandardFont()->getStringWidth( fullName , _fontController::getStandardFontSize() ) + 12
				, _guiController::getFileObjectHeight()
			);
			break;
		}
		default:
			break;
	};
	
	return handled;
}

_driveFileObject::_driveFileObject( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string filename , _fileViewType viewType , _style&& style ) :
	_fileObject( x , y , width , height , move(filename) , viewType , move(style) )
{
	this->initializeMenu();
	
	// Register Update Handler..
	this->setInternalEventHandler( onUpdate , make_callback( &_driveFileObject::updateHandler ) );
	
	// Update
	this->updateNow();
	
	// Register Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_driveFileObject::refreshHandler ) );
	this->setInternalEventHandler( onBlur , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onMouseRightClick , _gadgetHelpers::openContextMenu( _driveFileObject::defaultMenu.get() ) );
	
	// Refresh...
	this->redraw();
}

void _driveFileObject::defaultMenuHandler( _u16 listIndex , _u16 entryIndex )
{
	_driveFileObject* fileObject = (_driveFileObject*) _gadgetHelpers::openContextMenu::getCurrentSubject();
	
	if( !fileObject || fileObject->getType() != _gadgetType::fileobject )
		return;
	
	switch( entryIndex )
	{
		case 1: // Open
			fileObject->execute();
			break;
		case 3: // Properties
			_winds::execute("%SYSTEM%/filedetail.exe -\"" + fileObject->file->getFileName() + "\"" );
			break;
	}
}

_uniquePtr<_menu> 				_driveFileObject::defaultMenu;