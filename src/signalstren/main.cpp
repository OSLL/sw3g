#include "bat.h"
#include <QtGui>
#include <QApplication>
#include <QtCore>

int main(int argc,char *argv[])
{
    QApplication app(argc,argv);
    new bat();

    return app.exec();
}
