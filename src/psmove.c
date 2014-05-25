#include "psmove.h"

#include "hidapi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>


// USB Vendor ID and Product ID of PS Move Motion Controller
#define PSMOVE_VID 0x054C
#define PSMOVE_PID 0x03D5

// debug output macro
#ifdef DEBUG
#	define psmove_DEBUG( msg, ... ) fprintf( stderr, "[DEBUG] " msg, ## __VA_ARGS__ )
#else
#	define psmove_DEBUG( msg, ... )
#endif




struct _PSMove
{
	hid_device* hid_handle;
	char* serial_number;
};


PSMove* psmove_connect_internal( wchar_t const* serial, char const* path );


int psmove_count_connected()
{
	struct hid_device_info* dev;
	struct hid_device_info* cur_dev;
	int count = 0;

	// count number of attached HID devices that match the given VID and PID
	dev = hid_enumerate( PSMOVE_VID, PSMOVE_PID );
	for( cur_dev = dev; cur_dev != NULL; cur_dev = cur_dev->next )
	{
		count++;
	}

	hid_free_enumeration( dev );

	return count;
}


PSMove* psmove_connect()
{
	// we need at least 1 attached controller
	if( psmove_count_connected() < 1 )
	{
		psmove_DEBUG( "no controller attached\n" );
		return NULL;
	}

	struct hid_device_info* dev;
	PSMove* move = NULL;

	// connect to the first attached HID device that matches the given VID and PID
	dev = hid_enumerate( PSMOVE_VID, PSMOVE_PID );
	if( dev )
	{
		move = psmove_connect_internal( dev->serial_number, dev->path );
	}

	hid_free_enumeration( dev );

	return move;
}


PSMove* psmove_connect_internal( wchar_t const* serial, char const* path )
{
	PSMove* move = (PSMove*) calloc( 1, sizeof( PSMove ) );

	if( serial == NULL && path != NULL )
	{
		psmove_DEBUG( "opening HID device using path=\"%s\"\n", path );
		move->hid_handle = hid_open_path( path );
	}
	else
	{
		psmove_DEBUG( "opening HID device using VID=0x%04X PID=0x%04X serial=\"%ls\"\n",
				PSMOVE_VID, PSMOVE_PID, serial );
		move->hid_handle = hid_open( PSMOVE_VID, PSMOVE_PID, serial );
	}

	if( ! move->hid_handle )
	{
		psmove_DEBUG( "opening HID device failed\n" );
		free( move );
		return NULL;
	}

	hid_set_nonblocking( move->hid_handle, 1 );

	// save the device's serial number
	// NOTE: We need to do this since USB devices have an empty serial number
	//       and hidapi's hid_get_serial_number_string() will not return an
	//       empty string for these but fail instead. To later distinguish USB
	//       and Bluetooth devices we need to keep the original serial number.
	move->serial_number = (char*) calloc( 255, sizeof( char ) );
	if( serial )
	{
		wcstombs( move->serial_number, serial, 255 );
	}

	return move;
}


void psmove_disconnect( PSMove* move )
{
	if( ! move )
	{
		return;
	}

	hid_close( move->hid_handle );

	free( move->serial_number );
	free( move );
}


enum PSMove_Connection_Type psmove_connection_type( PSMove const* move )
{
	if( ! move )
	{
		return Conn_Unknown;
	}

	if( move->serial_number == NULL )
	{
		return Conn_Unknown;
	}

	if( strlen( move->serial_number ) == 0 )
	{
		return Conn_USB;
	}

	return Conn_Bluetooth;
}



