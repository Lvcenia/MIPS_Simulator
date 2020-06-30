#ifndef CHARDATA_H
#define CHARDATA_H

#include <QObject>
#include <QFile>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>
class CharData
{
public:
    CharData(const QString& chara)
    {
        QTextCodec* gbk = QTextCodec::codecForName("GB2312");
        QString a = gbk->toUnicode(chara.toLocal8Bit());
        QByteArray u = a.toLocal8Bit();
        qDebug() << a;
        this->Q = (u[0] - 0xA1)&0xFF;
        qDebug() <<"String: " + QString(u) << "Q: "<< Q;
        this->W = (u[1] - 0xA1)&0xFF;
        qDebug() << "W: "<< W;
        this->number = (this->Q * 94 + this->W);
    }
    QList<uint16_t> data;
    int Q;
    int W;
    int number;
    short Matrix[16];//从上到下
};


#endif // CHARDATA_H
