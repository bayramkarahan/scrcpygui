#include<QWidget>
#include<QPaintEvent>
#include<QLabel>
#ifndef DEVICE_H
#define DEVICE_H

class Device:public QWidget
{
    Q_OBJECT
public:
    explicit Device(QWidget *parent = nullptr);
    void setDeviceName(QString name);
protected:
    void paintEvent(QPaintEvent* event);
    QString komut(QString cmd);
 public slots:
    void button1click();
    void button2click();
    void baglanButtonclick();

private:
    QLabel *cihaz;
    QString ip="192.168.1.100";
};

#endif // DEVICE_H
