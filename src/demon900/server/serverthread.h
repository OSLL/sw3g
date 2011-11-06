#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include "../inc/protocol.h"
#include "server.h"

class ServerThread : public QThread
{
  Q_OBJECT

  QTcpSocket  *m_socket;
  Message      m_message;
  Server      *m_server;

  static int s_sessionId;

public:
  explicit ServerThread(QTcpSocket *socket, Server *server);
  void run();

protected:
  void handleSessionReq();
  void handleSessionKill();
  void handleDataReq();
  void handleError();

  bool receiveMessage();
  bool sendMessage();

signals:

public slots:

};

#endif // SERVERTHREAD_H
