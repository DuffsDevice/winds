#ifndef _WIN_L_ICODECODER_
#define _WIN_L_ICODECODER_

class GenericIcoDecoder
{
	private:
		
		struct ICONDIRENTRY{
			unsigned char bWidth;			// Width of the image
			unsigned char bHeight;			// Height of the image (times 2)
			unsigned char bColorCount;		// Number of colors in image (0 if >=8bpp)
			unsigned char bReserved;		// Reserved
			unsigned short wPlanes;			// Color Planes
			unsigned short wBitCount;		// Bits per pixel
			unsigned long dwBytesInRes;		// how many bytes in this resource?
			unsigned long dwImageOffset;	// where in the file is this image
		};
		
		struct ICONDIR{
			unsigned short      idReserved;		// Reserved
			unsigned short      idType;			// resource type (1 for icons)
			unsigned short      idCount;		// how many images?
			//ICONDIRENTRY  	idEntries[1];	// the entries for each image
		};
		
		// size - 40 bytes
		struct BITMAPINFOHEADER{
			unsigned long biSize;
			unsigned long  biWidth;
			unsigned long  biHeight; // Icon Height (added height of XOR-Bitmap and AND-Bitmap)
			unsigned short  biPlanes;
			unsigned short  biBitCount;
			unsigned long biCompression;
			long biSizeImage;
			unsigned long  biXPelsPerMeter;
			unsigned long  biYPelsPerMeter;
			unsigned long biClrUsed;
			unsigned long biClrImportant;
		};

		// 46 bytes
		struct ICONIMAGE{
		   BITMAPINFOHEADER   		icHeader;		// DIB header
		   unsigned long			icColors[1];	// Color table (short 4 bytes) //RGBQUAD
		   unsigned char            icXOR[1];		// DIB bits for XOR mask
		   unsigned char            icAND[1];		// DIB bits for AND mask
		};

	public:
		
		static unsigned char* decode( const unsigned char* data , unsigned int& width , unsigned int& height , unsigned int page = 0 );
		
		static unsigned int getNumPages( const unsigned char* data );
};
	
#endif