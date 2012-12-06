#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "_type/type.h"

unsigned int getFreeMemory();

int string2int(const char *p);
string int2string( _int val );

/*//! Tonc-lib
void memset32(void *dst, unsigned int src, unsigned int wdn);
void memset16(void *dst, unsigned short int val, unsigned int hwn);

//! Tonc-lib
void memcpy16(void *dst, const void *src, unsigned int hwn);
void memcpy32(void *dst, const void *src, unsigned int hwn);*/

void memSet( void* dst , const unsigned short int val , unsigned int hwn );
void memCpy( void* dst , const void* src , unsigned int hwn );

#endif