#include "_library/library.image.bmp.h"
#include "_type/type.h"

unsigned short* GenericBMPDecoder::decode( const unsigned char* data , unsigned int& width , unsigned int& height )
{
	BMPHeader0 *Bmpinfo0 = (BMPHeader0 *)(data);
	BMP_Headers *Bmpinfo = (BMP_Headers*)(data + 14);
	
	//			unsigned char *gfx = temp+54;
	unsigned int start = Bmpinfo0->ImageStart1 + ((Bmpinfo0->ImageStart2) << 16);
	const unsigned char* gfx = data + start;
	unsigned short* gfx2 = (unsigned short*)(data + start);
	
	//	unsigned short *gfx2 = (unsigned short*)(temp+54); // Pour le mode 16 bit...
	int r, g, b;
	int tempx, tempy;
	
	int lx = Bmpinfo->Width;
	int ly = Bmpinfo->Height;
	
	width = 0;
	height = 0;
	
	if( ly <= 0 || lx <= 0 )
		return nullptr;
	
	// write to destination
	width = (unsigned int)lx;
	height = (unsigned int)ly;
	
	// Allocate Space
	unsigned short* dest = new unsigned short[lx*ly];
	
	unsigned short Bits = Bmpinfo->BitsperPixel;
	
	//Buffer = (unsigned short*)(Buffer + ((x + (y*SWidth)) << 1)); // Position de départ
	
	unsigned int i = 0;
	
	//printf("Bits: %d\n", Bits);
	
	if( Bits > 16 ) // Pour 24 et 32 bits
	{
		for( tempy = ly - 1 ; tempy >= 0 ; tempy-- )
		{
			for( tempx = 0 ; tempx < lx ; tempx++ )
			{
				b = ( gfx[i++] >> 3 ) & 31;
				g = ( gfx[i++] >> 3 ) & 31;
				r = ( gfx[i++] >> 3 ) & 31;
				
				if( Bits == 32 ) i++; // On passe le bit alpha
				
				dest[ tempx + ( tempy * lx ) ] = RGB( r , g , b );
			}
			while( i & 3 ) i++; // Padding....
		}
	}
	else if( Bits == 16 )
	{
		for( tempy = ly - 1 ; tempy >= 0 ; tempy-- )
		{
			for( tempx = 0 ; tempx < lx ; tempx++ )
			{
				b = *gfx2 & 31;
				g = ( *gfx2 >> 5 ) & 31;
				r = ( *gfx2 >> 10 ) & 31;
				
				dest[ tempx + ( tempy * lx ) ] = RGB( r , g , b );
				
				gfx2++; // On passe au pixel suivant
			}
			int temp = (int) gfx2;
			
			while( temp & 3 ) temp++; // Padding....
			
			gfx2 = (unsigned short*) temp;
		}
	}
	else if ( Bits <= 8 )
	{
		// look for palette data if present //
		const unsigned char *colormap = data + 14 + Bmpinfo->SizeofHeader;
		unsigned short palette[256];
		unsigned int colors;
		
		if ( Bmpinfo->NColors != 0 )
			colors = Bmpinfo->NColors & 0xff;
		else 
			colors = 256;
		
		for ( i = 0 ; i != colors ; i++ )
		{
			b = (( colormap[ ( i << 2 ) + 0 ] ) >> 3 ) & 31;
			g = (( colormap[ ( i << 2 ) + 1 ] ) >> 3 ) & 31;
			r = (( colormap[ ( i << 2 ) + 2 ] ) >> 3 ) & 31;
			
			palette[i] = RGB( r , g , b );
		}
		
		i = 0;
		
		switch( Bits )
		{
			case 8:
			{
				for( tempy = ly - 1 ; tempy >= 0 ; tempy-- )
				{
					for(tempx = 0 ; tempx != lx ; tempx++ )
						dest[ tempx + ( tempy * lx ) ] = palette[ gfx[ i++ ] ];
				}
				break;
			}
			case 4:
			{
				for( tempy = ly - 1 ; tempy >= 0 ; tempy-- )
				{
					unsigned char shift = 4;
					for( tempx = 0 ; tempx != lx ; tempx++ )
					{
						dest[ tempx + ( tempy * lx ) ] = palette[ ( gfx[i] >> shift ) & 0xF ];
						if( !shift  )
						{
							i++;
							shift = 4;
						}
						else
							shift -= 4;
					}
					// Correct data alignment
					if( !shift  )
						i++;
				}
				break;
			}
			default:
				delete[] dest;
				width = 0;
				height = 0;
				return nullptr;
		} //switch
	}
	
	
	return dest;
}