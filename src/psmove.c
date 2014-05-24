#include "psmove.h"

#include "hidapi.h"


// USB Vendor ID and Product ID of PS Move Motion Controller
#define PSMOVE_VID 0x054C
#define PSMOVE_PID 0x03D5


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

