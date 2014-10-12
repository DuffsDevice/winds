#include <_library/library.image.bmp.encoder.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

bool GenericBMPEncoder::encode( _literal fileName , _int width , _int height , _pixelArray data )
{
	FILE* fp = fopen( fileName, "wb" );
	if( fp == NULL )
	{
		fclose( fp );
		return false;
	}

	// some preliminaries
	_u8 bytesPerPixel = 2;
	_u32 bytesPerRow = bytesPerPixel * width;
	_u8 bytePaddingPerRow = ((4-bytesPerRow)%4)%4;

	_u32 actualBytesPerRow = bytesPerRow + bytePaddingPerRow;

	_u32 totalPixelBytes = height * actualBytesPerRow;

	// leave some room for 16-bit masks 
	_u8 paletteSize = 3*4;
	
	// write the file header
	BMPFILEHEADER bmfh;
	bmfh.type = 19778;
	bmfh.size = 14 + 40 + paletteSize + totalPixelBytes; 
	bmfh.reserved1 = 0; 
	bmfh.reserved2 = 0; 
	bmfh.offsetBytes = (_u32) (14+40+paletteSize);
	bmfh.SwitchEndianess();

	// write the info header
	BMPINFOHEADER bmih;
	bmih.size = 40;
	bmih.width = width;
	bmih.height = height;
	bmih.planes = 1;
	bmih.bitCount = 16;
	bmih.compression = 3; // indicates that we'll be using bit fields for 16-bit files
	bmih.sizeImage = (_u32) totalPixelBytes;
	bmih.xPixelsPerMeter = DefaultXPelsPerMeter;
	bmih.yPixelsPerMeter = DefaultYPelsPerMeter;
	bmih.colorUsed = 0;
	bmih.colorImportant = 0;
	bmih.SwitchEndianess();

	fwrite( (_byte*) &(bmfh) , sizeof(BMPFILEHEADER) , 1 , fp );
	fwrite( (_byte*) &(bmih) , sizeof(BMPINFOHEADER) , 1 , fp );

	// write the pixels 
	_int i,j;
	
	// write the bit masks
	_u16 BlueMask = 31;    // bits 11-15
	_u16 GreenMask = 991; // bits 6-10
	_u16 RedMask = 31744;  // bits 1-5
	_u16 ZeroWORD;
	
	RedMask = FlipWORD( RedMask );
	fwrite( (_byte*) &RedMask , 2 , 1 , fp );
	fwrite( (_byte*) &ZeroWORD , 2 , 1 , fp );
	
	GreenMask = FlipWORD( GreenMask );
	fwrite( (_byte*) &GreenMask , 2 , 1 , fp );
	fwrite( (_byte*) &ZeroWORD , 2 , 1 , fp );
	
	BlueMask = FlipWORD( BlueMask );
	fwrite( (_byte*) &BlueMask , 2 , 1 , fp );
	fwrite( (_byte*) &ZeroWORD , 2 , 1 , fp );
	
	_int dataBytes = width*2;
	_int paddingBytes = ( 4 - dataBytes % 4 ) % 4;
	
	// write the actual pixels
	for( j=height-1 ; j >= 0 ; j-- )
	{
		// write all row pixel data
		i=0;
		_int writeNumber = 0;
		while( writeNumber < dataBytes )
		{
			_u32 Color = data[ j * width + i ];
			
			Color = FlipWORD( Color );
			
			fwrite( (_byte*) &Color , 2, 1, fp);
			writeNumber += 2;
			i++;
		}
		// write any necessary row padding
		writeNumber = 0;
		while( writeNumber < paddingBytes )
		{
			_u8 TempBYTE;
			fwrite( (_byte*) &TempBYTE , 1, 1, fp);
			writeNumber++;
		}
	}

	fclose(fp);
	return true;
}