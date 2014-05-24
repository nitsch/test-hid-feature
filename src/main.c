#include <stdio.h>

#include "psmove.h"


int main( void )
{
	int count = psmove_count_connected();
	if( count < 1 )
	{
		printf( "No controller connected.\n" );
		return 0;
	}

	printf( "Number of connected controllers: %d\n", count );

	return 0;
}

