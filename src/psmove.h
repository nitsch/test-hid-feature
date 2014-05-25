#ifndef PSMOVE_H_
#define PSMOVE_H_

#include <stdlib.h>


struct _PSMove;
typedef struct _PSMove PSMove;

enum PSMove_Connection_Type
{
	Conn_Bluetooth,
	Conn_USB,
	Conn_Unknown,
};


int psmove_count_connected();

PSMove* psmove_connect();
void psmove_disconnect( PSMove* move );

enum PSMove_Connection_Type psmove_connection_type( PSMove const* move );

int psmove_send_feature_report( PSMove const* move, unsigned char const* buf, size_t len );
int psmove_get_feature_report( PSMove const* move, unsigned char* buf, size_t len );


#endif // PSMOVE_H_

