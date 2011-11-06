#include "server.h"
#include "serverthread.h"
#include <QTcpSocket>


Server::Server(const QString &file, QObject *parent) : QTcpServer(parent), m_file(file)
{
  setMaxPendingConnections(1); // No DoS attack ((:

  m_file.open(QIODevice::ReadOnly);
  m_map = m_file.map(0,m_file.size());
  m_blocks = m_file.size()/MAX_DATA_SIZE;
  m_lastblock = m_file.size() - (MAX_DATA_SIZE * m_blocks);

  connect(this,SIGNAL(newConnection()), this, SLOT(createNewClient()));
}


Server::~Server()
{
  m_file.unmap(m_map);
  m_file.close();
}

void Server::createNewClient()
{
  qDebug() << "createNewClient called";
  (new ServerThread(nextPendingConnection(),this))->start();
}

bool Server::isFileCorrect() const
{
  if ( m_file.isOpen() && m_map != NULL && m_blocks >=1)
    return true;
  return false;
}

bool Server::getDataBlock(qlonglong blocknum, char *buf, int bufsize, int *blocksize)
{
  // no mutexes because only read operation and only local parameters
  qlonglong realblocks = m_blocks + ( m_lastblock>0 ? 1 : 0);

  // check parameters
  if( blocknum >= realblocks  || blocknum < 0 || bufsize < MAX_DATA_SIZE  )
    return false;

  uchar *offset = m_map + (MAX_DATA_SIZE * blocknum);

  if( blocknum == m_blocks ) // last block
  {
    *blocksize = m_lastblock;
  }
  else
  {
    *blocksize = MAX_DATA_SIZE;
  }

  memcpy(buf,offset,*blocksize);
  return true;
}
