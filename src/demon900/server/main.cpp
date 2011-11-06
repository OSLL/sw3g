#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include "server.h"

int main(int c, char **v)
{
  QCoreApplication app(c,v);

  if(c !=2 )
  {
    qDebug() << "You have to use:\n server [path to file]";
  }

  Server server(v[1],NULL);

  if(server.isFileCorrect())
  {
    qDebug() << "Using file " << v[1];
  }
  else
  {
    qDebug() << "Can't open of invalid file " << v[1];
    return 1;
  }

  server.listen(QHostAddress::Any,DEBUG_PORT);

  while(true)
  {
    qDebug() << "Wait for new connections...";
    server.waitForNewConnection(-1);
  }
  return 0;
}
