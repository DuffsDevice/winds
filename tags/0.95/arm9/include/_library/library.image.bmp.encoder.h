/*************************************************
*                                                *
*  EasyBMP Cross-Platform Windows Bitmap Library * 
*                                                *
*  Author: Paul Macklin                          *
*   email: macklin01@users.sourceforge.net       *
* support: http://easybmp.sourceforge.net        *
*                                                *
*          file: EasyBMP.h                       *
*    date added: 05-02-2005                      *
* date modified: 12-01-2006                      *
*       version: 1.06                            *
*                                                *
*   License: BSD (revised/modified)              *
* Copyright: 2005-6 by the EasyBMP Project       * 
*                                                *
* description: Defines basic data structures for *
*              the BMP class                     *
*                                                *
*************************************************/

// set to a default of 96 dpi
#define DefaultXPelsPerMeter 3780
#define DefaultYPelsPerMeter 3780

#include <stdio.h>
#include "_type/type.predefines.h"

class GenericBMPEncoder
{
	private:

		static inline _u16 FlipWORD( _u16 in )
		{ return ( (in >> 8) | (in << 8) ); }
		
		static inline _u32 FlipDWORD( _u32 in )
		{
			return ( ((in&0xFF000000)>>24) | ((in&0x000000FF)<<24) | 
			((in&0x00FF0000)>>8 ) | ((in&0x0000FF00)<<8 )   );
		}
		
		struct BMPFILEHEADER
		{
			_u16	type;
			_u32	size;
			_u16	reserved1;
			_u16	reserved2;
			_u32	offsetBytes; 
			
			void SwitchEndianess(){
				type = FlipWORD( type );
				size = FlipDWORD( size );
				reserved1 = FlipWORD( reserved1 );
				reserved2 = FlipWORD( reserved2 );
				offsetBytes = FlipDWORD( offsetBytes );
			}
		};

		struct BMPINFOHEADER
		{
			_u32 size;
			_u32 width;
			_u32 height;
			_u16 planes;
			_u16 bitCount;
			_u32 compression;
			_u32 sizeImage;
			_u32 xPixelsPerMeter;
			_u32 yPixelsPerMeter;
			_u32 colorUsed;
			_u32 colorImportant;
			
			void SwitchEndianess(){
				size = FlipDWORD( size );
				width = FlipDWORD( width );
				height = FlipDWORD( height );
				planes = FlipWORD( planes );
				bitCount = FlipWORD( bitCount );
				compression = FlipDWORD( compression );
				sizeImage = FlipDWORD( sizeImage );
				xPixelsPerMeter = FlipDWORD( xPixelsPerMeter );
				yPixelsPerMeter = FlipDWORD( yPixelsPerMeter );
				colorUsed = FlipDWORD( colorUsed );
				colorImportant = FlipDWORD( colorImportant );
			}
		};
	
	public:

		static bool encode( _literal fileName , _int width , _int height , _pixelArray data );
};