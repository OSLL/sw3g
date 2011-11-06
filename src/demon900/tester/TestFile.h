#ifndef TESTFILE_H
#define TESTFILE_H

#include <QtTest/QtTest>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

#include "../inc/protocol.h"

class TestFile : public QObject
{
  Q_OBJECT;

  QTcpSocket m_socket;
  Message    m_message;

public:

  TestFile()
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

    return (-1 != result);
  }

  bool read()
  {
    char *iobuf = reinterpret_cast<char*>(&m_message);
    m_socket.waitForReadyRead(-1);
    qint64 result = m_socket.read(iobuf,sizeof(m_message));
    return (-1 != result);
  }

private slots:

  void downloadFile()
  {
    QFile file("dwnl.dat");
    file.open(QIODevice::WriteOnly);
    qlonglong number = 0;

    do
    {
      prepare();
      m_message.type = MSG_DATA_REQ;
      m_message.blocknum = number++;

      QVERIFY( write() );
      QVERIFY( read() );

      if( MSG_DATA_BLK == m_message.type)
      {
        file.write(m_message.data,m_message.datasize);
      }

    } while (MSG_NOP != m_message.type);

    file.close();
  }
};


#endif // TESTFILE_H
