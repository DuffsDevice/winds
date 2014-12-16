#include <_resource/resource.program.taskmanager.h>
#include <_resource/resource.icon.taskmgr.h>
#include <_controller/controller.localization.h>
#include <_controller/controller.memory.h>
#include <_controller/controller.font.h>
#include <_controller/controller.gui.h>
#include <_gadget/gadget.window.dialog.h>
#include <_type/type.windows.h>

PROG_TaskManager::PROG_TaskManager() :
	updateTimer( make_callback( this , &PROG_TaskManager::timerHandler ) , 400 , true )
	, lastCpuValueY( -1 )
	, lastMemoryValueY( -1 )
{
	_programHeader header;
	header.fileIcon = BMP_TaskMgrIcon();
	header.windowIcon = PROG_TaskManager::getLogo();
	header.name = string("Task Manager");
	header.fileName = string("Task Manager");
	header.author = string("WinDS");
	header.description = string("Program that shows currently running Processes and their current memory/cpu consumption");
	this->setHeader( move(header) );
}

void PROG_TaskManager::main( _args args )
{
	_mainFrame* mainFrame = _program::getMainFrame( 109 , 95 , _style::notResizeable );
	mainFrame->setUserEventHandler( onKeyDown , make_callback( &_dialogWindow::submitEscapeHandler ) );
	
	// Simple Indicators
	this->currentCpuUsage = new _imageGadget( 7 , 10 , _bitmap( 29 , 29 , _color::black ) , ignore , ignore , _style::canNotTakeFocus );
	this->currentMemoryUsage = new _imageGadget( 7 , 49 , _bitmap( 29 , 29 , _color::black ) , ignore , ignore , _style::canNotTakeFocus );
	this->currentCpuUsage->setInternalEventHandler( onDraw , make_callback( this , &PROG_TaskManager::usageRefreshHandler ) );
	this->currentMemoryUsage->setInternalEventHandler( onDraw , make_callback( this , &PROG_TaskManager::usageRefreshHandler ) );
	
	// Histograms
	this->histogramCpuUsage = new _imageGadget( 42 , 10 , _bitmap( 28 , 29 , _color::black ) , ignore , ignore , _style::canNotTakeFocus );
	this->histogramCpuUsageSub = new _imageGadget( 72 , 10 , _bitmap( 28 , 29 , _color::black ) , ignore , ignore , _style::canNotTakeFocus );
	this->histogramMemoryUsage = new _imageGadget( 42 , 49 , _bitmap( 58 , 29 , _color::black ) , ignore , ignore , _style::canNotTakeFocus );
	
	// Prepare Histograms
	this->histogramCpuUsage->setInternalEventHandler( onDraw , make_callback( this , &PROG_TaskManager::histogramPrepareHandler ) );
	this->histogramCpuUsageSub->setInternalEventHandler( onDraw , make_callback( this , &PROG_TaskManager::histogramPrepareHandler ) );
	this->histogramMemoryUsage->setInternalEventHandler( onDraw , make_callback( this , &PROG_TaskManager::histogramPrepareHandler ) );
	histogramCpuUsage->redraw();
	histogramCpuUsageSub->redraw();
	histogramMemoryUsage->redraw();
	
	// Register the 'real' refresh-handler
	this->histogramCpuUsage->setInternalEventHandler( onDraw , make_callback( this , &PROG_TaskManager::histogramRefreshHandler ) );
	this->histogramCpuUsageSub->setInternalEventHandler( onDraw , make_callback( this , &PROG_TaskManager::histogramRefreshHandler ) );
	this->histogramMemoryUsage->setInternalEventHandler( onDraw , make_callback( this , &PROG_TaskManager::histogramRefreshHandler ) );
	
	// Labels
	this->cpuLabel = new _label( 8 , 2 , ignore , ignore , _localizationController::getBuiltInString("def_cpu") );
	this->memoryLabel = new _label( 8 , 41 , ignore , ignore , _localizationController::getBuiltInString("def_memory") );
	
	mainFrame->addChild( this->cpuLabel );
	mainFrame->addChild( this->memoryLabel );
	mainFrame->addChild( this->currentCpuUsage );
	mainFrame->addChild( this->currentMemoryUsage );
	mainFrame->addChild( this->histogramCpuUsage );
	mainFrame->addChild( this->histogramCpuUsageSub );
	mainFrame->addChild( this->histogramMemoryUsage );
	
	this->updateTimer.start();
}

void PROG_TaskManager::cleanUp(){
	_program::getMainFrame()->removeChildren( true );
	this->updateTimer.stop();
}

void PROG_TaskManager::drawBorders( _bitmapPort& bP )
{
	// Draw bevelled border
	bP.drawVerticalLine( 0 , 0 , bP.getHeight() , _guiController::getControlFg() );
	bP.drawVerticalLine( bP.getWidth() - 1 , 0 , bP.getHeight() , _color::white );
	bP.drawHorizontalLine( 0 , 0 , bP.getWidth() , _guiController::getControlFg() );
	bP.drawHorizontalLine( 0 , bP.getHeight() - 1 , bP.getWidth() , _color::white );
}

_callbackReturn PROG_TaskManager::histogramPrepareHandler( _event event )
{
	_imageGadget* that = event.getGadget<_imageGadget>();
	_bitmapPort bP = that->getBitmapPort( event );
	
	bP.fill( _color::black );
	bP.drawHorizontalLine( 0 , 7 , bP.getWidth() , gridColor );
	bP.drawHorizontalLine( 0 , 14 , bP.getWidth() , gridColor );
	bP.drawHorizontalLine( 0 , 21 , bP.getWidth() , gridColor );
	
	int curX = 1;
	
	// Draw Vertical lines
	for( curX = 1; curX < bP.getWidth() ; curX++ ){
		if( curX % 7 == 0 )
			bP.drawVerticalLine( bP.getWidth() - curX , 1 , bP.getHeight() - 2 , gridColor );
	}
	this->pixelsBeforeNextGridLine = 1;
	
	return handled;
}

_callbackReturn PROG_TaskManager::histogramRefreshHandler( _event event )
{
	_imageGadget* that = event.getGadget<_imageGadget>();
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Move everything to the left
	that->getBitmap().move( 1 , 0 , 0 , 0 , bP.getWidth() , bP.getHeight() );
	
	// Draw a new background
	if( !this->pixelsBeforeNextGridLine )
		bP.drawVerticalLine( bP.getWidth() - 2 , 0 , bP.getHeight() , gridColor );
	else
		bP.drawVerticalLine( bP.getWidth() - 2 , 0 , bP.getHeight() , _color::black );
	
	
	// Continue the grid
	bP.drawPixel( bP.getWidth() - 2 , 7 , gridColor );
	bP.drawPixel( bP.getWidth() - 2 , 14 , gridColor );
	bP.drawPixel( bP.getWidth() - 2 , 21 , gridColor );
	
	
	_length availHeight = bP.getHeight() - 2;
	
	// Draw Graph
	if( that == this->histogramCpuUsage )
	{
		_s8 newCpuValue = availHeight - availHeight * _windows::getCpuUsage() / 100;
		
		// Either draw dot (first time the graph is painted)
		if( this->lastCpuValueY >= 0 )
			bP.drawLine( bP.getWidth() - 3 , this->lastCpuValueY , bP.getWidth() - 2 , newCpuValue , _color::lime );
		else
			bP.drawPixel( bP.getWidth() - 2 , newCpuValue , _color::lime );
		
		// Save last value for line drawing
		this->lastCpuValueY = newCpuValue;
	}
	else if( that == this->histogramCpuUsageSub )
	{
		_s8 newCpuSubValue = availHeight - availHeight * _windows::getCpuUsageSub() / 100;
		
		// Either draw dot (first time the graph is painted)
		if( this->lastCpuSubValueY >= 0 )
			bP.drawLine( bP.getWidth() - 3 , this->lastCpuSubValueY , bP.getWidth() - 2 , newCpuSubValue , _color::lime );
		else
			bP.drawPixel( bP.getWidth() - 2 , newCpuSubValue , _color::lime );
		
		// Save last value for line drawing
		this->lastCpuSubValueY = newCpuSubValue;
	}
	else{
		availHeight--; // Decrease area because the graph is 2px width
		const _color	graphColor = _color::fromRGB8( 0 , 135 , 255 );
		_u32			freeMemory = _memoryController::getFreeMemory();
		_u32			availMemory = _memoryController::getAvailableMemory();
		_s8				newMemoryValue = availHeight - availHeight * ( availMemory - freeMemory ) / availMemory;
		
		// Either draw dot (first time the graph is painted)
		if( this->lastMemoryValueY >= 0 ){
			bP.drawLine( bP.getWidth() - 3 , this->lastMemoryValueY , bP.getWidth() - 2 , newMemoryValue , graphColor );
			bP.drawLine( bP.getWidth() - 3 , this->lastMemoryValueY + 1 , bP.getWidth() - 2 , newMemoryValue + 1 , graphColor );
		}
		else
			bP.drawVerticalLine( bP.getWidth() - 2 , newMemoryValue , 2 , graphColor );
		
		// Save last value for line drawing
		this->lastMemoryValueY = newMemoryValue;
	}
	
	// Draw bevelled border
	PROG_TaskManager::drawBorders( bP );
	
	return handled;
}

_callbackReturn PROG_TaskManager::usageRefreshHandler( _event event )
{
	_imageGadget* that = event.getGadget<_imageGadget>();
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Clear Everything
	bP.fill( _color::black );
	
	// Draw bevelled border
	PROG_TaskManager::drawBorders( bP );
	
	_u32	rawValue;
	_u8		percentageValue;
	
	if( that == this->currentCpuUsage ){
		percentageValue = _windows::getCpuUsage() * _windows::getCpuUsageSub();
		rawValue = percentageValue;
	}else{
		_u32 freeMemory = _memoryController::getFreeMemory();
		_u32 availMemory = _memoryController::getAvailableMemory();
		rawValue = availMemory - freeMemory;
		percentageValue = rawValue * 100 / availMemory;
	}
	
	_u8		numLines = ( ( percentageValue << 1 ) / 7 ) >> 1;
	bool	startX = true;
	
	for( _u8 i = 0 ; i < 14 ; i++ )
	{
		if( i == 2 || i == 5 || i == 8 || i == 11 )
			continue;
		
		_u8 curY = 16 - i;
		
		if( i <= numLines ){
			bP.drawHorizontalLine( 8 , curY , 6 , _color::lime );
			bP.drawHorizontalLine( 16 , curY , 6 , _color::lime );
		}else{
			bP.drawHorizontalDottedLine( 8 + startX , curY , 5 , _color::green );
			bP.drawHorizontalDottedLine( 16 + startX , curY , 5 , _color::green );
		}
		
		startX = !startX;
	}
	
	_fontHandle	font = _fontController::getStandardFont();
	_u8			fontSize = _fontController::getStandardFontSize();
	string		displayString;
	
	if( that == this->currentCpuUsage )
		displayString = fmt2string( "%d %%" , rawValue );
	else
		displayString = fmt2string( "%.1f MB" , float(rawValue) / float(1024 * 1024) );
	
	bP.drawString( ( 29 + 1 - font->getStringWidth( displayString ) ) >> 1 , 18 , font , displayString , _color::lime , fontSize );
	
	return handled;
}

void PROG_TaskManager::timerHandler(){
	if( !this->pixelsBeforeNextGridLine )
		pixelsBeforeNextGridLine = 7;
	
	this->pixelsBeforeNextGridLine--;
	
	// Redraw everything
	this->currentCpuUsage->redraw();
	this->currentMemoryUsage->redraw();
	this->histogramCpuUsage->redraw();
	this->histogramCpuUsageSub->redraw();
	this->histogramMemoryUsage->redraw();
}

_bitmap PROG_TaskManager::getLogo()
{
	_bitmap logo {6,6,_color::black};
	logo.drawLine( 0 , 4 , 3 , 1 , _color::lime );
	logo.drawPixel( 4 , 2 , _color::lime );
	logo.drawPixel( 5 , 1 , _color::lime );
	return move( logo );
}

const _color PROG_TaskManager::gridColor = _color::fromRGB8( 0 , 89 , 45 );