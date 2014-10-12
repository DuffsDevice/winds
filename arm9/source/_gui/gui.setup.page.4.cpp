#include <_gui/gui.setup.page.4.h>
#include <_controller/controller.localization.h>
#include <_controller/controller.font.h>
#include <_type/type.windows.h>
#include <_type/type.user.h>

const _s8		_guiSetupPage4::iconNumbers[] = { 5 , 16 , 17 , 22 , 3 , 11 , 4 , 2 , -1 };

bool _guiSetupPage4::destroy( _gadget* viewParent )
{
	if( this->txtProfileName->getStrValue().empty() )
	{
		this->txtProfileName->blink();
		return false;
	}
	delete this->lblTitle;
	delete this->lblName;
	delete this->lblIcon;
	delete this->txtProfileName;
	for( _pair<_imageGadget*,_bitmap> data : imgProfileIcons )
		delete data.first;
	imgProfileIcons.clear();
	
	// Switching the view is allowed
	return true;
}

_guiSetupPage4::_guiSetupPage4() :
	profileName( _windows::getDSUserName() )
	, profileIcon( -1 )
{}

void _guiSetupPage4::create( _gadget* viewParent )
{
	// Title
	this->lblTitle = new _label( 13 , 28 , ignore , ignore , _localizationController::getBuiltInString("lbl_profile") );
	this->lblTitle->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	this->lblTitle->setFont( _fontController::getFont( "ArialBlack13" ) );
	viewParent->addChild( this->lblTitle );
	
	// Labels for both the profile name textbox and the profile icon
	this->lblName = new _label( 20 , 60 , ignore , ignore , _localizationController::getBuiltInString("txt_name") );
	this->lblIcon = new _label( 20 , 90 , ignore , ignore , _localizationController::getBuiltInString("txt_profile_icon") );
	this->lblName->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	this->lblIcon->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	viewParent->addChild( this->lblName );
	viewParent->addChild( this->lblIcon );
	
	// Textbox for profile name
	this->txtProfileName = new _textBox( 21 , 70 , 80 , ignore , profileName  );
	this->txtProfileName->setInternalEventHandler( onEdit , make_callback( this , &_guiSetupPage4::profileNameTextboxHandler ) );
	viewParent->addChild( this->txtProfileName );
	
	// User Icons
	for( int i = 0 ; iconNumbers[i] >= 0 ; i++ )
	{
		_bitmap icon = _user::getStandardLogo( iconNumbers[i] );
		_imageGadget* image = new _imageGadget(
			22 + i * 20
			, 102
			, move(icon)
			, ignore 
			, ignore
			, _style::storeInt( iconNumbers[i] )
		);
		
		// Register event handler
		image->setInternalEventHandler( onDraw , make_callback( this , &_guiSetupPage4::profileIconDrawHandler ) );
		image->setInternalEventHandler( onMouseDown , make_callback( this , &_guiSetupPage4::profileIconClickHandler ) );
		
		// Add to list
		imgProfileIcons[image] = (_bitmap&&)icon;
		
		image->redraw();
		
		// Add to parent
		viewParent->addChild( image );
	}
}

_callbackReturn _guiSetupPage4::profileIconClickHandler( _event event )
{
	// Receive gadget
	_imageGadget* that = event.getGadget<_imageGadget>();
	
	_u8 iconNumber = that->getStyle().val;
	_u8 oldNumber = this->profileIcon;
	
	if( oldNumber == iconNumber )
		return handled;
	
	this->profileIcon = iconNumber;
	
	// Refresh the old icon
	for( _pair<_imageGadget*,_bitmap> g : this->imgProfileIcons )
	{
		if( g.first->getStyle().val == oldNumber )
			g.first->redraw();
	}
	
	// Refresh the new active icon
	that->redraw();
	
	return handled;
}

_callbackReturn _guiSetupPage4::profileIconDrawHandler( _event event )
{
	_imageGadget* that = event.getGadget<_imageGadget>();
	
	// Get index of imagegadget
	_constBitmap& iconBitmap = imgProfileIcons[that] ;
	
	// Get bitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Check if the user icon is active and
	// decide whether to draw it normal or semi-transparent
	if( that->getStyle().val != this->profileIcon )
	{
		for (_int i = 0; i != iconBitmap.getWidth(); i++ )
		{
			for (_int j = 0; j != iconBitmap.getHeight() ; j++ )
			{
				_color color	= iconBitmap.getPixel( i , j );
				_color bgColor	= _color::fromHex( 0x5A7EDC );
				
				// Plot the pixel
				bP.drawPixel( i , j , _color::mix( color , bgColor , 16 ) );
			}
		}
	}
	else // Copy it normal
		bP.copyTransparent( 0 , 0 , iconBitmap );
	
	return handled;
}