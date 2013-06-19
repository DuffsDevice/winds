system.require("_bitmap")
system.require("_area")
system.require("_bitmapPort")
system.require("_event")
system.require("_color")
system.require("_window")
system.require("_imagegadget")
system.require("_font")

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
local ballX = 10
local ballY = 10
local ballVX = 1
local ballVY = 1
local pLength = 15 -- width of the paddle
local gameHeight = 60
local gameWidth = 100


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

-- starts the global Timer calling frame() every 1/60s
function startTimer()
	system.executeTimer( frame , 1000/60 , true )
end

-- Main entry point
function main()
	
	-- reset player coordinates
	p1 = ( gameHeight - pLength ) / 2
	p2 = p1
	
	-- create the image
	pongBmp = _imagegadget( 0 , 0 , _bitmap( gameWidth , gameHeight ) )
	pongBmp.setUserEventHandler( "refresh" , refresher )
	
	window = _window( gameWidth + 2 , gameHeight + 11 , 10 , 10 , "Pong" , "notResizeable | minimizeable" )
	
	window.addChild( pongBmp )
	window.setUserEventHandler( "keyDown" , eventHandler )
	window.setUserEventHandler( "keyUp" , eventHandler )
	window.setUserEventHandler( "onBlur" , eventHandler )
	window.setUserEventHandler( "onFocus" , eventHandler )
	
	system.addChild( window )
	
	window.focus()
	
end

function frame()
	move()
	pongBmp.bubbleRefresh( true )
end

function move()
	
	if keyUpPressed then
		p1 = p1 - 0.3
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
	local port = pongBmp.getBitmapPort();
	
	if event.hasClippingRects() then
		local damagedRects = event.damagedRects
		damagedRects.toRelative( pongBmp.getAbsolutePosition() )
		
		port.addClippingRects( damagedRects )
	else
		port.normalizeClippingRects()
	end
	
	-- reset to black
	port.fill( "black" )
	
	-- Draw Paddles
	port.drawFilledRect( 1 , p1 + 1 , 3 , pLength , system.rgb( 0 , 31 , 0 ) )
	port.drawFilledRect( gameWidth - 4 , p2 + 1 , 3 , pLength , system.rgb( 0 , 31 , 0 ) )
	
	-- Draw middle Line and Ball
	port.drawVerticalDottedLine( gameWidth/2 , 0 , gameHeight , system.rgb( 0 , 31 , 0 ) )
	port.drawFilledCircle( ballX , ballY , 2 , system.rgb( 31 , 0 , 31 ) )
	
	-- Paint Scores
	local font = system.getFont("ArialBlack13")
	port.drawString( gameWidth / 2 - 15 , 3 , font , scoreUser , system.rgb( 0 , 31 , 0 ) )
	port.drawString( gameWidth / 2 + 10 , 3 , font , scoreComputer , system.rgb( 0 , 31 , 0 ) )
	
	return "handled"
end

-- called on keyDown, keyUp , onBlur and onFocus
function eventHandler( event )

	if event.type == "keyDown" then
		if event.keyCode == 7 then -- Up
			keyUpPressed = true
		elseif event.keyCode == 14 then -- Down
			keyDownPressed = true
		elseif event.keyCode == 1 then -- A
			gameState = 2
		end
	elseif event.type == "keyUp" then
		if event.keyCode == 7 then -- Up
			keyUpPressed = false
		elseif event.keyCode == 14 then -- Down
			keyDownPressed = false
		end
	elseif event.type == "onBlur" then
		keyUpPressed = false
		keyDownPressed = false
		system.terminateTimer( frame )
	elseif event.type == "onFocus" then
		startTimer()
	end
	
	-- Calls the internal gadget-specific eventHandler
	-- which is neccesary e.g. with onBlur, onFocus
	return "use_internal"
end