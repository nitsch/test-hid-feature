#include <stdio.h>

#include "psmove.h"


int main( void )
{
	// get number of attached controllers
	int count = psmove_count_connected();
	if( count < 1 )
	{
		printf( "No controller connected.\n" );
		return 0;
	}

	printf( "Number of connected controllers: %d\n", count );

	// connect to the controller
	PSMove* move = psmove_connect();
	if( ! move )
	{
		printf( "Cannot connect to controller.\n" );
		return 0;
	}

	// print the connection type
	printf( "Connection type: " );
	switch( psmove_connection_type( move ) )
	{
		case Conn_Bluetooth:
			printf( "Bluetooth" );
			break;

		case Conn_USB:
			printf( "USB" );
			break;

		default:
			printf( "Unknown" );
	}
	printf( "\n" );

	psmove_disconnect( move );

	return 0;
}

