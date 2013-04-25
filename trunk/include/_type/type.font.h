#ifndef _WIN_FONT_
#define _WIN_FONT_

#include "_type/type.h"
#include "_type/type.rect.h"

class _bitmap;

class _font{

	private:
		
		string	 	name; //! Name
		
	protected:
		
		//! Set Font-Name
		void setName( string newName );
		
	public:
		
		//! Constructor with name
		_font( string newName );
		
		virtual ~_font(){}
		
		//! Get the CharacterWidth related to a specific character
		virtual _u16 getCharacterWidth( _char character , _u8 fontSize = 0 ) const = 0;
		
		//! Get the With of a string
		_u16 getStringWidth( string str , _u8 fontSize = 0 ) const ;
		
		//! Check whether this font is monospace
		virtual _u16 isMonospace() const = 0;
		
		//! Check if the supplied character can be displayed by the font
		virtual bool isCharSupported( _char ch ) const  = 0;
		
		//! Get the Height of the font
		virtual _u16 getHeight( _u8 fontSize = 0 ) const = 0;
		
		//! Get the Main-Height of the font, which is :
		/**
		ASC ->  	||  ||			<- H
		  - 		|||||| 	 /``\	   E
		ENT ->______||  ||___\,,|___   I
								|      GH
							 \,,/	<- T
		*/
		virtual _u16 getAscent( _u8 fontSize = 0 ) const = 0;
		
		//! Check if this font is valid
		virtual bool valid() const = 0;
		
		//! Get the Name of the Font e.g. 'Arial'
		string getName() const ;
		
		//! draw a Character!
		virtual _u16 drawCharacter( _bitmap* dest , _coord x , _coord y , _char character , _pixel color , _rect clip , _u8 fontSize = 0 ) const = 0;
		
		
		//! Receive a font, created from file
		static const _font* fromFile( string path );
};

#include "_type/type.bitmap.h"

namespace glyph
{
	const char backspace = 'E',
	arrowLeft = 'B',
	arrowRight = 'A',
	arrowUp = 'C',
	arrowDown = 'D',
	shift = 'I',
	shiftPressed = 'J',
	caps = 'G',
	capsPressed = 'H',
	windows = 'K',
	carriageReturn = 'F',
	arrowRotateRight = 'L',
	arrowRotateLeft = 'M',
	stop = 'N',
	fastForward = 'O',
	fastBackward = 'P',
	forward = 'R',
	backward = 'Q',
	pause = 'S',
	circuit = 'T',
	volume1 = 'U',
	volume2 = 'V',
	volume3 = 'W',
	batteryEmpty = 'X',
	batteryFull = 'Y',
	batteryCharging = 'Z',
	buttonA = 'a',
	buttonB = 'b',
	buttonX = 'c',
	buttonY = 'd',
	buttonL = 'e',
	buttonR = 'f',
	buttonStart = 'g',
	buttonSelect = 'h',
	dPadLeft = 'i',
	dPadUp = 'j',
	dPadRight = 'k',
	dPadDown = 'l',
	brighter = 'm',
	darker = 'n',
	clock = 'o',
	timer = 'p',
	hourglass = 'q',
	sum = '\'',
	delta = '(',
	settings = ')',
	heart = '-',
	diam = ',',
	club = '+',
	spade = '*',
	root = '.',
	func = '/',
	infinity = ':',
	reset = ';',
	arrowLeftRight = '<',
	arrowUpDown = '=',
	tick = '>',
	cross = '?',
	magnifier = '@',
	enlarge = '\\',
	reduce = '[',
	square = ']',
	circle = '^',
	circleFilled = '_',
	squareFilled = '`',
	triangleFilled = '{',
	triangle = '|';
};

namespace tiles
{
	const char filled1 = 'r',
	filled2 = 's',
	filled3 = 't',
	
	// Single Lined
	upDown = 'u',
	upDownLeft = 'v',
	upDownRight = 'w',
	upRight = 'x',
	upLeft = 'y',
	upLeftRight = 'z',
	downLeftRight = '0',
	downRight = '1',
	downLeft = '2',
	leftRight = '3',
	upDownLeftRight = '4',
	
	// Double Lined
	dblUpDown = '5',
	dblUpDownLeft = '6',
	dblUpDownRight = '7',
	dblUpRight = '8',
	dblUpLeft = '9',
	dblUpLeftRight = '!',
	dblDownLeftRight = '"',
	dblDownRight = '#',
	dblDownLeft = '$',
	dblLeftRight = '%',
	dblUpDownLeftRight = '&';
}

#endif