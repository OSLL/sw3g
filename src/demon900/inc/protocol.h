#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtGlobal>

static const int MAX_DATA_SIZE = 2000; // max size for data block in bytes

// message types
static const int MSG_NOP          = 0;
static const int MSG_SESSION_REQ  = 1;
static const int MSG_SESSION_ID   = 2;
static const int MSG_SESSION_KILL = 3;
static const int MSG_DATA_REQ     = 4;
static const int MSG_DATA_BLK     = 5;

// Magic value
static const int MAGIC            = 0x75646c61;
static const int DEBUG_SESSION_ID = ((int) -1);
static const int DEBUG_PORT       = 10000;

struct Message
{
  int magic;
  int type;
  int session;
  qlonglong blocknum;
  int       datasize;
  char data[MAX_DATA_SIZE];
};



#endif // PROTOCOL_H
