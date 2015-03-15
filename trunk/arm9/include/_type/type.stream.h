#ifndef _WIN_T_STREAM_OUT_
#define _WIN_T_STREAM_OUT_

#include <_type/type.h>

class _outStream
{
	public:
		
		// Methods to output data to the outstream
		virtual void puts( wstring str ){
			for( _wchar ch : str )
				putc( ch );
		}
		virtual void putc( _wchar str ) = 0;
		
		// Virtual Dtor
		virtual ~_outStream();
};

class _inStream
{
	public:
		
		// Receive input from the instream
		virtual string getc() = 0;
		virtual string gets( _u32 length ) = 0;
		
		// Virtual Dtor
		virtual ~_inStream();
};

#endif