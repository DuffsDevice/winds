#include "_gadget/gadget.fileobject.h"
#include "_gadget/gadget.fileview.h"
#include "_gadget/gadget.imagegadget.h"
#include "_type/type.mime.h"
#include "_type/type.system.h"
#include "_type/type.shortcut.h"
#include "_type/type.gadget.helpers.h"

_callbackReturn _fileobject::clickHandler( _event event )
{
	_fileobject* that = event.getGadget<_fileobject>();
	
	// Execute!
	if( that->file->isDirectory() )
	{
		if( that->parent->getType() == _gadgetType::fileview )
		{
			// Trigger 'onEdit'-Event
			that->parent->triggerEvent( onEdit );
			
			((_fileview*)that->parent)->setPath( that->file->getFileName() );
		}
	}
	else
		that->file->execute();
	
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
			
			string ext = that->file->getExtension();
			
			// Certain Files do not have an .extension
			if( !_system::getUser()->sFE || !ext.length() )
				ext = "";
			else
				ext = "." + ext;
			
			string fullName = that->file->getName() + ext;
			
			// Receive Font
			const _font* ft = _system::getFont();
			
			// Font Size
			int ftSize = _system::_rtA_->getDefaultFontSize();
			
			// Draw String Vertically middle and left aligned
			bP.drawString( max( 1 , int( myW - ft->getStringWidth( fullName ) ) >> 1 ) , myH - ft->getHeight() , ft , fullName , COLOR_WHITE , ftSize );
			
			// Set Icon
			const _bitmap& fileIcon = that->file->getFileImage();
			
			bP.copyTransparent(
				( 25 - fileIcon.getWidth() ) >> 1 // X
				, ( myH - ft->getHeight() - fileIcon.getHeight() ) >> 1 // Y
				, fileIcon // Bitmap
			);
			
			// Draw Outer Dotted Line Background
			if( that->hasFocus() )
			{
				that->bitmap.drawVerticalDottedLine( 0 , 0 , myH , RGB255( 10 , 36 , 106 ) );
				that->bitmap.drawVerticalDottedLine( myW - 1 , 0 , myH , RGB255( 10 , 36 , 106 ) );
				that->bitmap.drawHorizontalDottedLine( 0 , 0 , myW , RGB255( 10 , 36 , 106 ) );
				that->bitmap.drawHorizontalDottedLine( 0 , myH - 1 , myW , RGB255( 10 , 36 , 106 ) );
			}
			
			break;
		}
		case _fileviewType::list:
		default:
		{
			// Darw Background
			bP.fill( that->hasFocus() ? RGB255( 10 , 36 , 106 ) : COLOR_TRANSPARENT );
			
			string ext = that->file->getExtension();
			
			// Certain Files do not have an .extension
			if( !_system::getUser()->sFE || !ext.length() )
				ext = "";
			else
				ext = "." + ext;
			
			string fullName = that->file->getName() + ext;
			
			// Receive Font
			const _font* ft = _system::getFont();
			
			// Font Size
			int ftSize = _system::_rtA_->getDefaultFontSize();
			
			// Font Color
			_pixel ftColor = that->hasFocus() ? COLOR_WHITE : COLOR_BLACK;
			
			// Draw String Vertically middle and left aligned
			bP.drawString( 11 , ( ( myH - 1 ) >> 1 ) - ( ( ft->getAscent( ftSize ) + 1 ) >> 1 ) , ft , fullName , ftColor , ftSize );
			
			// Set Icon
			const _bitmap& fileIcon = that->file->getFileImage();
			
			bP.copyTransparent(
				5 - ( fileIcon.getWidth() >> 1 ) // X
				, ( _system::getUser()->fOH >> 1 ) - ( fileIcon.getHeight() >> 1 ) // Y
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

_fileobject::_fileobject( _coord x , _coord y , string&& fl , _fileviewType viewtype , bool singleClickToExecute , _style&& style ) :
	_gadget( _gadgetType::fileobject , 50 , _system::getUser()->fOH , x , y , (_style&&)style )
	, file( new _direntry(fl) )
	, viewType( viewtype )
	, pressed( false )
{
	if( file->getMimeType() == _mime::application_x_ms_shortcut )
	{
		delete this->file;
		this->file = new _shortcut( fl );
	}
	
	switch( this->viewType )
	{
		case _fileviewType::symbol_big:
		{
			this->setDimensions( _rect( x , y , 26 , 26 ) );
			
			auto cb = _gadgetHelpers::moveBesidePrecedent( _dimension::vertical , 2 , 2 , true , 0 );
			this->setInternalEventHandler( onParentSet , cb );
			this->setInternalEventHandler( onParentResize , cb );
			this->setInternalEventHandler( onPreSet , cb );
			break;
		}
		case _fileviewType::list:
		default:
		{
			string ext = this->file->getExtension();
			
			// Certain Files do not have an .extension
			if( !_system::getUser()->sFE || !ext.length() )
				ext = "";
			else
				ext = "." + ext;
			
			string fullName = this->file->getName() + ext;
			
			// Receive Font
			const _font* ft = _system::getFont();
			
			this->setWidth( ft->getStringWidth( fullName ) + 11 );
			
			auto cb = _gadgetHelpers::moveBesidePrecedent( _dimension::vertical , 2 , 2 , false , 0);
			this->setInternalEventHandler( onParentSet , cb );
			this->setInternalEventHandler( onPreSet , cb );
			
			break;
		}
	};
	
	// Register Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_fileobject::refreshHandler ) );
	this->setInternalEventHandler( onFocus , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( onBlur , _gadgetHelpers::eventForwardRefresh() );
	this->setInternalEventHandler( singleClickToExecute ? onMouseClick : onMouseDblClick , make_callback( &_fileobject::clickHandler ) );
	
	//this->setInternalEventHandler( onDragging , make_callback( &_fileobject::dragHandler ) );
	//this->setInternalEventHandler( onDragStart , make_callback( &_fileobject::dragHandler ) );
	//this->setInternalEventHandler( onDragStop , make_callback( &_fileobject::dragHandler ) );
	
	// Refresh...
	this->redraw();
}

_fileobject::~_fileobject()
{
	this->removeChildren( true );
	if( this->file )
		delete this->file;
}