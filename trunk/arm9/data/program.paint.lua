--	IMG = %APPDATA%/PaintIcon.png
--	AUTHOR = Jakob Riedle (DuffsDevice)
--	VERSION = 1.0
--	DESC = Application to modify Images
--	COPYRIGHT = (C) 2014
--	LANG = neutral

using "System.Event"
using "System.ImageFile"
using "System.Direntry"
using "System.Menu"
using "Drawing.Font"
using "Drawing.Bitmap"
using "Drawing.BitmapPort"
using "Drawing.Area"
using "Drawing.Rect"
using "Drawing.Color"
using "UI.Gadget"
using "UI.Window"
using "UI.ImageGadget"
using "UI.WindowBar"
using "UI.WindowMenu"

local wnd
local windowBar
local windowMenu
local canvas
local greyBackground
local curColor = "red"
local imagefile
local windowTitle = System.getLocalizedString("lbl_unnamed")

function main( filename )

	local wndIcon = Bitmap( 6 , 6 , "red" )
	wndIcon.drawFilledRect( 3 , 0 , 3 , 3 , Color.fromRGB8( 181 , 230 , 29 ) )
	wndIcon.drawFilledRect( 3 , 3 , 3 , 3 , Color.fromRGB8( 255 , 210 , 14 ) )
	wndIcon.drawFilledRect( 0 , 0 , 3 , 3 , Color.fromRGB8( 255 , 127 , 39 ) )
	wndIcon.drawFilledRect( 0 , 3 , 3 , 3 , Color.fromRGB8( 153 , 217 , 255 ) )
	
	windowBar = WindowBar()
	
	local menu = WindowMenu.getStandardMenu()
	windowMenu = WindowMenu( menu )
	
	if filename ~= nil then
		imagefile = ImageFile( filename )
		
		local temp = imagefile.readBitmap()
		
		if temp ~= nil then
			canvas = Bitmap( temp.width , temp.height )
			canvas.copy( 0 , 0 , temp )
			local file = Direntry( filename )
			windowTitle = file.displayName
		else
			canvas = Bitmap( 65 , 55 , "white" )
		end
	else
		canvas = Bitmap( 100 , 80 , "white" )
	end
	
	greyBackground = ImageGadget( 0 , 9 , Bitmap( 150 , 140 ) , "draggable | smallDragTrig" )
	greyBackground.setInternalEventHandler( "onDraw" , refreshHandler )
	greyBackground.setUserEventHandler( "onDragging" , dragHandler )
	greyBackground.setUserEventHandler( "onDragStop" , dragHandler )
	greyBackground.setUserEventHandler( "onDragStart" , dragHandler )
	greyBackground.setUserEventHandler( "onMouseDown" , dragHandler )
	greyBackground.setUserEventHandler( "onMouseUp" , dragHandler )
	greyBackground.redraw()
	
	wnd = Window( 10 , 10 , 100 , 100 , windowTitle .. " - Paint" , wndIcon , true , "minimizeable|draggable" )
	wnd.addChild( greyBackground )
	wnd.setUserEventHandler( "onResize" , windowResizeHandler )
	wnd.setUserEventHandler( "onClose" , System.exit )
	wnd.addEnhancedChild( windowMenu )
	wnd.addEnhancedChild( windowBar )
	
	System.addChild( wnd )

end

local lastX = -1
local lastY = -1
local pressed = false

function dragHandler( event )
	local curX = event.posX - 2
	local curY = event.posY - 2
	if event.type == "onDragging" then
		curY = curY - greyBackground.y
		curX = curX - greyBackground.x
	end
	if pressed then
		canvas.drawFilledCircle( curX , curY , event.pressure / 100 , curColor )
		--canvas.drawLine( lastX , lastY , curX , curY , curColor )
	else
		canvas.drawFilledCircle( curX , curY , event.pressure / 100 , curColor )
		--canvas.drawPixel( curX , curY , curColor )
	end
	lastX = curX
	lastY = curY
	greyBackground.redraw()

	if event.type == "onDragStop" or event.type == "onMouseUp" then
		pressed = false
	else
		pressed = true
	end
	return "handled"
end

function refreshHandler( event )
	local port = greyBackground.getBitmapPort( event )
	
	port.fill( "gray" )
	port.copy( 2 , 2 , canvas )
	port.drawVerticalLine( canvas.width + 2 , 3 , canvas.height - 1 , "black" )
	port.drawHorizontalLine( 3 , canvas.height + 2 , canvas.width , "black" )
	return "handled"
end

function windowResizeHandler()
	local clientRect = wnd.getClientRect()
	greyBackground.setSize( clientRect.width , clientRect.height )
	return "use_internal"
end