--NAME		= Pong
--FILE_ICON	= %APPDATA%/PongIcon.bmp
--FILE_NAME	= %APPDATA%/PongIcon.bmp
--AUTHOR	= WinDS
--VERSION	= 1.0
--DESC		= Simple Cover of the famous Game 'Pong'
--COPYRIGHT	= (C) 2014

using "Drawing.Area"
using "Drawing.Bitmap"
using "Drawing.BitmapPort"
using "Drawing.Color"
using "Drawing.Font"
using "System.Event"
using "System.Timer"
using "UI.MainFrame"
using "UI.Gadget"
using "UI.Button"
using "UI.ImageGadget"

-- Gadgets
local pongBmp
local window
local timer

local gameState = 0 -- Indicates the state of the Game:
-- 0 = User has Ball
-- 1 = Computer has Ball
-- 2 = Ball in game

local p1 = 0
local p2 = 0
local ballX = -10
local ballY = -10
local ballVX = 1
local ballVY = 1
local pLength = 15 -- width of the paddle
local gameHeight = 60
local gameWidth = 100
local timer
local time


-- Parameters that are used when the KI should start the game
local computerDestination
local computerShootsIn


-- Scores!
local scoreUser = 0
local scoreComputer = 0


-- Variables that indicate the state of buttons
local keyUpPressed = false
local keyDownPressed = false


-- Handles the case where the ball is near
-- the left Border and eventually sets the gameState
function handleUserCollision()
	if ballY > p1 and ballY < p1 + pLength then
		ballVX = -ballVX
	else
		gameState = 0
		scoreComputer = scoreComputer + 1
	end
end

-- Handles the case where the ball is near
-- the right Border
function handleComputerCollision()
	if ballY > p2 and ballY < p2 + pLength then
		ballVX = -ballVX
	else
		gameState = 1
		computerShootsIn = math.random( 30 , 120 ) -- Frames 'till the computer will shoot the ball
		computerDestination = math.random( 0 , gameHeight - pLength ) -- Position where the computer tries to release the Ball
		
		scoreUser = scoreUser + 1 -- Increase the users score
	end
end

-- Main entry point
function main()
	
	-- reset player coordinates
	p1 = ( gameHeight - pLength ) / 2
	p2 = p1
	
	local canvas = Bitmap( gameWidth , gameHeight )
	
	-- create the image
	pongBmp = ImageGadget( 0 , 0 , canvas )
	pongBmp.setUserEventHandler( "onDraw" , refresher )
	
	local bmp = Bitmap(6, 6)
	
	bmp.reset( "black" )
	bmp.drawVerticalLine( 0 , 1 , 3 , "lime" )
	bmp.drawVerticalLine( 5 , 3 , 3 , "lime" )
	bmp.drawFilledRect( 2 , 2 , 2 , 2 , "fuchsia" )
	
	window = System.getMainFrame( gameWidth + 2 , gameHeight + 11 , "notResizeable" )
	window.icon = bmp
	
	window.addChild( pongBmp )
	window.setUserEventHandler( "onKeyDown" , eventHandler )
	window.setUserEventHandler( "onKeyUp" , eventHandler )
	window.setUserEventHandler( "onBlur" , eventHandler )
	window.setUserEventHandler( "onFocus" , eventHandler )
	
	timer = Timer( frame , 1000/60 , true )
end

function frame()
	move()
	pongBmp.redraw()
end

function move()
	
	if keyUpPressed then
		p1 = p1 - 0.5
	elseif keyDownPressed then
		p1 = p1 + 0.5
	end
	
	-- check bounds
	p1 = math.min( gameHeight - pLength - 2 , math.max( 0 , p1 ) )
	p2 = math.min( gameHeight - pLength - 2 , math.max( 0 , p2 ) )
	
	-- move the ball
	if gameState == 2 
	then
		ballX = ballX + ballVX
		ballY = ballY + ballVY
		
		-- collide with horizontal walls!
		if ballY > gameHeight - 4 or ballY < 4 then
			ballVY = -ballVY
		end
		
		-- check for collisions with players
		if ballX < 6 and ballVX < 0 then
			handleUserCollision()
		elseif ballX > gameWidth - 7 and ballVX > 0 then
			handleComputerCollision()
		end
		
		-- compute the KI
		if p2 + pLength/2 > ballY then
			p2 = p2 - 0.6
		elseif p2 + pLength/2 < ballY then
			p2 = p2 + 0.6
		end
		
	elseif gameState == 0 -- User has Ball
	then
		ballX = 7
		ballY = p1 + pLength/2
		
		p2 = p2 + ( ( gameHeight - pLength ) / 2 - p2 ) / 13
	else -- Computer has Ball
		ballX = gameWidth - 8
		ballY = p2 + pLength/2
		
		-- prepare Start
		if computerShootsIn <= 0
		then
			gameState = 2 -- play!
			ballVX = -ballVX
			ballVY = -ballVY
		else
			computerShootsIn = computerShootsIn - 1
			
			-- move the computers paddle
			if computerShootsIn < 70
			then
				p2 = p2 + ( computerDestination - p2 ) / 10
			end
		end
	end
	
end

-- Refreshes the whole bmp
function refresher( event )
	
	-- Receive Gadget
	local port = pongBmp.getBitmapPort( event )
	
	-- reset to black
	port.fill( "black" )
	
	-- Draw Paddles
	port.drawFilledRect( 1 , p1 + 1 , 3 , pLength , "lime" )
	port.drawFilledRect( gameWidth - 4 , p2 + 1 , 3 , pLength , "lime" )
	
	-- Draw middle Line and Ball
	port.drawVerticalDottedLine( gameWidth/2 , 0 , gameHeight , "lime" )
	port.drawFilledCircle( ballX , ballY , 2 , "fuchsia" )
	
	-- Paint Scores
	local font = System.getFont("ArialBlack13")
	port.drawString( gameWidth / 2 - 15 , 3 , font , scoreUser , "lime" , 10 )
	port.drawString( gameWidth / 2 + 10 , 3 , font , scoreComputer , "lime" , 10 )
	
	return "handled"
end

-- called on keyDown, keyUp , onBlur and onFocus
function eventHandler( event )

	if event.type == "onKeyDown" then
		if event.keyCode == 7 then -- Up
			keyUpPressed = true
		elseif event.keyCode == 14 then -- Down
			keyDownPressed = true
		elseif event.keyCode == 1 then -- A
			gameState = 2
		end
	elseif event.type == "onKeyUp" then
		if event.keyCode == 7 then -- Up
			keyUpPressed = false
		elseif event.keyCode == 14 then -- Down
			keyDownPressed = false
		end
	elseif event.type == "onBlur" then
		keyUpPressed = false
		keyDownPressed = false
		timer.stop();
	elseif event.type == "onFocus" then
		timer.start()
	end
	
	-- Calls the internal gadget-specific eventHandler
	-- which is necessary e.g. with onBlur, onFocus
	return "use_internal"
end