--IMG = %APPDATA%/CalcIcon.bmp
--AUTHOR=Jakob Riedle (DuffsDevice)
--VERSION = 1.0
--DESC = Application to modify Images
--COPYRIGHT = (C) 2014
--LANG = neutral

using "System.Event"
using "Drawing.Bitmap"
using "Drawing.Font"
using "Drawing.Color"
using "UI.Gadget"
using "UI.Button"
using "UI.Label"
using "UI.Window"

local wnd
local btn1, btn2, btn3, btn4, btn5, btn6, btn7, btn8, btn9, btn0
local btnClear, btnBackspace, btnEqual, btnComma, btnAdd, btnSub, btnMul, btnDiv, btnSqrt, btnSign
local label

local CURNUMBER = "0"
local OPERAND_FIRST = nil
local OPERAND_SECOND = nil
local OPERATOR = nil

-- Calculator State:
-- 1: Entering First Number
-- 2: Operator Set
-- 3: Entering Second Number
local calcState = 1

function main()
	
	local btnStartX = 1
	local btnStartY = 12
	local btnWidth = 17
	local btnHeight = 12
	local btnWidthEx = btnWidth + 1;
	local btnHeightEx = btnHeight + 1;
	local winWidth = btnStartX + btnWidthEx * 4 + 2
	local winHeight = btnStartY + btnHeightEx * 5 + 11
	
	-- Create Window
	wnd = Window( 40 , 5 , winWidth , winHeight , "Calculator" , getLogo() , true , true , "notResizeable | draggable" )
	
	-- Create Label
	label = Label( 1 , 1 , winWidth - 3 , 10 , CURNUMBER )
	label.align = "right"
	label.bgColor = Color.fromRGB( 27 , 29 , 31 )
	
	--------------------
	-- Create Buttons --
	--------------------
	
	-- First row
	btnClear		= Button( btnStartX , btnStartY , btnWidth , btnHeight , "C" )
	btnBackspace	= Button( btnStartX + btnWidthEx * 1 , btnStartY , btnWidth , btnHeight , "¨" )
	btnAdd			= Button( btnStartX + btnWidthEx * 2 , btnStartY , btnWidth , btnHeight , "+" )
	btnSub			= Button( btnStartX + btnWidthEx * 3 , btnStartY , btnWidth , btnHeight , "-" )
	
	-- Second
	btn1		= Button( btnStartX , btnStartY + btnHeightEx * 1 , btnWidth , btnHeight , "1" )
	btn2		= Button( btnStartX + btnWidthEx * 1 , btnStartY + btnHeightEx * 1 , btnWidth , btnHeight , "2" )
	btn3		= Button( btnStartX + btnWidthEx * 2 , btnStartY + btnHeightEx * 1 , btnWidth , btnHeight , "3" )
	btnMul		= Button( btnStartX + btnWidthEx * 3 , btnStartY + btnHeightEx * 1 , btnWidth , btnHeight , "°" )
	
	-- Third
	btn4		= Button( btnStartX , btnStartY + btnHeightEx * 2 , btnWidth , btnHeight , "4" )
	btn5		= Button( btnStartX + btnWidthEx * 1 , btnStartY + btnHeightEx * 2 , btnWidth , btnHeight , "5" )
	btn6		= Button( btnStartX + btnWidthEx * 2 , btnStartY + btnHeightEx * 2 , btnWidth , btnHeight , "6" )
	btnDiv		= Button( btnStartX + btnWidthEx * 3 , btnStartY + btnHeightEx * 2 , btnWidth , btnHeight , "±" )
	
	-- Fourth
	btn7		= Button( btnStartX , btnStartY + btnHeightEx * 3 , btnWidth , btnHeight , "7" )
	btn8		= Button( btnStartX + btnWidthEx * 1 , btnStartY + btnHeightEx * 3 , btnWidth , btnHeight , "8" )
	btn9		= Button( btnStartX + btnWidthEx * 2 , btnStartY + btnHeightEx * 3 , btnWidth , btnHeight , "9" )
	btnSqrt		= Button( btnStartX + btnWidthEx * 3 , btnStartY + btnHeightEx * 3 , btnWidth , btnHeight , "." )
	
	-- Fifth
	btnSign		= Button( btnStartX , btnStartY + btnHeightEx * 4 , btnWidth , btnHeight , "±" )
	btn0		= Button( btnStartX + btnWidthEx * 1 , btnStartY + btnHeightEx * 4 , btnWidth , btnHeight , "0" )
	btnComma	= Button( btnStartX + btnWidthEx * 2 , btnStartY + btnHeightEx * 4 , btnWidth , btnHeight , "." )
	btnEqual	= Button( btnStartX + btnWidthEx * 3 , btnStartY + btnHeightEx * 4 , btnWidth , btnHeight , "=" )
	
	local symbolFont = System.getFont("SystemSymbols8")
	
	-- Adjust some fonts
	btnSqrt.font = symbolFont
	btnMul.font = symbolFont
	btnDiv.font = symbolFont
	btnBackspace.font = symbolFont
	
	-- Add Buttons
	wnd.addChild( btn0 )
	wnd.addChild( btn1 )
	wnd.addChild( btn2 )
	wnd.addChild( btn3 )
	wnd.addChild( btn4 )
	wnd.addChild( btn5 )
	wnd.addChild( btn5 )
	wnd.addChild( btn6 )
	wnd.addChild( btn7 )
	wnd.addChild( btn8 )
	wnd.addChild( btn9 )
	wnd.addChild( btnComma )
	wnd.addChild( btnAdd )
	wnd.addChild( btnSub )
	wnd.addChild( btnMul )
	wnd.addChild( btnDiv )
	wnd.addChild( btnBackspace )
	wnd.addChild( btnClear )
	wnd.addChild( btnEqual )
	wnd.addChild( btnSqrt )
	wnd.addChild( btnSign )
	
	btn1.setUserEventHandler( "onMouseClick" , numberHandler )
	btn2.setUserEventHandler( "onMouseClick" , numberHandler )
	btn3.setUserEventHandler( "onMouseClick" , numberHandler )
	btn4.setUserEventHandler( "onMouseClick" , numberHandler )
	btn5.setUserEventHandler( "onMouseClick" , numberHandler )
	btn6.setUserEventHandler( "onMouseClick" , numberHandler )
	btn7.setUserEventHandler( "onMouseClick" , numberHandler )
	btn8.setUserEventHandler( "onMouseClick" , numberHandler )
	btn9.setUserEventHandler( "onMouseClick" , numberHandler )
	btn0.setUserEventHandler( "onMouseClick" , numberHandler )
	btnComma.setUserEventHandler( "onMouseClick" , numberHandler )
	
	btnAdd.setUserEventHandler( "onMouseClick" , stuffHandler )
	btnSub.setUserEventHandler( "onMouseClick" , stuffHandler )
	btnMul.setUserEventHandler( "onMouseClick" , stuffHandler )
	btnDiv.setUserEventHandler( "onMouseClick" , stuffHandler )
	btnSqrt.setUserEventHandler( "onMouseClick" , stuffHandler )
	btnBackspace.setUserEventHandler( "onMouseClick" , stuffHandler )
	btnClear.setUserEventHandler( "onMouseClick" , stuffHandler )
	btnEqual.setUserEventHandler( "onMouseClick" , stuffHandler )
	btnSign.setUserEventHandler( "onMouseClick" , stuffHandler )
	
	-- Add Label
	wnd.addChild( label )
	
	System.addChild( wnd )
end

function numberHandler( event )

	local number = event.gadget.text
	
	-- If an operator was set: store last entered number as OPERAND_FIRST
	if calcState == 2 then
		OPERAND_FIRST = tonumber( CURNUMBER )
		CURNUMBER = ""
		calcState = 3
	end
	
	if number == "." and string.find( CURNUMBER , "." , 1 , true ) ~= nil then
		return "use_internal"
	end
	
	-- Concat Number
	CURNUMBER = CURNUMBER .. number
	
	-- Ensure no leading '0'
	if string.sub( CURNUMBER , 1 , 1 ) == "0" and string.sub( CURNUMBER , 2 , 2 ) ~= "." then
		CURNUMBER = string.sub( CURNUMBER , 2 )
	end
	
	-- Apply to label
	label.text = CURNUMBER
	
	return "use_internal"
end

function stuffHandler( event )

	-- Fetch gadget of event
	local gadget = event.gadget
	if gadget.equals( btnAdd ) then
		set_operator( op_add )
	elseif gadget.equals( btnSub ) then
		set_operator( op_sub )
	elseif gadget.equals( btnMul ) then
		set_operator( op_mul )
	elseif gadget.equals( btnDiv ) then
		set_operator( op_div )
	elseif gadget.equals( btnSign ) then
		if calcState == 2 then
			calcState = 1
			OPERATOR = nil
		end
		CURNUMBER = tostring( 0 - tonumber(CURNUMBER) )
		-- Apply to label
		label.text = CURNUMBER
	elseif gadget.equals( btnClear ) then
		--reset calcState
		calcState = 1
		OPERAND_FIRST = nil
		OPERAND_SECOND = nil
		OPERATOR = nil
		CURNUMBER = "0"
		label.text = "0"
	elseif gadget.equals( btnSqrt ) then
		if calcState == 3 then
			solve() -- Solve equitation first, then build square root out of result
		end
		-- build number
		OPERAND_FIRST = tonumber( CURNUMBER )
		
		-- compute and apply
		OPERAND_FIRST = math.sqrt( OPERAND_FIRST )
		CURNUMBER = tostring(OPERAND_FIRST)
		label.text = CURNUMBER
		
		-- reset calcState
		calcState = 1
		OPERAND_FIRST = nil
		OPERATOR = nil
	elseif gadget.equals( btnEqual ) then
		if calcState == 3 then -- When both operands and operator are set: compute!
			solve()
		end
		calcState = 1
	elseif gadget.equals( btnBackspace ) then
		removeNumber()
	end
end

function set_operator( op )
	-- If we already have a valid expression:
	-- solve equitation and continue with the next operand
	if calcState == 3 then
		solve()
	end
	
	-- Set operator
	OPERATOR = op
	
	calcState = 2
end

function removeNumber()

	-- Remove Operator
	if calcState == 2 then
		OPERATOR = nil
		calcState = 1
	end
	
	-- Write '0' instead of ''
	if #CURNUMBER == 1 then
		CURNUMBER = "0"
	else
		CURNUMBER = string.sub( CURNUMBER , 1 , -2 )
	end
	
	-- Write '0' instead of '-'
	if CURNUMBER == "-" then
		CURNUMBER = "0"
	end
	
	label.text = CURNUMBER
end

function solve()

	-- Check if all neccesary things have been assembled
	if calcState == 3 then
		OPERAND_SECOND = tonumber(CURNUMBER) -- Build Seocnd operand from current displayed number
		OPERAND_FIRST = OPERATOR( OPERAND_FIRST , OPERAND_SECOND ) -- Write result
		
		CURNUMBER = tostring(OPERAND_FIRST)
		label.text = CURNUMBER
		
		-- reset
		OPERAND_FIRST = nil
		OPERATOR = nil
		OPERAND_SECOND = nil
		calcState = 1
	end
end

function getLogo()
	local col = Color.fromRGB( 27 , 27 , 0 )
	local logo = Bitmap( 6 , 6 , "black" )
	logo.drawVerticalLine( 1 , 0 , 6 , col )
	logo.drawHorizontalLine( 2 , 0 , 2 , col )
	logo.drawPixel( 0 , 3 , col )
	logo.drawPixel( 4 , 3 , col )
	logo.drawPixel( 3 , 2 , col )
	logo.drawPixel( 5 , 4 , col )
	logo.drawPixel( 3 , 4 , col )
	logo.drawPixel( 5 , 2 , col )
	return logo
end

-- Basic Operations
function op_add( first , second ) return first + second end
function op_sub( first , second ) return first - second end
function op_mul( first , second ) return first * second end
function op_div( first , second ) return first / second end