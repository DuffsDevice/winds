#ifndef _WIN_L_BMPDECODER_
#define _WIN_L_BMPDECODER_

#include "_type/type.h"

class GenericBMPDecoder
{
	private:
	
		struct BMPHeader0{
			unsigned short Id; // ?
			unsigned int Length;
			unsigned short Nothing1, Nothing2; // ?
			unsigned short ImageStart1, ImageStart2; // Offset of start of image, start at position 0x0A, which can only be 2-byte aligined
		}PACKED;
		
		struct BMP_Headers{
			unsigned int SizeofHeader; // 40
			unsigned int Width, Height;
			unsigned short Colorplanes; // Usually 1
			unsigned short BitsperPixel; //1, 2, 4, 8, 16, 24, 32
			unsigned int Compression;  // 0 for none, 1...
			unsigned int SizeofData; // Not reliable
			unsigned int WidthperMeter, HeightperMeter; // Don't care
			unsigned int NColors, ImportantColors; // Number of colors used, important colors ?
		}PACKED;
	
	public:
		
		static unsigned short* decode( const unsigned char* data , const unsigned int size , unsigned int& width , unsigned int& height );
};

#endif