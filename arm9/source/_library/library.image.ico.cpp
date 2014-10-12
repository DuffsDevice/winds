/*
 *	
 *	code by Victor Laskin (victor.laskin@gmail.com)
 *  
 *
 *	
 *	THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS
 *	OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS forA PARTICULAR PURPOSE
 *	ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE forANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *	GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 *	IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 *	ifADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <_library/library.image.ico.h>

unsigned int GenericIcoDecoder::getNumPages( const unsigned char* buffer ){
	ICONDIR* icoDir = (ICONDIR*)buffer;
	return icoDir->idCount;
}


unsigned char* GenericIcoDecoder::decode( const unsigned char* buffer,	///< input buffer data
						unsigned int& width, 							///< output - width
						unsigned int& height, 							///< output - height
						unsigned int pageNumber								///< Page to read
						)
{
	ICONDIR* icoDir = (ICONDIR*)buffer;

	//LOG << "ICO:" <<  icoDir->idReserved << " "<<  icoDir->idType << " "<<  icoDir->idCount << NL;
	unsigned int iconsCount = icoDir->idCount;

	if(icoDir->idReserved != 0) return nullptr;
	if(icoDir->idType != 1) return nullptr;
	if(iconsCount == 0) return nullptr;
	if(iconsCount > 20) return nullptr;

	const unsigned char* cursor = buffer;
	cursor += 6;
	ICONDIRENTRY* dirEntry = (ICONDIRENTRY*)(cursor);
	
	if( pageNumber >= iconsCount )
		pageNumber = iconsCount - 1;
	
	int offset = 0;
	for(unsigned int i = 0; i < iconsCount; i++)
	{
		unsigned int w = dirEntry->bWidth;
		unsigned int h = dirEntry->bHeight;
		
		if( i == pageNumber ){
			width = w;
			height = h;
			offset = dirEntry->dwImageOffset;
		}
		
		dirEntry++;
	}

	if(offset == 0) return nullptr;

	//LOG << "Offset: " << offset << NL;
	cursor = buffer;
	cursor += offset;


	ICONIMAGE* icon = (ICONIMAGE*)(cursor);
	//LOG << "BitmapInfo: Struct: " << (int)(icon->icHeader.biSize) << " Size: " << (int)(icon->icHeader.biWidth) << "x" << (int)(icon->icHeader.biHeight) << " Bytes: " << (int)(icon->icHeader.biSizeImage) << NL;
	//LOG << "BitmapInfo: Planes: " << (int)icon->icHeader.biPlanes << " bits: " << (int)icon->icHeader.biBitCount << " Compression: " << (int)icon->icHeader.biCompression << NL;
	unsigned int realBitsCount = (unsigned int)icon->icHeader.biBitCount;
	bool hasAndMask = (height != icon->icHeader.biHeight);

	cursor += 40;
	int numBytes = width * height * 4;
	
	unsigned char* image = new unsigned char[numBytes];

	
	switch( realBitsCount )
	{
		case 32: // rgba + vertical swap
		{
			int shift;
			int shift2;
			for(unsigned int x = 0; x < width; x++)
				for(unsigned int y = 0; y < height; y++)
				{
					shift = 4 * (x + y * width);
					shift2 = 4 * (x + (height - y - 1) * width);
					image[shift] = cursor[shift2 +2];
					image[shift+1] = cursor[shift2 +1];
					image[shift+2] = cursor[shift2 ];
					image[shift+3] = cursor[shift2 +3];
				}
			break;
		}
		case 24:
		{
			int shift;
			int shift2;
			for(unsigned int x = 0; x < width; x++)
				for(unsigned int y = 0; y < height; y++)
				{
					shift = 4 * (x + y * width);
					shift2 = 3 * (x + (height - y - 1) * width);
					image[shift] = cursor[shift2 +2];
					image[shift+1] = cursor[shift2 +1];
					image[shift+2] = cursor[shift2 ];
					image[shift+3] = 255;
				}
			break;
		}
		case 8: // 256 colors
		{
			// 256 color table
			unsigned char * colors = (unsigned char *)cursor;
			cursor += 256 * 4;
			int shift;
			int shift2;
			int index;
			for(unsigned int x = 0; x < width; x++)
				for(unsigned int y = 0; y < height; y++)
				{
					shift = 4 * (x + y * width);
					shift2 = (x + (height - y - 1) * width);
					index = 4 * cursor[shift2];
					image[shift] = colors[index + 2];
					image[shift+1] = colors[index + 1];
					image[shift+2] = colors[index ];
					image[shift+3] = 255;
				}
			break;
		}
		case 4: // 16 colors
		{
			// 16 color table
			unsigned char * colors = (unsigned char *)cursor;
			cursor += 16 * 4;
			int shift;
			int shift2;
			unsigned char index;
			for(unsigned int x = 0; x < width; x++)
				for(unsigned int y = 0; y < height; y++)
				{
					shift = 4 * (x + y * width);
					shift2 = (x + (height - y - 1) * width);
					index = cursor[shift2 / 2];
					if(shift2 % 2 == 0)
						index = (index >> 4) & 0xF;
					else
						index = index & 0xF;
					index *= 4;
					
					image[shift] = colors[index + 2];
					image[shift+1] = colors[index + 1];
					image[shift+2] = colors[index ];
					image[shift+3] = 255;
				}
			break;
		}
	}


	// Read AND mask after base color data - 1 BIT MASK
	if(hasAndMask)
	{
		int shift;
		int shift2;
		unsigned char bit;
		int mask;
		cursor += realBitsCount * width * height / 8;
		unsigned int boundary = 32; //!!! 32 bit boundary (http://www.daubnet.com/en/file-format-ico)
		if(width > boundary) boundary = width;

		for(unsigned int y = 0; y < height; y++)
			for(unsigned int x = 0; x < width; x++)
			{
				shift = 4 * (x + y * width) + 3;
				bit = 7 - (x % 8);
				shift2 = (x + (height - y - 1) * boundary) / 8;
				mask = (0x01 & ((unsigned char)cursor[shift2] >> bit));
				//LOG << "Bit: " << bit << "Value: " << mask << " from byte: " << cursor[shift2] << " row: " << y << " index:" << shift2 << NL;
				image[shift] *= 1 - mask;
			}
	}

	return image;
}