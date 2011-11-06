#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QFile>
#include "../inc/protocol.h"

class Server : public QTcpServer
{
  Q_OBJECT

  QFile       m_file;
  uchar      *m_map;        // pointer to file memory
  qlonglong   m_blocks;     // number of file blocks
  qlonglong   m_lastblock;  // last data block size if any

public:
    Server(const QString &file, QObject *parent);
    ~Server();
    bool isFileCorrect() const;
    bool getDataBlock(qlonglong blocknum, char *buf, int bufsize, int *blocksize);


  protected slots:
    void createNewClient();

};

#endif // SERVER_H
