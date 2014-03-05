#include "_library/library.image.bmp.decoder.h"
#include "_type/type.h"
#include "_type/type.color.h"

unsigned short* GenericBMPDecoder::decode( const unsigned char* data , const unsigned int size , unsigned int& width , unsigned int& height )
{
	if( size < sizeof(BMP_Headers) + 14 )
		return nullptr;
	
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
	
	unsigned int i = 0;
	
	if( Bits > 16 ) // Pour 24 et 32 bits
	{
		if( int(size - start) < ly * lx * 3 )
			return nullptr;
		
		for( tempy = ly - 1 ; tempy >= 0 ; tempy-- )
		{
			for( tempx = 0 ; tempx < lx ; tempx++ )
			{
				b = gfx[i++];
				g = gfx[i++];
				r = gfx[i++];
				
				if( Bits == 32 ) i++; // On passe le bit alpha
				
				dest[ tempx + ( tempy * lx ) ] = _color::fromRGB8( r , g , b );
			}
			while( i & 3 ) i++; // Padding....
		}
	}
	else if( Bits == 16 )
	{
		if( int(size - start) < ly * lx * 2 )
			return nullptr;
		
		for( tempy = ly - 1 ; tempy >= 0 ; tempy-- )
		{
			for( tempx = 0 ; tempx < lx ; tempx++ )
			{
				b = *gfx2 & 31;
				g = ( *gfx2 >> 5 ) & 31;
				r = ( *gfx2 >> 10 ) & 31;
				
				dest[ tempx + ( tempy * lx ) ] = _color::fromRGB( r , g , b );
				
				gfx2++; // On passe au pixel suivant
			}
			int temp = (int) gfx2;
			
			while( temp & 3 ) temp++; // Padding....
			
			gfx2 = (unsigned short*) temp;
		}
	}
	else
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
			
			palette[i] = _color::fromRGB( r , g , b );
		}
		
		i = 0;
		
		switch( Bits )
		{
			case 8:
			{
				if( int(size - start) < ly * lx )
					return nullptr;
				
				for( tempy = ly - 1 ; tempy >= 0 ; tempy-- )
				{
					for(tempx = 0 ; tempx != lx ; tempx++ )
						dest[ tempx + ( tempy * lx ) ] = palette[ gfx[ i++ ] ];
				}
				break;
			}
			case 4:
			{
				if( int( size - start ) * 2 < ly * lx )
					return nullptr;
				
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
			case 2:
			{
				if( int( size - start ) * 8 < ly * lx )
					return nullptr;
				
				unsigned char mask = 1 << 7;
				
				for( tempy = ly - 1 ; tempy >= 0 ; tempy-- )
				{
					for(tempx = 0 ; tempx != lx ; tempx++ )
					{
						if( !mask ){
							mask = 1 << 7;
							i++;
						}
						dest[ tempx + ( tempy * lx ) ] = palette[ gfx[i] & mask ];
						mask >>= 1;
					}
				}
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