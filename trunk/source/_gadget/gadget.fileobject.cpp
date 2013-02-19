#include "_gadget/gadget.fileobject.h"
#include "_gadget/gadget.fileview.h"
#include "_gadget/gadget.imagegadget.h"
#include "_type/type.mime.h"
#include "_type/type.system.h"

_callbackReturn _fileobject::doubleClickHandler( _event event )
{
	_fileobject* that = event.getGadget<_fileobject>();
	
	// Execute!
	if( that->file.isDirectory() )
	{
		if( that->parent->getType() == _gadgetType::fileview )
		{
			// Trigger 'onChange'-Event
			that->parent->triggerEvent( onChange );
			
			((_fileview*)that->parent)->setPath( that->file.getFileName() );
		}
	}
	else
		that->file.execute();
	
	return handled;
}

_callbackReturn _fileobject::refreshHandler( _event event )
{
	// Receive Gadget
	_fileobject* that = event.getGadget<_fileobject>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	// Darw Background
	bP.fill( that->hasFocus() ? RGB255( 10 , 36 , 106 ) : COLOR_WHITE );
	
	_length myH = bP.getHeight();
	//_length myW = bP.getWidth();
	
	switch( that->viewType )
	{
		case _fileviewType::list:
		{
			string ext = that->file.getExtension();
			
			// Certain Files do not have an .extension
			if( !_system::_runtimeAttributes_->user->sFE || !ext.length() )
				ext = "";
			else
				ext = "." + ext;
			
			string fullName = that->file.getName() + ext;
			
			// Receive Font
			_font* ft = _system::getFont();
			
			// Font Size
			int ftSize = _system::_runtimeAttributes_->defaultFontSize;
			
			// Font Color
			_pixel ftColor = that->hasFocus() ? COLOR_WHITE : COLOR_BLACK;
			
			// Draw String Vertically middle and left aligned
			bP.drawString( 11 , ( ( myH - 1 ) >> 1 ) - ( ( ft->getAscent( ftSize ) + 1 ) >> 1 ) , ft , fullName , ftColor , ftSize );
			
			// Set Icon
			const _bitmap& fileIcon = that->file.getFileImage();
			
			bP.copyTransparent(
				5 - ( fileIcon.getWidth() >> 1 ) // X
				, ( _system::_runtimeAttributes_->user->fOH >> 1 ) - ( fileIcon.getHeight() >> 1 ) // Y
				, fileIcon // Bitmap
			);
			
			break;
		}
		default:
			break;
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

_fileobject::_fileobject( _coord x , _coord y , string fl , _fileviewType viewtype , _style style ) :
	_gadget( _gadgetType::fileobject , 50 , _system::_runtimeAttributes_->user->fOH , x , y , style ) , file( fl ) , viewType( viewtype ) , pressed( false )
{
	switch( this->viewType )
	{
		case _fileviewType::list:{
			string ext = this->file.getExtension();
			
			// Certain Files do not have an .extension
			if( !_system::_runtimeAttributes_->user->sFE || !ext.length() )
				ext = "";
			else
				ext = "." + ext;
			
			string fullName = this->file.getName() + ext;
			
			// Receive Font
			_font* ft = _system::getFont();
			
			this->setWidth( ft->getStringWidth( fullName ) + 11 );
		}
		default:
			break;
	};
	
	// Register Handlers
	this->registerEventHandler( refresh , new _staticCallback( &_fileobject::refreshHandler ) );
	//this->registerEventHandler( dragging , new _staticCallback( &_fileobject::dragHandler ) );
	//this->registerEventHandler( dragStart , new _staticCallback( &_fileobject::dragHandler ) );
	//this->registerEventHandler( dragStop , new _staticCallback( &_fileobject::dragHandler ) );
	this->registerEventHandler( onFocus , new _gadget::eventForwardRefresh() );
	this->registerEventHandler( onBlur , new _gadget::eventForwardRefresh() );
	this->registerEventHandler( mouseDoubleClick , new _staticCallback( &_fileobject::doubleClickHandler ) );
	
	// Refresh...
	this->refreshBitmap();
}

_fileobject::~_fileobject()
{
	this->removeChildren( true );
}