#include "device.h"
#include<QWidget>
#include<QVBoxLayout>
#include<QPushButton>
#include<QStyleOption>
#include<QPainter>
#include<QLabel>
#include<QProcess>
#include<QDebug>

Device::Device(QWidget *parent) : QWidget(parent)
{

setStyleSheet("background-color:#a5a5a5;");

    setFixedSize(200,100);
    QPushButton *button1 = new QPushButton("Ara",this);
    connect(button1, SIGNAL(clicked(bool)), this, SLOT(button1click()));

    QPushButton *baglanButton = new QPushButton("Bağlan",this);
    connect(baglanButton, SIGNAL(clicked(bool)), this, SLOT(baglanButtonclick()));

    QPushButton *button2 = new QPushButton("Yansıt",this);
    connect(button2, SIGNAL(clicked(bool)), this, SLOT(button2click()));
    cihaz= new QLabel(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(cihaz);
    layout->addWidget(button1);
    layout->addWidget(baglanButton);

    layout->addWidget(button2);

    setLayout(layout);


}

void Device::setDeviceName(QString name)
{
    cihaz->setText(name);
}
void Device::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

QString Device::komut(QString cmd)
{
    QProcess process;
    process.start(cmd);
    process.waitForFinished(-1); // will wait forever until finished

    QString stdout = process.readAllStandardOutput();
    QString stderr = process.readAllStandardError();
    QStringList liste=stdout.split("\n");
    stdout.chop(1);
    return  stdout;
}

void Device::button1click()
{
    QString name=komut("adb -s "+cihaz->text()+" shell getprop ro.product.brand");
    QString model=komut("adb -s "+cihaz->text()+" shell getprop ro.product.model");
    QString version=komut("adb -s "+cihaz->text()+" shell ro.build.version.release");
    QString codename=komut("adb -s "+cihaz->text()+" shell getprop ro.build.product");
    QString sdk=komut("adb -s "+cihaz->text()+" shell ro.build.version.sdk");
    QString screen=komut("adb -s "+cihaz->text()+" shell wm size");
    ip=komut("adb -s "+cihaz->text()+" shell ip addr show wlan0  | grep 'inet ' | cut -d ' ' -f 6 | cut -d / -f 1");

    qDebug()<<name;
    qDebug()<<model;
    qDebug()<<version;
    qDebug()<<codename;
    qDebug()<<sdk;
    qDebug()<<screen;
    qDebug()<<ip;
}

void Device::button2click()
{
    QString scrcpy=komut("scrcpy");
    qDebug()<<scrcpy;
}

void Device::baglanButtonclick()
{
   // qDebug()<<cihaz->text();
   // return;
    QString devices=komut("adb devices");
    QString tcplisten=komut("adb tcpip 5555");
    QString cnn=komut("adb connect "+cihaz->text()+":5555");
qDebug()<<cnn<<cihaz->text();
}
