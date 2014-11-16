#include <_gadget/gadget.fileobject.h>
#include <_gadget/gadget.fileview.h>
#include <_gadget/gadget.image.h>
#include <_type/type.mime.h>
#include <_type/type.windows.h>
#include <_type/type.direntry.shortcut.h>
#include <_type/type.gadget.helpers.h>
#include <_controller/controller.font.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.localization.h>

_callbackReturn _fileObject::keyHandler( _event event )
{
	_fileObject* that = event.getGadget<_fileObject>();
	
	// Go to the next visible node
	if( event.getKeyCode() == _key::down ){
		_gadget* subVisible = that->getSubcedentChild( true );
		if( subVisible )
			subVisible->focus();
	}
	// Go to the pervious visible node
	else if( event.getKeyCode() == _key::up ){
		_gadget* preVisible = that->getPrecedentChild( true );
		if( preVisible )
			preVisible->focus();
	}
	// Finish the renaming
	else if( event.getKeyCode() == _key::a ){
		if( that->isRenamed() )
			that->finishRename();
	}
	
	return use_default;
}

_callbackReturn _fileObject::blurHandler( _event event ){
	_fileObject* that = event.getGadget<_fileObject>();
	that->finishRename();
	that->redraw();
	return handled;
}

_callbackReturn _fileObject::clickHandler( _event event )
{
	_fileObject* that = event.getGadget<_fileObject>();
	
	if( event.getPosX() >= 10 )
		that->initRename();
	return handled;
}

void _fileObject::initRename()
{
	if( this->isRenamed() )
		return;
	
	_fileObject::renameTextBox = new _textBox( 11 , 0 , this->getWidth() - 11 , this->getHeight() , this->file->getFullName() );
	this->addChild( this->renameTextBox.get() );
	_fileObject::renameTextBox->focus();
	_fileObject::renamedFile = this;
}

void _fileObject::finishRename()
{
	if( !this->isRenamed() )
		return;
	
	this->file->rename( _fileObject::renameTextBox->getStrValue() );
	_fileObject::renameTextBox = nullptr;
	_fileObject::renamedFile = nullptr;
	this->update();
}

_callbackReturn _fileObject::updateHandler( _event event )
{
	_fileObject* that = event.getGadget<_fileObject>();
	
	switch( that->viewType )
	{
		case _fileViewType::symbol:
			that->setSizeIfAuto( 27 , 27 );
			break;
		case _fileViewType::list:
			that->setSizeIfAuto(
				_fontController::getStandardFont()->getStringWidth( that->file->getFullName() , _fontController::getStandardFontSize() ) + 12
				, _guiController::getFileObjectHeight()
			);
			break;
		default:
			break;
	};
	
	return handled;
}

void _fileObject::execute( _programArgs args , bool openInNewWindow )
{
	// Execute!
	if( this->file->isDirectory() )
	{
		_fileView* fileView = (_fileView*) this->getParent();
		
		if( openInNewWindow )
			_windows::executeCommand("%SYSTEM%/explorer.exe -\"" + this->file->getFileName() + "\"" );
		else if( fileView->getType() == _gadgetType::fileview )
		{
			// Trigger 'onEdit'-Event
			fileView->triggerEvent( onEdit );
			
			// Set new path
			fileView->setPath( this->file->getFileName() );
		}
	}
	else	
		this->file->execute( move(args) );
}

_callbackReturn _fileObject::refreshHandler( _event event )
{
	// Receive Gadget
	_fileObject* that = event.getGadget<_fileObject>();
	
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
			_u8				ftSize = _fontController::getStandardFontSize();
			_color			ftColor = that->file->isHidden() ? _color::gray : _guiController::getItemFg( true );
			string			fullName = that->file->getFullName();
			
			// Draw String Vertically middle and left aligned
			bP.drawString( max( 1 , int( myW - ft->getStringWidth( fullName ) ) >> 1 ) , myH - ft->getHeight() , ft , fullName , ftColor , ftSize );
			
			// Copy Icon
			_bitmap fileIcon = that->file->getFileImage();
			
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
			_u8				ftSize = _fontController::getStandardFontSize();
			_color			ftColor = that->file->isHidden() ? _color::gray : _guiController::getItemFg( that->hasFocus() , that->isSelected() );
			string			fullName = that->file->getFullName();
			
			// Draw String Vertically middle and left aligned
			if( !that->isRenamed() )
				bP.drawString( 12 , ( ( myH - 1 ) >> 1 ) - ( ( ft->getAscent( ftSize ) + 1 ) >> 1 ) , ft , fullName , ftColor , ftSize );
			
			// Copy Icon
			_bitmap fileIcon = that->file->getFileImage();
			
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

void _fileObject::initializeMenu()
{
	if( !_fileObject::defaultMenu )
	{
		_fileObject::defaultMenu = new _menu(
			{
				{ 1 , _localizationController::getBuiltInString("lbl_open") } ,
				{ 2 , _menu::divider } ,
				{ 3 , _localizationController::getBuiltInString("lbl_cut") } ,
				{ 4 , _localizationController::getBuiltInString("lbl_copy") } ,
				{ 5 , _menu::divider } ,
				{ 6 , _localizationController::getBuiltInString("lbl_delete") } ,
				{ 7 , _menu::divider } ,
				{ 8 , _localizationController::getBuiltInString("lbl_rename") } ,
				{ 9 , _localizationController::getBuiltInString("lbl_move_to") } ,
				{ 10, _menu::divider } ,
				{ 11, _localizationController::getBuiltInString("lbl_properties") }
			}
		);
		_fileObject::defaultMenu->addHandler( _menuHandlerRule() , make_callback( &_fileObject::defaultMenuHandler ) );
	}
}

_fileObject::_fileObject( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , const string& filename , _fileViewType viewtype , _style&& style ) :
	_gadget( _gadgetType::fileobject , x , y , width , height , ( viewtype == _fileViewType::symbol ) ? (_style&&)style : style | _style::noTransparentParts )
	, file( new _direntry(filename) )
	, viewType( viewtype )
{
	this->initializeMenu();
	
	// Replace _direntry with the more specialized class '_shortcut'
	if( file->getMimeType() == _mime::application_x_ms_shortcut ){
		delete this->file;
		this->file = new _shortcut( filename );
	}
	
	// Register Update Handler..
	this->setInternalEventHandler( onUpdate , make_callback( &_fileObject::updateHandler ) );
	
	// and update Size
	this->updateNow();
	
	// Register Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_fileObject::refreshHandler ) );
	this->setInternalEventHandler( onFocus , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onSelect , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onDeselect , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onBlur , make_callback( &_fileObject::blurHandler ) );
	this->setInternalEventHandler( onKeyDown , make_callback( &_fileObject::keyHandler ) );
	this->setInternalEventHandler( onKeyRepeat , make_callback( &_fileObject::keyHandler ) );
	this->setInternalEventHandler( onMouseRightClick , _gadgetHelpers::openContextMenu( _fileObject::defaultMenu.get() ) );
	
	// Refresh...
	this->redraw();
}

_fileObject::~_fileObject()
{
	if( this->file )
		delete this->file;
}

void _fileObject::deleteDialogHandler( _dialogResult result )
{
	if( result != _dialogResult::yes )
		return;
	
	_fileObject* deletedFile = _fileObject::deletedFile;
	
	// Delete the file
	deletedFile->file->unlink( true );
	
	// Update the view
	_fileView* parent = (_fileView*)deletedFile->getParent();
	if( parent && parent->getType() == _gadgetType::fileview )
		parent->setPath( parent->getPath() );
	
	_fileObject::deleteDialog = nullptr;
	_fileObject::deletedFile = nullptr;
}

void _fileObject::defaultMenuHandler( _u16 listIndex , _u16 entryIndex )
{
	_fileObject* fileObject = (_fileObject*)_gadgetHelpers::openContextMenu::getCurrentSubject();
	
	if( !fileObject || fileObject->getType() != _gadgetType::fileobject )
		return;
	
	if( listIndex == 0 )
	{
		switch( entryIndex )
		{
			case 1: // Open
				fileObject->execute();
				break;
			case 6: // Delete
			{
				_fileObject::deletedFile = fileObject;
				_fileObject::deleteDialog = new _fileDeleteDialog( *fileObject->file );
				_fileObject::deleteDialog->setCallback( make_callback( &_fileObject::deleteDialogHandler ) );
				_fileObject::deleteDialog->execute();
				break;
			}
			case 8: // Rename
				fileObject->initRename();
				break;
			case 11: // Properties
				_windows::executeCommand("%SYSTEM%/filedetail.exe -\"" + fileObject->file->getFileName() + "\"" );
				break;
		}
	}
}

_uniquePtr<_menu> 				_fileObject::defaultMenu;
_uniquePtr<_textBox>			_fileObject::renameTextBox;
_fileObject*					_fileObject::renamedFile;
_uniquePtr<_fileDeleteDialog>	_fileObject::deleteDialog;
_fileObject*					_fileObject::deletedFile;