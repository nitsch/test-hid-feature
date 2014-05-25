#ifndef PSMOVE_H_
#define PSMOVE_H_

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


#endif // PSMOVE_H_

