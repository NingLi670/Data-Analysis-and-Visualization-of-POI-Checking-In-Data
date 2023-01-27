#ifndef READFILE_H
#define READFILE_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>

class MainWindow;

class readfile :  public QObject
{
    Q_OBJECT
public:
    readfile(MainWindow *);
    void loaddata();
    //void sort_time();
    void transfer_time();
signals:
    void workdone();
    void pro(int count);

private:
    MainWindow *w;
    QString filename;
};

#endif // READFILE_H
