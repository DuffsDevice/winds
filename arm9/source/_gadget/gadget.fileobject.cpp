#include "_gadget/gadget.fileobject.h"
#include "_gadget/gadget.fileview.h"
#include "_gadget/gadget.imagegadget.h"
#include "_type/type.mime.h"
#include "_type/type.system.h"
#include "_type/type.direntry.shortcut.h"
#include "_type/type.gadget.helpers.h"

_callbackReturn _fileobject::updateHandler( _event event )
{
	_fileobject* that = event.getGadget<_fileobject>();
	
	switch( that->viewType )
	{
		case _fileviewType::symbol_big:
			that->setSizeIfAuto( 26 , 26 );
			break;
		case _fileviewType::list:
			that->setSizeIfAuto(
				_system::getFont()->getStringWidth( that->file->getDisplayName() , _system::getRTA().getDefaultFontSize() ) + 11
				, _system::getUser().fOH
			);
			break;
		default:
			break;
	};
	
	return handled;
}

void _fileobject::execute( _cmdArgs args , bool openInNewWindow )
{
	// Execute!
	if( this->file->isDirectory() )
	{
		if( openInNewWindow )
			_system::executeCommand("%SYSTEM%/explorer.exe -" + this->file->getFileName() );
		else if( this->parent->getType() == _gadgetType::fileview )
		{
			((_fileview*)this->parent)->setPath( this->file->getFileName() );
			
			// Trigger 'onEdit'-Event
			this->parent->triggerEvent( onEdit );
		}
	}
	else
		this->file->execute( move(args) );
}

_callbackReturn _fileobject::clickHandler( _event event )
{
	event.getGadget<_fileobject>()->execute();
	
	return handled;
}

_callbackReturn _fileobject::refreshHandler( _event event )
{
	// Receive Gadget
	_fileobject* that = event.getGadget<_fileobject>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myH = bP.getHeight();
	_length myW = bP.getWidth();
	
	switch( that->viewType )
	{
		case _fileviewType::symbol_big:
		{	
			bP.fill( COLOR_TRANSPARENT );
			
			// Receive Font
			const _font*	ft = _system::getFont();
			_u8				ftSize = _system::getRTA().getDefaultFontSize();
			string			fullName = that->file->getDisplayName();
			
			// Draw String Vertically middle and left aligned
			bP.drawString( max( 1 , int( myW - ft->getStringWidth( fullName ) ) >> 1 ) , myH - ft->getHeight() , ft , fullName , _system::getRTA().getItemForeground( true ) , ftSize );
			
			// Copy Icon
			_constbitmap& fileIcon = that->file->getFileImage();
			
			bP.copyTransparent(
				( 25 - fileIcon.getWidth() ) >> 1 // X
				, ( myH - ft->getHeight() - fileIcon.getHeight() ) >> 1 // Y
				, fileIcon // Bitmap
			);
			
			// Draw Outer Dotted Line Background
			if( that->hasFocus() )
				that->bitmap.drawDottedRect( 0 , 0 , myH , myW , _system::getRTA().getItemBackground( true ) );
			
			break;
		}
		case _fileviewType::list:
		default:
		{
			// Draw Background
			bP.fill( that->hasFocus() ? RGB255( 10 , 36 , 106 ) : COLOR_TRANSPARENT );
			
			// Font
			const _font*	ft = _system::getFont();
			_u8				ftSize = _system::getRTA().getDefaultFontSize();
			_pixel			ftColor = that->hasFocus() ? COLOR_WHITE : COLOR_BLACK;
			string			fullName = that->file->getDisplayName();
			
			// Draw String Vertically middle and left aligned
			bP.drawString( 11 , ( ( myH - 1 ) >> 1 ) - ( ( ft->getAscent( ftSize ) + 1 ) >> 1 ) , ft , fullName , ftColor , ftSize );
			
			// Copy Icon
			_constbitmap& fileIcon = that->file->getFileImage();
			
			bP.copyTransparent(
				5 - ( fileIcon.getWidth() >> 1 ) // X
				, ( that->getHeight() >> 1 ) - ( fileIcon.getHeight() >> 1 ) // Y
				, fileIcon // Bitmap
			);
			
			break;
		}
	};
	
	return handled;
}

//_callbackReturn _fileobject::dragHandler( _event event )
//{	
//	// Receive Gadget
//	//_fileobject* that = event.getGadget<_fileobject>();
//	
//	return not_handled;
//}

_fileobject::_fileobject( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , const string& fl , _fileviewType viewtype , bool singleClickToExecute , _style&& style ) :
	_gadget( _gadgetType::fileobject , x , y , width , height , (_style&&)style )
	, file( new _direntry(fl) )
	, viewType( viewtype )
	, pressed( false )
{
	if( file->getMimeType() == _mime::application_x_ms_shortcut )
	{
		delete this->file;
		this->file = new _shortcut( fl );
	}
	
	// Register Update Handler..
	this->setInternalEventHandler( onUpdate , make_callback( &_fileobject::updateHandler ) );
	
	// and update Size
	this->updateNow();
	
	// Register Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_fileobject::refreshHandler ) );
	this->setInternalEventHandler( onFocus , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onBlur , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( singleClickToExecute ? onMouseClick : onMouseDblClick , make_callback( &_fileobject::clickHandler ) );
	
	// Refresh...
	this->redraw();
}

_fileobject::~_fileobject()
{
	if( this->file )
		delete this->file;
}