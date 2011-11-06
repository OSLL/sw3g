
#include <QtGui/QMainWindow>
//#include <QSystemDeviceInfo>
#include<QLabel>
#include<QtGui>
#include <QSystemNetworkInfo>
#include<time.h>
#include<QWidget>
QTM_USE_NAMESPACE

class bat: public QWidget
{
    Q_OBJECT
public:

    bat(QWidget *parent = 0)
{	
	setupGeneral();
}
    int convert(int num);

private:

        QLabel *status;
    void setupGeneral();
    void compare();

    QSystemNetworkInfo *deviceInfo;


};
