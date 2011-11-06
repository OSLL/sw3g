#ifndef TESTMESSAGE_H
#define TESTMESSAGE_H

#include <QtTest/QtTest>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

#include "../inc/protocol.h"

class TestMessage : public QObject
{
  Q_OBJECT;

  QTcpSocket m_socket;
  Message    m_message;


public:

  TestMessage()
  {
    m_socket.connectToHost(QHostAddress::LocalHost, DEBUG_PORT);
  }

private:

  void prepare()
  {
    memset(&m_message,0,sizeof(Message));
    m_message.magic = MAGIC;
  }

  bool write()
  {
    const char *iobuf = reinterpret_cast<const char*>(&m_message);
    qint64 result = m_socket.write(iobuf,sizeof(m_message));
    m_socket.waitForBytesWritten();

    qDebug() << "sent " << result << "bytes";
    return (-1 != result);
  }

  bool read()
  {
    char *iobuf = reinterpret_cast<char*>(&m_message);
    m_socket.waitForReadyRead(-1);
    qint64 result = m_socket.read(iobuf,sizeof(m_message));
    qDebug() << "read" << result << "bytes";
    return (-1 != result);
  }

private slots:

  void sessionReq()
  {
    prepare();
    m_message.type = MSG_SESSION_REQ;
    QVERIFY( write() );
    QVERIFY( read() );
    QVERIFY( m_message.type == MSG_SESSION_ID );
  }

  void sessionKill()
  {
    prepare();
    m_message.type = MSG_SESSION_KILL;
    QVERIFY( write() );
    QVERIFY( read() );
    QVERIFY( m_message.type == MSG_NOP );
  }

  void dataReq()
  {
    prepare();
    m_message.type = MSG_DATA_REQ;
    QVERIFY( write() );
    QVERIFY( read() );
    QVERIFY( m_message.type == MSG_DATA_BLK );
  }
};

#endif // TESTMESSAGE_H
