#include "serverthread.h"

#include <QTcpSocket>


int ServerThread::s_sessionId = 0;

ServerThread::ServerThread(QTcpSocket *socket, Server *server) :
    QThread(NULL), m_socket(socket),  m_server(server)
{
}

void ServerThread::run()
{
  while( QTcpSocket::ConnectedState == m_socket->state() )
  {
    qDebug() << "Waiting a message..";
    if (!receiveMessage())
      terminate();

    if( MAGIC != m_message.magic)
    {
      qDebug() << "Invalid magic, skip data";
      return;
    } else
    {
      qDebug() << "Message type " << m_message.type;
    }

    switch(m_message.type)
    {
    case MSG_SESSION_REQ:
      handleSessionReq();
      break;

    case MSG_SESSION_KILL:
      handleSessionKill();
      break;

    case MSG_DATA_REQ:
      handleDataReq();
      break;

    default:
      qDebug() << "Unknown/invalid message type, skip data";
      handleError();
    }
  }
}

void ServerThread::handleSessionReq()
{
  m_message.type = MSG_SESSION_ID;
  sendMessage();
}

void ServerThread::handleSessionKill()
{
  m_message.type = MSG_NOP;
  sendMessage();
}

void ServerThread::handleDataReq()
{
  m_message.type = MSG_DATA_BLK;
  if (m_server->getDataBlock(m_message.blocknum,m_message.data,MAX_DATA_SIZE,&m_message.datasize))
  {
    sendMessage();
  }
  else
  {
    handleError();
  }
}

void ServerThread::handleError()
{
  m_message.type = MSG_NOP;
  sendMessage();
}

bool ServerThread::sendMessage()
{
  int result = m_socket->write(reinterpret_cast<const char*>(&m_message),sizeof(Message));
  m_socket->waitForBytesWritten(-1);
  qDebug() << "written bytes " << result;
  return -1 != result;
}

bool ServerThread::receiveMessage()
{
  memset(&m_message,0,sizeof(Message));
  m_socket->waitForReadyRead(-1);
  int result = m_socket->read(reinterpret_cast<char*>(&m_message),sizeof(Message));
  qDebug() << "read bytes " << result;
  return -1 != result;
}
