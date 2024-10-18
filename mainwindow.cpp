#include "mainwindow.h"
#include<QProcess>
#include<QDebug>
#include<QLabel>
#include<QWidget>
#include<QPushButton>
#include<QLayout>
#include<device.h>
#include<QHostInfo>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("scrcpy Gui"));
    resize(400,600);
    udpSocketSend = new QUdpSocket(this);
    timerUdpSocketSend = new QTimer(this);
    connect(timerUdpSocketSend, SIGNAL(timeout()), this, SLOT(yenileClick()));
    timerUdpSocketSend->start(5000);

     QPushButton *button1 = new QPushButton("Yenile",this);
     connect(button1, SIGNAL(clicked(bool)), this, SLOT(yenileClick()));


    dliste=new QWidget(this);
    dliste->move(1,30);
    layout=new QVBoxLayout(dliste);
    yenileClick();


}
void MainWindow::yenileClick()
{
    sendBroadcast();
    qDebug()<<"yenilendi.......";
    deviceliste.clear();
    qDeleteAll(dliste->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly));

    deviceliste.append(readArp());
    for(int i=0;i<deviceliste.length();i++)    {
        if(deviceliste[i].split(".")[3]=="1") continue;
        //  if(deviceliste[i]==ipmaclist[0].ip) continue;

        dliste->setFixedSize(this->width(),this->height()-30);
        Device *device = new Device(dliste);
        device->setDeviceName(deviceliste[i]);
        layout->addWidget(device);
    }
}

void MainWindow::sendBroadcast()
{
    hostAddressMacButtonSlot();
    for(int i=0;i<ipmaclist.count();i++)
    {
          // qDebug()<<ipmaclist[i].broadcast;
            for(int k=1;k<255;k++)
            {
                QString broadadres;
                broadadres=ipmaclist[i].broadcast.section(".",0,2)+"."+QString::number(k);
              /// qDebug()<<broadadres<<k;
                QString msg="";
                QByteArray datagram = msg.toUtf8();
                //udpSocketSend->writeDatagram("",QHostAddress(broadCastAddress1), tcpPort.toInt());
                udpSocketSend->writeDatagram(datagram,QHostAddress(broadadres), tcpPort.toInt());
            }

    }

    qDebug()<<"broadcasting network...";
}

QStringList MainWindow::readArp()
{
    QStringList list;
    const int size = 256;

    char ip_address[size];
    int hw_type;
    int flags;
    char mac_address[size];
    char mask[size];
    char device[size];

    FILE* fp = fopen("/proc/net/arp", "r");
    if(fp == NULL)
    {
        perror("Error opening /proc/net/arp");
    }

    char line[size];
    fgets(line, size, fp);    // Skip the first line, which consists of column headers.
    while(fgets(line, size, fp))
    {
        sscanf(line, "%s 0x%x 0x%x %s %s %s\n",
               ip_address,
               &hw_type,
               &flags,
               mac_address,
               mask,
               device);
        if(flags==2)
        {
            ///qDebug()<< ip_address<< mac_address<<hw_type<<flags;
            list<<QString(ip_address);//+"|"+ QString(mac_address);


        }
    }

    fclose(fp);
    return list;
}

void MainWindow::hostAddressMacButtonSlot()
{
    QHostAddress localhost = QHostAddress(QHostAddress::LocalHost);
    ipmaclist.clear();
    foreach (const QNetworkInterface& networkInterface, QNetworkInterface::allInterfaces()) {
        foreach (const QNetworkAddressEntry& entry, networkInterface.addressEntries()) {
            QHostAddress *hostadres=new QHostAddress(entry.ip().toString());
            if(hostadres->protocol() == QAbstractSocket::IPv4Protocol &&
                !hostadres->isLoopback() )
            {
                IpMac im;
                im.ip=entry.ip().toString();
                im.mac=networkInterface.hardwareAddress();
                im.broadcast=entry.broadcast().toString();
                ipmaclist.append(im);

                // qDebug()<<"mac:"<<networkInterface.hardwareAddress();
                //qDebug()<<"ip  address:"<<entry.ip().toString();
                // qDebug()<<"broadcast  address:"<<entry.broadcast().toString();
            }
        }
    }
    //qDebug()<<"ip  address:"<<ipmaclist.count();


}

MainWindow::~MainWindow() {}
