#ifndef _WIN_T_SOUND_NOTE_
#define _WIN_T_SOUND_NOTE_

#include "_type/type.h"

struct _note
{
	// Frequencies of notes around C4 multiplied by 8
	enum : _u16
	{
		c =		2093,
		cis =	2217,
		des =	cis,
		d =		2349,
		dis =	2489,
		es =	dis,
		e =		2637,
		f =		2794,
		fis =	2960,
		ges =	fis,
		g =		3136,
		gis =	3322,
		as =	gis,
		a =		3520,
		ais =	3729,
		b =		ais,
		h =		3951
	} value;
	
	_note( _u16 freq ) :
		value( (decltype(value))freq )
	{}
	operator _u16(){ return this->value; }
	
	// Get a
	static _note transpose( _note base , _u8 octave ){
		return _note( _u16(base) << (octave-4) );
	}
};

#endif