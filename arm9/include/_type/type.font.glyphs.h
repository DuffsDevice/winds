#ifndef _WIN_T_FONTGLYPHS_
#define _WIN_T_FONTGLYPHS_

#include "_type/type.h"

namespace _glyph
{
	const _char backspace = 'E',
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
	batterycharging = 'Z',
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
	integral = '/',
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
	triangle = '|',
	aim = '}',
	smallCircleFilled = '~',
	smallSquareFilled = '�',
	resizeHandleXY = '�',
	resizeHandleY = '�',
	resizeHandleX = '�',
	func = '�',
	arrDown = '�',
	arrUp = '�',
	arrRight = '�',
	arrLeft = '�',
	approximately = '�',
	lessThanEqual = '�',
	greaterThanEqual = '�',
	trademark = '�',
	dot = '�',
	divide = '�';
};

namespace tiles
{
	const _char filled1 = 'r',
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