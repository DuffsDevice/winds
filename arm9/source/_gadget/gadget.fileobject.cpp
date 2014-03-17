#include "_gadget/gadget.fileobject.h"
#include "_gadget/gadget.fileview.h"
#include "_gadget/gadget.image.h"
#include "_type/type.mime.h"
#include "_type/type.system.h"
#include "_type/type.direntry.shortcut.h"
#include "_type/type.gadget.helpers.h"

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
				_system::getFont()->getStringWidth( that->file->getDisplayName() , _system::getRTA().getDefaultFontSize() ) + 13
				, _system::getUser().fOH
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
			_system::executeCommand("%SYSTEM%/explorer.exe -\"" + this->file->getFileName() + "\"" );
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
			const _font*	ft = _system::getFont();
			_u8				ftSize = _system::getRTA().getDefaultFontSize();
			_color			ftColor = that->file->isHidden() ? _color::gray : _system::getRTA().getItemForeground( true );
			string			fullName = that->file->getDisplayName();
			
			// Draw String Vertically middle and left aligned
			bP.drawString( max( 1 , int( myW - ft->getStringWidth( fullName ) ) >> 1 ) , myH - ft->getHeight() , ft , fullName , ftColor , ftSize );
			
			// Copy Icon
			_constBitmap& fileIcon = that->file->getFileImage();
			
			bP.copyTransparent(
				( myW - fileIcon.getWidth() ) >> 1 // X
				, ( myH - ft->getHeight() - fileIcon.getHeight() ) >> 1 // Y
				, fileIcon // Bitmap
			);
			
			// Draw Outer Dotted Line Background
			if( that->hasFocus() )
				bP.drawDottedRect( 0 , 0 , myH , myW , _system::getRTA().getItemBackground( true ) );
			
			break;
		}
		case _fileViewType::list:
		default:
		{
			// Draw Background
			bP.fill( _system::getRTA().getItemBackground( that->hasFocus() ) );
			
			// Font
			const _font*	ft = _system::getFont();
			_u8				ftSize = _system::getRTA().getDefaultFontSize();
			_color			ftColor = that->file->isHidden() ? _color::gray : _system::getRTA().getItemForeground( that->hasFocus() );
			string			fullName = that->file->getDisplayName();
			
			// Draw String Vertically middle and left aligned
			bP.drawString( 13 , ( ( myH - 1 ) >> 1 ) - ( ( ft->getAscent( ftSize ) + 1 ) >> 1 ) , ft , fullName , ftColor , ftSize );
			
			// Copy Icon
			_constBitmap& fileIcon = that->file->getFileImage();
			
			bP.copyTransparent(
				5 - ( fileIcon.getWidth() >> 1 ) // X
				, ( ( that->getHeight() + 1 ) >> 1 ) - ( ( fileIcon.getHeight() + 1 ) >> 1 ) // Y
				, fileIcon // Bitmap
			);
			
			break;
		}
	};
	
	return handled;
}

//_callbackReturn _fileObject::dragHandler( _event event )
//{	
//	// Receive Gadget
//	//_fileObject* that = event.getGadget<_fileObject>();
//	
//	return not_handled;
//}

_fileObject::_fileObject( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , const string& filename , _fileViewType viewtype , _style&& style ) :
	_gadget( _gadgetType::fileobject , x , y , width , height , move(style) )
	, file( new _direntry(filename) )
	, viewType( viewtype )
	, pressed( false )
{
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
	this->setInternalEventHandler( onBlur , _gadgetHelpers::eventForwardRefresh() );
	
	// Refresh...
	this->redraw();
}

_fileObject::~_fileObject()
{
	if( this->file )
		delete this->file;
}