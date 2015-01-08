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
	char text[] = "Himmelfahrtsgetue ist raffitückisch";
	char* t = text;
	
	TinyHyphenator parser = { TinyHyphenatorLanguages::German() };
	
	std::list<int> p = parser.parseText( t );
	bool* booltable = new bool[500];
	memset( booltable , 0 , 500 );
	for( int i : p )
		booltable[i] = true;
	int i = 0;
	for( char* c = t ; c < t + strlen( t ) ; c++ )
	{
		printf("%c",*c);
		if( booltable[i++] )
			printf("-");
	}
	printf("---- %d\n",cpuGetTiming());
	while(1)
		swiWaitForVBlank();

}