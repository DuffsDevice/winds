-- Hello World Program...

local win;
local btn;

function main()

   -- generate our window
   win = _window( 120 , 60 , 5 , 5 , "Hello World Program!" )
   -- You can also use _window:new

   -- generate our button
   btn = _button( 20 , 20 , "Button1" )

   -- Add it to our window
   win:addChild( btn );

   -- add our window to Windows ( we want it to be displayed! )
   registerToWindows( win );

end