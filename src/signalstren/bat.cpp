#include "bat.h"
#include <QtCore/QCoreApplication>
#include <QDialog>
#include <QtGui>
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <QMessageBox>

#include "core/scanner.h"
#include "core/net_info.h"
#include "impl/scanners.h"
#include "impl/units.h"

using namespace std;
using namespace fine;
using namespace fine::impl;

int i=0,j=0,num=0,count1=0;
char name[10][30],sig[10][15],ch,str[15];

FILE *fp;
void bat::setupGeneral()
{
    // detecting networks and updating net_info
    set<network> detected;
    for (network_type net_type = FIRST; net_type != LAST; ++net_type) {
        scanner &scan = scanners::instance().get(net_type);
        set<network> detected_for_type = scan.scan();
        if (!detected_for_type.empty()) {
            detected.insert(detected_for_type.begin(), detected_for_type.end());
        }
    }
    cout << "=============== before update ================" << "\n";
    net_info::instance().dump();
    net_info::instance().update(detected);
    cout << "\n\n=============== after update ================" << "\n";
    net_info::instance().dump();

    exit(0);
    // old code:

    status=new QLabel("");

    QGridLayout *lay =new QGridLayout;
    lay->addWidget(status,0,0);
    deviceInfo = new QSystemNetworkInfo;

    status->setNum(deviceInfo->networkSignalStrength(QSystemNetworkInfo::WcdmaMode));

    connect(deviceInfo, SIGNAL(networkSignalStrengthChanged(QSystemNetworkInfo::NetworkMode, int)),
            this, SLOT(signalStrengthChanged(QSystemNetworkInfo::NetworkMode, int)));
    compare();
    exit(0);
    setLayout(lay);
}

void bat::signalStrengthChanged(QSystemNetworkInfo::NetworkMode, int strength) {
    status->setNum(strength);
}

void bat::compare()
{

    status->setNum(deviceInfo->networkSignalStrength(QSystemNetworkInfo::WcdmaMode));
    system("iwlist scan | grep 'ESSID' | awk -F '\"' '{print $2}' > wlanname.out");
    system("iwlist scan | grep 'Signal level' | awk -F ' ' '{print $3}' | cut -b 7-  > signal.out");
    fp = fopen("wlanname.out","r");

    i=0;
    while(!feof(fp))
    {
        fscanf(fp,"%[^\n]s",name[i]);
        i++;
        ch = getc(fp);
        if(ch==EOF)
            break;
        if(fp==NULL)
            break;
    }
    fclose(fp);
    count1 = i;
    i=0;
    fp = fopen("signal.out","r");
    while(!feof(fp))
    {
        fscanf(fp,"%[^\n]s",sig[i]);
        i++;
        ch = getc(fp);
        if(ch==EOF)
            break;
        if(fp==NULL)
            break;
    }
    i=0;fclose(fp);
    fp = fopen("wireless_name.txt","r");
    fscanf(fp,"%[^\n]s",str);
    fclose(fp);
    while(i!=count1)
    {
        if(strcmp(name[i],str)==0)
        {
            num = atoi(sig[i]);
            //num = -(num);
            num = convert(num);
            break;
        }
        i++;
    }

    if(num>(status->text().toInt()))
    {
        system("echo wireless > aq.txt");
    }
    else
    {
        system("echo 3G > aq.txt");

    }
}

int bat::convert(int num) {
    // todo get rid of this...
    return -1;
}

