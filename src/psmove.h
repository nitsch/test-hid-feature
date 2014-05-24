#ifndef PSMOVE_H_
#define PSMOVE_H_

struct _PSMove;
typedef struct _PSMove PSMove;


int psmove_count_connected();

PSMove* psmove_connect();
void psmove_disconnect( PSMove* move );


#endif // PSMOVE_H_

