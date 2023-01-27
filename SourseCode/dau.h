#ifndef DAU_H
#define DAU_H

#include <QString>
#include <QVector>
#include <QDateTime>

class MainWindow;

class DAU
{
public:
    DAU(MainWindow *w);
    void calculate(int a,int b);
    void calculate(int c);
    QVector<int> counta;
    QVector<int> countb;
    QVector<int> countc;
    QVector<QDateTime> axisxvalue;
    QVector<QDateTime> axisxvaluec;
private:
    MainWindow *w;
    int poia;
    int poib;
    int poic;
//    QStringList timesa,timesb;
//    QVector<QDateTime> stdtimesa,stdtimesb;
//    QVector<int> counta(730),countb(730);
//    QDateTime tmp(QDate(2009,1,1),QTime(0,0,0));
//    QVector<QDateTime> axisxvalue;
};

#endif // DAU_H
