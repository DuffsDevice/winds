/*---------------------------------------------------------------------------------

	Basic template code for starting a DS app

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
using namespace std;

#include "parser.h"



//---------------------------------------------------------------------------------
int main(void) {
//---------------------------------------------------------------------------------
	consoleDemoInit();
	
	cpuStartTiming(true);
	const char text[] = "Himmelfahrtsgetue ist raffitückisch";
	const char* t = text;
	std::list<int> p = SyllableParser::parseText( t );
	bool* booltable = new bool[500];
	memset( booltable , 0 , 500 );
	for( int i : p )
		booltable[i] = true;
	int i = 0;
	for( const char* c = t ; c < t + strlen( t ) ; c++ )
	{
		printf("%c",*c);
		if( booltable[i++] )
			printf("-");
	}
	printf("---- %d\n",cpuGetTiming());
	while(1)
		swiWaitForVBlank();

}
