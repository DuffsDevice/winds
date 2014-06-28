#ifndef _WIN_F_INPUT_EMULATOR_
#define _WIN_F_INPUT_EMULATOR_

#include "_type/type.h"
#include "_type/type.event.h"
#include "_type/type.gadget.h"
#include "_type/type.direction.h"

void emulateMouseClick( _gadget* emulatedOn , _direction clickPosition = _direction::center )
{
	if( !emulatedOn )
		return;
	
	_coord xPos = 0;
	_coord yPos = 0;
	_rect gadgetDims = emulatedOn->getDimensions();
	
	if( getHorizontalPart( clickPosition ) == _direction::right )
		xPos = gadgetDims.getX2();
	else if( getHorizontalPart( clickPosition ) == _direction::center )
		xPos = gadgetDims.getX2() >> 1;
	
	if( getVerticalPart( clickPosition ) == _direction::down )
		yPos = gadgetDims.getY2();
	else if( getVerticalPart( clickPosition ) == _direction::middle )
		yPos = gadgetDims.getY2() >> 1;
	
	_event event = _event().setPosX( xPos ).setPosY( yPos );
	
	emulatedOn->triggerEvent( event.setType( onMouseDown ) );
	emulatedOn->triggerEvent( event.setType( onMouseUp ) );
	emulatedOn->triggerEvent( event.setType( onMouseClick ) );
}

#endif