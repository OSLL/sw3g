#include "bat.h"
#include <QtCore/QCoreApplication>
#include <QDialog>
#include <QtGui>
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <QMessageBox>
using namespace std;

int i=0,j=0,num=0,count1=0;
char name[10][30],sig[10][15],ch,str[15];

FILE *fp;
void bat::setupGeneral()
{
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
int bat::convert(int num)
{
    char string[100];
    int n=0;
    strcpy(string,"grep -w '\\");
    char ch1[6];
    sprintf(ch1,"%d",num);
    //cout<<"jjj"<<ch1<<"\n";
    strcat(string,ch1);
    strcat(string,"' converter.txt > value.out");
    system(string);
    FILE *f1;
    char str1[100];

    f1 = fopen("value.out","r");
    fscanf(f1,"%[^\n]s",str1);
    // cout<<"str"<<str1;
    int i=0,j=0;
    while(str1[i]!='='&&str1[i]!='\0')
        i++;
    i++;
    i++;
    while(str1[i]!='\0')
    {   //cout<<"sdhjsd"<<str1[i];
        ch1[j]=str1[i];
        j++;i++;
    }
    ch1[j]='\0';
    n = atoi(ch1);
    fclose(f1);
    return n;
}
