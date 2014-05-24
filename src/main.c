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

	PSMove* move = psmove_connect();
	if( ! move )
	{
		printf( "Cannot connect to controller.\n" );
		return 0;
	}

	psmove_disconnect( move );

	return 0;
}

