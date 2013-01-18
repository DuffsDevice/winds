#include "_type/type.dialog.h"
#include "_type/type.system.h"
#include "_gadget/gadget.button.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.window.h"
#include "_gadget/gadget.textbox.h"
#include "_gadget/gadget.imagegadget.h"

void _dialog::internalExecution()
{
	switch( this->dType )
	{
		case _dialogType::yesNo:
			_system::_gadgetHost_->addChild( (_window*)this->data[3] );
			this->finished = false;
			break;
		case _dialogType::enterText:
			_system::_gadgetHost_->addChild( (_window*)this->data[4] );
			this->finished = false;
			break;
		case _dialogType::image:
			_system::_gadgetHost_->addChild( (_window*)this->data[4] );
			this->finished = false;
			break;
		default:
			break;
	}
}

void _dialog::internalCleanup()
{
	switch( this->dType )
	{
		case _dialogType::yesNo:
			((_window*)this->data[3])->setParent( nullptr );
			break;
		case _dialogType::enterText:
			((_window*)this->data[4])->setParent( nullptr );
			break;
			case _dialogType::image:
			((_window*)this->data[4])->setParent( nullptr );
			break;
		default:
			break;
	}
}

_callbackReturn _dialog::internalEventHandler( _event e )
{
	_gadget* that = e.getGadget();

	switch( this->dType )
	{
		case _dialogType::yesNo:
			// No-Button
			if( (_button*)that == (_button*)this->data[0] )
			{
				this->finished = true;
				this->intResult = 0;
				if( this->exitFunc )
					(*this->exitFunc)( 0 );
				_system::executeTimer( new _classCallback( this , &_dialog::internalCleanup ) , 100 );
			}
			
			// Yes-Button
			else if( (_button*)that == (_button*)this->data[1] )
			{
				this->finished = true;
				this->intResult = 1;
				if( this->exitFunc )
					(*this->exitFunc)( 1 );
				_system::executeTimer( new _classCallback( this , &_dialog::internalCleanup ) , 100 );
			}
			
			// Window-Close-Button
			else if( (_window*)that == (_window*)this->data[3] )
			{
				this->finished = true;
				this->intResult = -1;
				if( this->exitFunc )
					(*this->exitFunc)( -1 );
				_system::executeTimer( new _classCallback( this , &_dialog::internalCleanup ) , 100 );
			}
			break;
		case _dialogType::enterText:	
			// OK-Button
			if( (_button*)that == (_button*)this->data[0] )
			{
				this->finished = true;
				this->intResult = 1;
				this->strResult = ((_textbox*)this->data[3])->getStrValue();
				if( this->exitFunc )
					(*this->exitFunc)( 1 );
				_system::executeTimer( new _classCallback( this , &_dialog::internalCleanup ) , 100 );
			}
			
			// Window-Close-Button or CANCEL
			else if( (_window*)that == (_window*)this->data[4] || (_button*)that == (_button*)this->data[1] )
			{
				this->finished = true;
				this->intResult = -1;
				if( this->exitFunc )
					(*this->exitFunc)( -1 );
				_system::executeTimer( new _classCallback( this , &_dialog::internalCleanup ) , 100 );
			}
			break;
		case _dialogType::image:
			// OK-Button
			if( (_button*)that == (_button*)this->data[0] )
			{
				this->finished = true;
				this->intResult = 1;
				if( this->exitFunc )
					(*this->exitFunc)( 1 );
				_system::executeTimer( new _classCallback( this , &_dialog::internalCleanup ) , 100 );
			}
			
			// Other-Button
			if( (_button*)that == (_button*)this->data[1] )
			{
				this->finished = true;
				this->intResult = 0;
				if( this->exitFunc )
					(*this->exitFunc)( 0 );
				_system::executeTimer( new _classCallback( this , &_dialog::internalCleanup ) , 100 );
			}
			
			// Window-Close-Button or CANCEL
			else if( (_window*)that == (_window*)this->data[4] )
			{
				this->finished = true;
				this->intResult = -1;
				if( this->exitFunc )
					(*this->exitFunc)( -1 );
				_system::executeTimer( new _classCallback( this , &_dialog::internalCleanup ) , 100 );
			}
			break;
		default:
			break;
	}
	
	return handled;
}

_dialog::~_dialog()
{
	switch( this->dType )
	{
		case _dialogType::yesNo:
			delete (_button*)this->data[0];
			delete (_button*)this->data[1];
			delete (_label*)this->data[2];
			delete (_window*)this->data[3];
			delete[] this->data;
			break;
		case _dialogType::enterText:
			delete (_button*)this->data[0];
			delete (_button*)this->data[1];
			delete (_label*)this->data[2];
			delete (_textbox*)this->data[3];
			delete (_window*)this->data[4];
			delete[] this->data;
			break;
		case _dialogType::image:
			delete (_button*)this->data[0];
			if( this->data[1] )
				delete (_button*)this->data[1];
			delete (_label*)this->data[2];
			delete (_textbox*)this->data[3];
			delete (_window*)this->data[4];
			delete[] this->data;
			break;
		default:
			break;
	}
}

//! yesNo
void _dialog::yesNoDialog( _dialog& d , string msg , string windowLbl , string yesLbl , string noLbl )
{
	// Dont allow to overwrite
	if( d.dType != _dialogType::none )
		return;
	
	d.dType = _dialogType::yesNo;
	d.data = new void* [4];
	
	// Buttons
	_button* btn1 = new _button( 0 , 0 , noLbl );
	_button* btn2 = new _button( 0 , 0 , yesLbl );
	btn2->setAutoSelect( true );
	
	// Labels
	_label* lbl1 = new _label( 2 , 2 , msg );
	
	// Window
	_length winWidth = max( lbl1->getWidth() + 4 , btn1->getWidth() + btn2->getWidth() + 5 );
	_length winHeight = max( lbl1->getHeight() + btn1->getHeight() + 8 , 30 ) + 11; // + 11 for the window
	
	_window* win1 = new _window( winWidth , winHeight , ( SCREEN_WIDTH - winWidth ) >> 1 , ( SCREEN_HEIGHT - winHeight ) >> 1 , windowLbl , _styleAttr() | _styleAttr::canNotLooseFocus | _styleAttr::notResizeable );
	
	btn2->moveTo( winWidth - btn2->getWidth() - 3 , winHeight - btn2->getHeight() - 12 );
	btn1->moveTo( winWidth - btn1->getWidth() - btn2->getWidth() - 4 , winHeight - btn1->getHeight() - 12 );
	
	win1->addChild( btn1 );
	win1->addChild( btn2 );
	win1->addChild( lbl1 );
	
	btn1->registerEventHandler( onAction , new _classCallback( &d , &_dialog::internalEventHandler ) );
	btn2->registerEventHandler( onAction , new _classCallback( &d , &_dialog::internalEventHandler ) );
	win1->registerEventHandler( onClose , new _classCallback( &d , &_dialog::internalEventHandler ) );
	
	d.data[0] = btn1;
	d.data[1] = btn2;
	d.data[2] = lbl1;
	d.data[3] = win1;
}

void _dialog::enterTextDialog( _dialog& d , string msg , string windowLbl , string okLbl , string cancelLbl )
{	
	// Dont allow to overwrite
	if( d.dType != _dialogType::none )
		return;
	
	d.dType = _dialogType::enterText;
	d.data = new void* [5];
	
	// Buttons
	_button* btn1 = new _button( 0 , 0 , okLbl );
	_button* btn2 = new _button( 0 , 0 , cancelLbl );
	btn2->setAutoSelect( true );
	
	// Labels
	_label* lbl1 = new _label( 2 , 2 , msg );
	
	// Window-Width
	_length winWidth = max( lbl1->getWidth() + 4 , btn1->getWidth() + btn2->getWidth() + 5 );
	
	_textbox* txt1 = new _textbox( 2 , lbl1->getHeight() + 4 , winWidth - 6 );
	
	_length winHeight = max( lbl1->getHeight() + btn1->getHeight() + 10 + txt1->getHeight() , 30 ) + 11; // + 11 for the window
	
	_window* win1 = new _window( winWidth , winHeight , ( SCREEN_WIDTH - winWidth ) >> 1 , ( SCREEN_HEIGHT - winHeight ) >> 1  , windowLbl , _styleAttr() | _styleAttr::canNotLooseFocus | _styleAttr::notResizeable );
	
	btn2->moveTo( winWidth - btn2->getWidth() - 3 , winHeight - btn2->getHeight() - 12 );
	btn1->moveTo( winWidth - btn1->getWidth() - btn2->getWidth() - 4 , winHeight - btn1->getHeight() - 12 );
	
	win1->addChild( btn1 );
	win1->addChild( btn2 );
	win1->addChild( lbl1 );
	win1->addChild( txt1 );
	
	btn1->registerEventHandler( onAction , new _classCallback( &d , &_dialog::internalEventHandler ) );
	btn2->registerEventHandler( onAction , new _classCallback( &d , &_dialog::internalEventHandler ) );
	win1->registerEventHandler( onClose , new _classCallback( &d , &_dialog::internalEventHandler ) );
	
	d.data[0] = btn1;
	d.data[1] = btn2;
	d.data[2] = lbl1;
	d.data[3] = txt1;
	d.data[4] = win1;
}

void _dialog::imageDialog( _dialog& d , string msg , string windowLbl , const _bitmap& bmp , string okLbl , string otherLbl )
{	
	// Dont allow to overwrite
	if( d.dType != _dialogType::none )
		return;
	
	d.dType = _dialogType::image;
	d.data = new void* [5];
	
	
	// Buttons
	_button* btn1 = new _button( 0 , 0 , okLbl );
	_button* btn2 = nullptr;
	btn1->setAutoSelect( true );
	
	if( !otherLbl.empty() )
		btn2 = new _button( 0 , 0 , otherLbl );
	
	
	// Imagegadget
	_imagegadget* img1 = new _imagegadget( 3 , 3 , bmp );
	
	
	// Labels
	_label* lbl1 = new _label( bmp.getWidth() + 6 , 2 , msg );
	lbl1->setVAlign( _valign::middle );
	
	// Set the height of the label to be minimum the size of the image
	lbl1->setHeight( max( bmp.getHeight() , lbl1->getHeight() ) );
	
	
	// Window-Width
	_length winWidth = max( 55 , max( lbl1->getWidth() + bmp.getWidth() + 10 , ( !otherLbl.empty() ? btn2->getWidth() + 1 : 0 ) + btn1->getWidth() + 4 ) );
	_length winHeight = max( lbl1->getHeight() + btn1->getHeight() + 7 , 30 ) + 11; // + 11 for the window
	
	_window* win1 = new _window( winWidth , winHeight , ( SCREEN_WIDTH - winWidth ) >> 1 , ( SCREEN_HEIGHT - winHeight ) >> 1  , windowLbl , _styleAttr() | _styleAttr::canNotLooseFocus | _styleAttr::notResizeable );
	
	btn1->moveTo( winWidth - btn1->getWidth() - 3 , winHeight - btn1->getHeight() - 12 );
	
	if( !otherLbl.empty() )
		btn2->moveTo( winWidth - btn1->getWidth() - 4 - btn2->getWidth() , winHeight - btn1->getHeight() - 12 );
	
	win1->addChild( btn1 );
	win1->addChild( img1 );
	win1->addChild( lbl1 );
	if( !otherLbl.empty() )
		win1->addChild( btn2 );
	
	btn1->registerEventHandler( onAction , new _classCallback( &d , &_dialog::internalEventHandler ) );
	win1->registerEventHandler( onClose , new _classCallback( &d , &_dialog::internalEventHandler ) );
	if( !otherLbl.empty() )
		btn2->registerEventHandler( onAction , new _classCallback( &d , &_dialog::internalEventHandler ) );
	
	d.data[0] = btn1;
	d.data[1] = btn2;
	d.data[2] = img1;
	d.data[3] = lbl1;
	d.data[4] = win1;
}

void _dialog::execute()
{
	internalExecution();
}

void _dialog::terminate()
{
	internalCleanup();
}