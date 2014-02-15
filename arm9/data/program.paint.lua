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
using "System.MenuRule"
using "System.MimeType"
using "System.Timer"
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
using "Dialog.FileSaveDialog"
using "Dialog.YesNoDialog"

-- Supported Filetypes
local possibleFileTypes = {
	{ 1 , { "Windows Bitmap" , "bmp" } } ,
	{ 2 , { "JPEG" , "jpg" } } ,
	{ 3 , { "Ptl. Ntwrk. Graphics" , "png" } } ,
	{ 4 , { "GIF" , "gif" } }
}

local wnd
local windowBar
local windowMenu
local canvas -- Bitmap for image editing
local greyBackground -- Holds The Canvas
local curColor = "red" -- Current Paint color
local imageFile -- Currently opened File
local windowTitle = System.getLocalizedString("lbl_unnamed")
local dialog = nil -- Variable to hold any type of dialog
local changes = false
local closeOnSave = false

function main( filename )

	local wndIcon = Bitmap( 6 , 6 , "red" )
	wndIcon.drawFilledRect( 3 , 0 , 3 , 3 , Color.fromRGB8( 181 , 230 , 29 ) )
	wndIcon.drawFilledRect( 3 , 3 , 3 , 3 , Color.fromRGB8( 255 , 210 , 14 ) )
	wndIcon.drawFilledRect( 0 , 0 , 3 , 3 , Color.fromRGB8( 255 , 127 , 39 ) )
	wndIcon.drawFilledRect( 0 , 3 , 3 , 3 , Color.fromRGB8( 153 , 217 , 255 ) )
	
	local windowRawMenu = WindowMenu.getStandardMenu()
	
	-- Edit the menu and add the 'Edit' Field
	local mainList = windowRawMenu.mainList
	mainList[2] = { 2 , System.getLocalizedString("lbl_edit") }
	windowRawMenu.mainList = mainList -- Set Main List
	windowRawMenu.setList( 2 , { { 1 , System.getLocalizedString("lbl_size") } } ) -- Reference a list
	
	-- Add a WindowMenu built from the _menu we constructed
	windowRawMenu.addMenuHandler( MenuRule( 1 , 0 , 2 ) , menuHandler ) -- Add our Handler for "open", "save" and "save as..."
	windowMenu = WindowMenu( windowRawMenu )

	
	if filename ~= nil then
		imageFile = ImageFile( filename )
		
		local temp = imageFile.readBitmap()
		
		if temp ~= nil then
			canvas = Bitmap( temp.width , temp.height )
			canvas.copy( 0 , 0 , temp )
			local file = Direntry( filename )
			windowTitle = file.displayName
		else
			imageFile = nil
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
	
	windowBar = WindowBar()
	wnd = Window( 10 , 10 , 100 , 100 , windowTitle .. " - Paint" , wndIcon , true , "minimizeable|draggable" )
	wnd.addChild( greyBackground )
	wnd.setUserEventHandler( "onResize" , windowResizeHandler )
	wnd.setUserEventHandler( "onClose" , windowExitHandler )
	wnd.addEnhancedChild( windowMenu )
	wnd.addEnhancedChild( windowBar )
	
	System.addChild( wnd )

end


-- Handles Clicks into the window menu!
function menuHandler( listIndex , entryIndex )

	-- Don't open two dialogs at once
	if dialog ~= nil then return end
	
	print( imageFile )
	if entryIndex == 2 or ( entryIndex == 1 and imageFile == nil ) then -- 'save as...' or first time 'save'
		dialog = FileSaveDialog( possibleFileTypes , nil , 1 )
		dialog.execute()
		dialog.setCallback( dialogSaveHandler )
	elseif entryIndex == 1 then -- 'save'
		imageFile.writeBitmap( canvas )
		changes = false
	elseif entryIndex == 0 then -- 'open'
		--dialog = FileSaveDialog( possibleFileTypes )
		--dialog.execute()
		--dialog.setCallback( dialogSaveHandler )
	end
end


-- Handles Actions you can take at an fileSaveDialog
function dialogSaveHandler( result )
	if result == "yes" then
		local output = ImageFile( dialog.fileName )
		output.writeBitmap( canvas , dialog.mimeType )
		if imageFile == nil then
			imageFile = output
			changes = false
			if closeOnSave then
				System.exit()
			end
		end
	end
	dialog = nil
	closeOnSave = false
end


-- Handles Window Resizes
function windowResizeHandler()
	local clientRect = wnd.getClientRect()
	greyBackground.setSize( clientRect.width , clientRect.height )
	return "use_internal"
end


-- Refresh The Canvas
function refreshHandler( event )
	local port = greyBackground.getBitmapPort( event )
	
	port.fill( "gray" )
	port.copy( 2 , 2 , canvas )
	port.drawVerticalLine( canvas.width + 2 , 3 , canvas.height - 1 , "black" )
	port.drawHorizontalLine( 3 , canvas.height + 2 , canvas.width , "black" )
	return "handled"
end


-- Paint Engine!
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
		canvas.drawLine( lastX , lastY , curX , curY , curColor )
	else
		canvas.drawPixel( curX , curY , curColor )
	end
	lastX = curX
	lastY = curY
	greyBackground.redraw()
	changes = true

	if event.type == "onDragStop" or event.type == "onMouseUp" then
		pressed = false
	else
		pressed = true
	end
	return "handled"
end

-- Handles the result from the 'Discard Changes'-Dialog
function windowExitDialogHandler( result )
	if result == "yes" then -- Try to Save File!
		dialog = FileSaveDialog( possibleFileTypes , nil , 1 )
		dialog.setCallback( dialogSaveHandler )
		closeOnSave = true
		dialog.execute()
	else -- Discard Changes!
		System.exit()
	end
end

-- Handles Clicks to the window's close button
function windowExitHandler()
	if changes == false then
		System.exit()
	else
		dialog = YesNoDialog( "Discard changes?" , "Save" , "Save" , "Discard" )
		dialog.setCallback( windowExitDialogHandler )
		dialog.execute()
	end
	return "prevent_default"
end