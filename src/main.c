#include "psmove.h"

#include <stdio.h>
#include <string.h>


void dump_buffer( unsigned char const* buf, int size )
{
	int i;

	for( i = 0; i < size; i++ )
	{
		printf( "%02x ", buf[ i ] );
	}
	printf( "\n" );
}


// Feature Report 0xA0
// - expected to work via Bluetooth
// - expected to fail via USB
void test_send_auth( PSMove const* move )
{
	int res;
	unsigned char buf[ 34 + 1 ] =
	{
		0xA0, // Report ID
		0x01, 0x00, 0x1D, 0x91, 0xE5, 0x81, 0x30, 0x6A,
		0x22, 0x9A, 0xAB, 0x2E, 0x80, 0xB4, 0xED, 0x2E,
		0xDE, 0x40, 0x0A, 0xF0, 0x02, 0xB0, 0x42, 0x8B,
		0x01, 0x41, 0xB2, 0xA4, 0x3D, 0xE7, 0xD4, 0xBF,
		0x05, 0x92
	};

	if( ! move )
	{
		return;
	}

	printf( "Trying to send %zu bytes ... ", sizeof( buf ) );

	res = psmove_send_feature_report( move, buf, sizeof( buf ) );

	printf( "result %d\n", res );
}


// Feature Report 0xFA
// - expected to work via Bluetooth (sending only, without actually changing the LEDs)
// - expected to work via USB
void test_send_led( PSMove const* move, unsigned char r, unsigned char g, unsigned char b )
{
	int res;
	unsigned char buf[] =
	{
		0xFA, // Report ID
		r, 0, g, 0, b, 0
	};

	if( ! move )
	{
		return;
	}

	printf( "Trying to send %zu bytes ... ", sizeof( buf ) );

	res = psmove_send_feature_report( move, buf, sizeof( buf ) );

	printf( "result %d\n", res );
}


// Feature Report 0xA1
// - expected to work via Bluetooth
// - expected to fail via USB
void test_read_auth( PSMove const* move )
{
	int res;
	unsigned char buf[ 22 + 1 ];

	if( ! move )
	{
		return;
	}

	memset( buf, 0, sizeof( buf ) );

	// set Report ID
	buf[ 0 ] = 0xA1;

	printf( "Trying to read %zu bytes ... ", sizeof( buf ) );

	res = psmove_get_feature_report( move, buf, sizeof( buf ) );

	printf( "result %d\n", res );

	if( res > 0 )
	{
		dump_buffer( buf, res );
	}
}


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

	// send and receive some different Feature Reports
	test_send_auth( move );
	test_send_led( move, 255, 255, 0 );
	test_read_auth( move );

	psmove_disconnect( move );

	return 0;
}

