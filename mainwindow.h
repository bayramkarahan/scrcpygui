#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QHostAddress>
#include <QMainWindow>
#include <QNetworkInterface>,
#include<QUdpSocket>
#include<QTimer>
#include<QHostInfo>
#include<QVBoxLayout>

class IpMac
{
public:
    QString ip;
    QString mac;
    QString broadcast;

};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void sendBroadcast();
    void yenileClick();
    QStringList readArp();
    void hostAddressMacButtonSlot();
  private:
    QList<IpMac> ipmaclist;
    bool sendBroadcastStatus;
    QString  tcpPort="5555";
    QUdpSocket *udpSocketSend;
     QTimer *timerUdpSocketSend;
    QWidget *dliste;
    QVBoxLayout *layout;
    QStringList deviceliste;
};
#endif // MAINWINDOW_H
