#include <_resource/resource.font.system.7.h>
#include <_controller/controller.font.h>

FONT_System7::FONT_System7( unsigned char fixedWidth )
	: _virtualFont
(
	"System7"
	, {{
		/* Ranges */
		{ _2wchar( 33 , 254 ) , 0 }
	}}
	, {{
		/* Special Chars */
		{ 9654 , { 'A' , 1 } }
		, { 9655 , { 'A' , 1 } } // Triangle Right
		, { 9656 , { 'A' , 1 } }
		, { 9657 , { 'A' , 1 } }
		, { 9658 , { 'A' , 1 } }
		, { 9659 , { 'A' , 1 } }
		
		, { 9664 , { 'B' , 1 } } // Triangle Left
		, { 9665 , { 'B' , 1 } }
		, { 9666 , { 'B' , 1 } }
		, { 9667 , { 'B' , 1 } }
		, { 9668 , { 'B' , 1 } }
		, { 9669 , { 'B' , 1 } }
		
		, { 9650 , { 'C' , 1 } } // Triangle Up
		, { 9651 , { 'C' , 1 } }
		, { 9652 , { 'C' , 1 } }
		, { 9653 , { 'C' , 1 } }
		
		, { 9660 , { 'D' , 1 } } // Triangle Down
		, { 9661 , { 'D' , 1 } }
		, { 9662 , { 'D' , 1 } }
		, { 9663 , { 'D' , 1 } }
		
		, { 8701 , { 'E' , 1 } } // Arrow Left (Backspace)
		
		, { 8629 , { 'F' , 1 } } // Arrow down & left (Enter/Carriage Return)
		, { 0x21B2 , { 'F' , 1 } } // Arrow down & left (Enter/Carriage Return)
		
		, { 8681 , { 'G' , 1 } } // White Arrow down (Caps)
		, { 11015 , { 'H' , 1 } } // Black Arrow down (Caps hold)
		
		, { 0x21E7 , { 'I' , 1 } } // White Arrow up (Shift)
		, { 0x2B06 , { 'J' , 1 } } // Black Arrow up (Shift hold)
		
		, { 63743 , { 'K' , 1 } } // Windows Logo
		
		, { 8635 , { 'L' , 1 } } // Right turning Arrow
		, { 8634 , { 'M' , 1 } } // Left turning Arrow
		
		, { 9632 , { 'N' , 1 } } // Black Square (Stop)
		, { 9197 , { 'O' , 1 } } // Fast Forward End
		, { 9198 , { 'P' , 1 } } // Fast Backward End
		, { 9194 , { 'Q' , 1 } } // Fast Backward
		, { 9193 , { 'R' , 1 } } // Fast Forward
		, { 9208 , { 'S' , 1 } } // Pause
		
		, { 128257 , { 'T' , 1 } } // Replay
		
		, { 128264 , { 'U' , 1 } } // Speaker 0 bars
		, { 128265 , { 'V' , 1 } } // Speaker 1 bar
		, { 128266 , { 'W' , 1 } } // Speaker 2 bars
		
		, { 128267 , { 'X' , 1 } } // Battery Empty
		, { 128266 , { 'Y' , 1 } } // Battery Full
		, { 128268 , { 'Z' , 1 } } // Battery Plug
		
		, { 9398 , { 'a' , 1 } } // Button (A)
		, { 9399 , { 'b' , 1 } } // Button (B)
		, { 9421 , { 'c' , 1 } } // Button (X)
		, { 9422 , { 'd' , 1 } } // Button (Y)
		, { 9409 , { 'e' , 1 } } // Button [L]
		, { 9415 , { 'f' , 1 } } // Button [R]
		, { 8984 , { 'g' , 1 } } // Button [START]
		, { 8997 , { 'h' , 1 } } // Button [SELECT]
		
		, { 8997 , { 'h' , 1 } } // Button [SELECT]
		
		, { 8986 , { 'o' , 1 } } // Clock
		, { 9201 , { 'p' , 1 } } // Stop watch
		, { 8987 , { 'q' , 1 } } // Hour glass
		
		, { 9608 , { 'r' , 1 } } // Filled Rect
		, { 9618 , { 's' , 1 } } // Dark Shaded Rect
		, { 9617 , { 't' , 1 } } // Light Shaded Rect
		
		, { 0x2502 , { 'u' , 1 } } // Box drawing (single line)
		, { 0x2524 , { 'v' , 1 } }
		, { 0x251C , { 'w' , 1 } }
		, { 0x2514 , { 'x' , 1 } }
		, { 0x2518 , { 'y' , 1 } }
		, { 0x2534 , { 'z' , 1 } }
		, { 0x252C , { '0' , 1 } }
		, { 0x250C , { '1' , 1 } }
		, { 0x2510 , { '2' , 1 } }
		, { 0x2500 , { '3' , 1 } }
		, { 0x253C , { '4' , 1 } }
		
		, { 0x2551 , { '5' , 1 } } // Box drawing (double line)
		, { 0x2563 , { '6' , 1 } }
		, { 0x2560 , { '7' , 1 } }
		, { 0x255A , { '8' , 1 } }
		, { 0x255D , { '9' , 1 } }
		, { 0x2569 , { '!' , 1 } }
		, { 0x2566 , { '"' , 1 } }
		, { 0x2554 , { '#' , 1 } }
		, { 0x2557 , { '$' , 1 } }
		, { 0x2550 , { '%' , 1 } }
		, { 0x256C , { '&' , 1 } }
		
		, { 0x2211 , { '\'' , 1 } } // n-ary summation
		, { 0x0394 , { '(' , 1 } } // delta
		
		, { 0x1F537 , { ')' , 1 } } // wrench
		
		, { 0x2660 , { '*' , 1 } } // pik
		, { 0x2664 , { '*' , 1 } } // pik
		, { 0x2663 , { '+' , 1 } } // kreuz
		, { 0x2667 , { '+' , 1 } } // kreuz
		, { 0x2666 , { ',' , 1 } } // karo
		, { 0x2662 , { ',' , 1 } } // karo
		, { 0x2665 , { '-' , 1 } } // herz
		, { 0x2764 , { '-' , 1 } } // herz
		
		, { 8730 , { '.' , 1 } } // Square Root
		, { 0x222B , { '/' , 1 } } // Integral
		, { 0x221E , { ':' , 1 } } // Infinity
		
		, { 0x2913 , { ';' , 1 } } // Downwards arrow to bar
		, { 0x2194 , { '<' , 1 } } // Left Right Arrow
		, { 0x2195 , { '=' , 1 } } // Up down arrow
		
		, { 0x2713 , { '>' , 1 } } // tick
		, { 0x2714 , { '>' , 1 } } // tick
		, { 0x2717 , { '?' , 1 } } // cross mark
		, { 0x2718 , { '?' , 1 } } // cross mark
		
		, { 0x1F50D , { '@' , 1 } } // magnifier
		, { 0x1F50E , { '@' , 1 } } // magnifier
		
		, { 0x229E , { '[' , 1 } } // squared plus
		, { 0x229F , { '\\' , 1 } } // squared minus
		
		, { 0x25A1 , { ']' , 1 } } // square
		, { 0x25A1 , { ']' , 1 } } // square
		, { 0x25FB , { ']' , 1 } } // square
		, { 0x25CB , { '^' , 1 } } // circle
		, { 0x25EF , { '^' , 1 } } // circle
		, { 0x25CF , { '_' , 1 } } // filled circle
		, { 0x25C9 , { '_' , 1 } } // filled circle
		, { 0x25A0 , { '`' , 1 } } // filled square
		, { 0x25AE , { '`' , 1 } } // filled square
		, { 0x25B2 , { '{' , 1 } } // filled uppointing triangle
		, { 0x25B4 , { '{' , 1 } } // filled uppointing triangle
		, { 0x25B3 , { '|' , 1 } } // uppointing triangle
		, { 0x25B5 , { '|' , 1 } } // uppointing triangle
		
		, { 0xFF0B , { '}' , 1 } } // big plus sign
		, { 0x2795 , { '}' , 1 } } // big plus sign
		
		, { 0x25E6 , { '~' , 1 } } // small filled circle/bullet
		, { 0x2022 , { '~' , 1 } } // small filled circle/bullet
		
		, { 0x25AA , { L'¡' , 1 } } // small filled quare
		, { 0x25AB , { L'¡' , 1 } } // small filled quare
		
		, { 0x25A8 , { L'¢' , 1 } } // drag handle
		, { 0x25A4 , { L'€' , 1 } } // drag handle height only
		, { 0x25A5 , { L'£' , 1 } } // drag handle width only
		
		, { 0x1D453 , { L'¤' , 1 } } // mathematical italic small f
		
		, { 8595 , { L'¥' , 1 } } // arrow down
		, { 8593 , { L'¦' , 1 } } // arrow up
		, { 8594 , { L'§' , 1 } } // arrow right
		, { 8592 , { L'¨' , 1 } } // arrow left
		
		, { 0x2260 , { L'©' , 1 } } // not equal
		, { 0x2248 , { L'ª' , 1 } } // almost equal
		, { 0x2264 , { L'«' , 1 } } // lower equal
		, { 0x2265 , { L'¬' , 1 } } // greater equal
		
		, { 0x25EB , { L'®' , 1 } } // squared vertical minus
		
		, { 0x2030 , { L'¯' , 1 } } // per mille
		, { 0x2031 , { L'°' , 1 } } // per permyriad
		
		, { 0x2031 , { L'±' , 1 } } // n-ary product
		
		, { 0x270E , { L'²' , 1 } } // pencil
		, { 0x1F589 , { L'²' , 1 } } // pencil
		, { 0x2710 , { L'²' , 1 } } // pencil
		, { 0x270F , { L'²' , 1 } } // pencil
		
		, { 0x2203 , { L'³' , 1 } } // it exists
		, { 0x2200 , { L'´' , 1 } } // for all
		
		, { 0x2261 , { L'µ' , 1 } } // equivalent
		
		, { 0x2283 , { L'¶' , 1 } } // superset
		, { 0x2282 , { L'·' , 1 } } // subset
		, { 0x2287 , { L'¸' , 1 } } // superset or equal
		, { 0x2286 , { L'¹' , 1 } } // subset or equal
		
		, { 0x2228 , { L'º' , 1 } } // logical or
		, { 0x2227 , { L'»' , 1 } } // logical and
		
		, { 0x2204 , { L'¼' , 1 } } // there exists not
		, { 0x2208 , { L'½' , 1 } } // element of
		, { 0x220A , { L'½' , 1 } } // element of
		
		, { 0x1F502 , { L'¾' , 1 } } // repeat once
		, { 0x1F500 , { L'¿' , 1 } } // shuffle
		
		, { 0x2553 , { L'À' , 1 } } // Box Drawing I
		, { 0x2558 , { L'Á' , 1 } }
		, { 0x255C , { L'Â' , 1 } }
		, { 0x2555 , { L'Ã' , 1 } }
		, { 0x2565 , { L'Ä' , 1 } }
		, { 0x255E , { L'Å' , 1 } }
		, { 0x2568 , { L'Æ' , 1 } }
		, { 0x2561 , { L'Ç' , 1 } }
		, { 0x256B , { L'È' , 1 } }
		, { 0x256A , { L'É' , 1 } }
		, { 0x2564 , { L'Ê' , 1 } }
		, { 0x255F , { L'Ë' , 1 } }
		, { 0x2567 , { L'Ì' , 1 } }
		, { 0x2562 , { L'Í' , 1 } }
		, { 0x2556 , { L'Î' , 1 } }
		, { 0x2552 , { L'Ï' , 1 } }
		, { 0x2559 , { L'Ð' , 1 } }
		, { 0x255B , { L'Ñ' , 1 } }
	}}
	, {{
		_fontHandle( _fontController::getFont("SystemASCII7") )
		, _fontHandle( _fontController::getFont("SystemSymbols8") )
	}}
){}