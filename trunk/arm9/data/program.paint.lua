--FILE_ICON	= %APPDATA%/PaintIcon.png
--AUTHOR	= WinDS
--VERSION	= 1.0
--FILE_NAME	= Paint
--NAME		= Paint
--DESC		= Application to modify Images
--COPYRIGHT = (C) 2014

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
using "UI.MainFrame"
using "UI.ImageGadget"
using "UI.WindowBar"
using "UI.WindowMenu"
using "Dialog.FileSaveDialog"
using "Dialog.FileOpenDialog"
using "Dialog.YesNoDialog"
using "Dialog.EnterTextDialog"

-- Supported Filetypes
local possibleSaveTypes = {
	[1] = { ["first"] = "Windows Bitmap" , ["second"] = "bmp" } ,
	[2] = { ["first"] = "JPEG" , ["second"] = "jpg" } ,
	[3] = { ["first"] = "Ptl. Ntwrk. Graphic" , ["second"] = "png" } ,
	[4] = { ["first"] = "Gfx. Interchange Fmt." , ["second"] = "gif" }
}

local possibleOpenTypes = {
	[1] = { ["first"] = "Image" , ["second"] = "bmp,jpg,png,gif" }
}

local APPDATA = {
	window = nil ,
	windowBar = nil ,
	windowMenu = nil ,
	canvas = nil , -- Bitmap for image editing
	greyBackground = nil , -- Holds The Canvas
	curColor = "red" , -- Current Paint color
	imageFile = nil , -- Currently opened File
	dialog = nil , -- Variable to hold any type of dialog
	changes = false ,
	colorIndicator = nil
}

function main( filename )

	local wndIcon = Bitmap( 6 , 6 , "red" )
	wndIcon.drawFilledRect( 3 , 0 , 3 , 3 , Color.fromRGB8( 181 , 230 , 29 ) )
	wndIcon.drawFilledRect( 3 , 3 , 3 , 3 , Color.fromRGB8( 255 , 210 , 14 ) )
	wndIcon.drawFilledRect( 0 , 0 , 3 , 3 , Color.fromRGB8( 255 , 127 , 39 ) )
	wndIcon.drawFilledRect( 0 , 3 , 3 , 3 , Color.fromRGB8( 153 , 217 , 255 ) )
	
	local windowRawMenu = WindowMenu.getStandardMenu()
	
	-- Edit the menu and add the 'Edit' Field
	local mainList = windowRawMenu.getList()
	mainList[2] = { System.getLocalizedString("lbl_edit") , 2 }
	windowRawMenu.setList( mainList ) -- Set Main List
	windowRawMenu.setList( 2 , { System.getLocalizedString("lbl_size") } ) -- Reference a list
	
	-- Add a WindowMenu built from the _menu we constructed
	windowRawMenu.addHandler( MenuRule( 1 , 1 , 4 ) , menu1Handler ) -- Add our Handler for "open", "save" and "save as..."
	windowRawMenu.addHandler( MenuRule( 2 , 1 ) , menu2Handler ) -- Add our Handler for "edit -> size"
	APPDATA.windowMenu = WindowMenu( windowRawMenu )

	
	if filename ~= nil then
		APPDATA.imageFile = ImageFile( filename )
		loadImageFile()
	else
		createEmptyCanvas()
	end
	
	local bg = ImageGadget( 0 , 9 , Bitmap( 150 , 140 ) , "draggable | smallDragThld | noTransparentParts" )
	bg.setInternalEventHandler( "onDraw" , refreshHandler )
	bg.setUserEventHandler( "onDragging" , dragHandler )
	bg.setUserEventHandler( "onDragStop" , dragHandler )
	bg.setUserEventHandler( "onDragStart" , dragHandler )
	bg.setUserEventHandler( "onMouseDown" , dragHandler )
	bg.setUserEventHandler( "onMouseUp" , dragHandler )
	APPDATA.greyBackground = bg
	bg.redraw()
	--APPDATA.colorIndicator = ImageGadget( 2 , 2 , Bitmap( 6 , 6 , "red" ) )
	APPDATA.windowBar = WindowBar( 10 )
	--APPDATA.windowBar.addChild( APPDATA.colorIndicator )
	
	local frame = System.getMainFrame( 100 , 100 )
	frame.icon = wndIcon
	
	frame.addChild( APPDATA.greyBackground )
	frame.setUserEventHandler( "onResize" , windowResizeHandler )
	frame.setUserEventHandler( "onClose" , windowExitHandler )
	frame.addEnhancedChild( APPDATA.windowMenu )
	frame.addEnhancedChild( APPDATA.windowBar )
	APPDATA.window = frame
	updateWindowTitle()
end


-- Handles Clicks into the window 'file' menu!
function menu1Handler( listIndex , entryIndex )

	-- Don't open two dialogs at once
	if APPDATA.dialog ~= nil and APPDATA.dialog.running then return end
	
	if entryIndex == 4 then -- 'save as...'
		APPDATA.dialog = FileSaveDialog( possibleSaveTypes , nil , 1 , nil , System.getLocalizedString("lbl_save_as") )
		APPDATA.dialog.execute()
		APPDATA.dialog.setCallback( saveDialogHandler )
	elseif entryIndex == 3 then -- 'save'
		if APPDATA.imageFile == nil then
			APPDATA.dialog = FileSaveDialog( possibleSaveTypes , nil , 1 )
			APPDATA.dialog.execute()
			APPDATA.dialog.setCallback( saveDialogHandler )
		else
			APPDATA.changes = false
			APPDATA.imageFile.writeBitmap( APPDATA.canvas )
		end
	elseif entryIndex == 2 then -- 'open'
		if APPDATA.changes then
			runDiscardChangeDialog( saveBeforeOpenDialogHandler )
		else
			openFileDialog()
		end
	elseif entryIndex == 1 then -- 'new'
		if APPDATA.changes then
			runDiscardChangeDialog( saveBeforeNewDialogHandler )
		else
			createEmptyCanvas()
		end
	end
end

function menu2Handler( listIndex , entryIndex )
	
	-- Don't open two dialogs at once
	if APPDATA.dialog ~= nil and APPDATA.dialog.running then return end
	
	if entryIndex == 1 then
		--APPDATA.dialog = EnterTextDialog
		System.debug("Feature Not Available Yet!")
	end
end

-- Open File Dialog
function openFileDialog()
	APPDATA.dialog = FileOpenDialog( possibleOpenTypes , nil , 1 )
	APPDATA.dialog.setCallback(
		function( result )
			if result == "yes" then
				APPDATA.imageFile = ImageFile( APPDATA.dialog.fileName )
				loadImageFile()
			end
		end
	)
	APPDATA.dialog.execute()
end



-- Will Be invoked when the dialog about discarding changes
-- is over and all necessary instructions are done
function saveBeforeOpenDialogHandler( result )
	if result ~= "cancel" then -- If the operation was not cancelled: continue opening
		openFileDialog()
	end
end
function saveBeforeNewDialogHandler( result )
	if result ~= "cancel" then
		createEmptyCanvas()
	end
end


-- Creates a new unsafed empyt canvas
function createEmptyCanvas()
	APPDATA.imageFile = nil
	if APPDATA.canvas ~= nil and APPDATA.canvas.isValid() then
		APPDATA.canvas.reset( "white" )
	else
		APPDATA.canvas = Bitmap( 50 , 50 , "white" )
	end
	redrawCanvas()
	APPDATA.changes = false
end


-- Updates the greyBackground
function redrawCanvas( area )
	if APPDATA.greyBackground ~= nil then
		local pos = APPDATA.greyBackground.getAbsolutePosition()
		local allRect = Rect( pos[1] + 2 , pos[2] + 2 , APPDATA.canvas.width , APPDATA.canvas.height );
		APPDATA.greyBackground.redraw( area and area.clipToIntersect( allRect ) or allRect )
	end
end


-- Updates the window's title
function updateWindowTitle()
	if APPDATA.window ~= nil then
		if APPDATA.imageFile ~= nil then
			APPDATA.window.title = APPDATA.imageFile.getFullName() .. " - Paint"
		else
			APPDATA.window.title = System.getLocalizedString("lbl_unnamed") .. " - Paint"
		end
	end
end

-- Loads a new canvas by using the global 'imageFile'
function loadImageFile()
	if APPDATA.imageFile == nil then return end -- Abort if invalid imagefile
	
	local temp = APPDATA.imageFile.readBitmap()
	if temp ~= nil then
		APPDATA.canvas = temp
		redrawCanvas()
	else
		APPDATA.imageFile = nil
		createEmptyCanvas()
	end
	APPDATA.changes = false
	updateWindowTitle()
end





-- Handles Actions you can take at an fileSaveDialog
function saveDialogHandler( result )
	if result == "yes" then
		local output = ImageFile( APPDATA.dialog.fileName )
		output.writeBitmap( APPDATA.canvas , APPDATA.dialog.mimeType )
		if APPDATA.imageFile == nil then
			APPDATA.imageFile = output
			APPDATA.changes = false
		end
	end
end

-- Handles Clicks to the window's close button
function windowExitHandler()
	if APPDATA.changes == false then
		return "handled"
	else
		runDiscardChangeDialog(
			function(result)
				if result ~= "cancel" then System.exit() end
			end
		)
	end
	return "prevent_default"
end



-- Runs A Discard Changes Dialog
-- Callback will be invoked with either
-- "yes" => File Saved
-- "no" => Don't Save File (Discard Changes)
-- "cancel" => Abort Situation!
function runDiscardChangeDialog( callback )
	local func =
		function( result )
			if result == "yes" then -- 
				if APPDATA.imageFile == nil then
					APPDATA.dialog = FileSaveDialog( possibleSaveTypes , nil , 1 )
					APPDATA.dialog.setCallback(
						function( result )
							saveDialogHandler( result ) -- If FileSaveDialog returns "yes" -> save file!
							callback( result ) -- Inform what happened
						end
					)
					APPDATA.dialog.execute()
				else
					APPDATA.changes = false
					APPDATA.imageFile.writeBitmap( APPDATA.canvas )
					callback( result ) -- Inform what happened
				end
			else
				callback( result )
			end
		end
	APPDATA.dialog = YesNoDialog( "Discard changes?" , "Save" , "Save" , "Discard" )
	APPDATA.dialog.setCallback( func )
	APPDATA.dialog.execute()
end





-- Handles Window Resizes
function windowResizeHandler()
	local clientRect = APPDATA.window.getClientRect()
	APPDATA.greyBackground.setSize( clientRect.width , clientRect.height )
	return "use_internal"
end

-- Refresh The Canvas
function refreshHandler( event )
	local port = APPDATA.greyBackground.getBitmapPort( event )
	
	port.fill( "gray" )
	local flag = 1
	for i = 0 , APPDATA.canvas.height do
		port.drawHorizontalDottedLine( flag + 2 , i + 2 , APPDATA.canvas.width - flag , "white" )
		flag = flag ~ 1
	end
	port.copyTransparent( 2 , 2 , APPDATA.canvas )
	port.drawVerticalLine( APPDATA.canvas.width + 2 , 3 , APPDATA.canvas.height - 1 , "black" )
	port.drawHorizontalLine( 3 , APPDATA.canvas.height + 2 , APPDATA.canvas.width , "black" )
	return "handled"
end

-- Paint Engine!
local lastX = -1
local lastY = -1
local pressed = false

function dragHandler( event )
	local curX = event.posX - 2
	local curY = event.posY - 2
	
	if event.type == "onDragging" or event.type == "onDragStop" then
		curY = curY - APPDATA.greyBackground.y
		curX = curX - APPDATA.greyBackground.x
	end
	if pressed then
		APPDATA.canvas.drawLine( lastX , lastY , curX , curY , APPDATA.curColor )
	else
		APPDATA.canvas.drawPixel( curX , curY , APPDATA.curColor )
	end
	redrawCanvas( Rect.fromCoords( lastX , lastY , curX , curY ) )
	lastX = curX
	lastY = curY
	APPDATA.changes = true

	if event.type == "onDragStop" or event.type == "onMouseUp" then
		pressed = false
	else
		pressed = true
	end
	return "handled"
end