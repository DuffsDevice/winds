#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "_type/type.h"

void memCopy( _pixelArray src , _pixelArray dest , _u32 size );

void memSet( _pixelArray dest , _s32 value , _u32 size );

#endif