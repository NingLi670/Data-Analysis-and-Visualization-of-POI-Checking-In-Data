#include "readfile.h"
#include "mainwindow.h"
#include <QDebug>
#include <ctime>

readfile::readfile(MainWindow *_w):w(_w){}

//void readfile::sort_time(){
//    w->sorted_time.resize(w->time_id.size());
//    std::iota(w->sorted_time.begin(),w->sorted_time.end(),0);
//    QVector<QString> &ti=w->time_id;
//    std::sort(w->sorted_time.begin(),w->sorted_time.end(),[&ti](int a,int b){return ti[a]<ti[b];});
//    qDebug()<<"done";
//}

void readfile::loaddata(){
    //time_t start,end;
    //start=clock();
    QFile file("C:/Users/lenovo/Desktop/gowalla.csv");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"OPEN FILE FAILED";
        QMessageBox error(QMessageBox::Critical,"Error","Uploading files failed.",QMessageBox::Close);
        error.exec();
        return;
    }

    QTextStream * out=new QTextStream(&file);
    while(!out->atEnd()){
        w->data.push_back(out->readLine());
    }
    int j;
    for(j=0;j<w->data.size();j++){
        QStringList tmp=w->data[j].split(",");
        if(tmp.count()==5){
            w->user_id.push_back(tmp[0].toInt());
            if(j==0||w->user_id[j]!=w->user_id[j-1]) w->user_pos.push_back(j);
            w->location_id.push_back(tmp[1].toInt());
            w->time_id.push_back(tmp[2]);
            w->la_id.push_back(tmp[3].toDouble());
            w->lo_id.push_back(tmp[4].toDouble());
            w->location_count[w->location_id[j]].push_back(j);
            if(j%15000==0){
                emit pro((double)(j+1)/w->data.size()*100);
            }
            if(!w->location_la_lo[w->location_id[j]]){
                Item *tmpitem=new Item;
                tmpitem->value=w->location_id[j];
                tmpitem->key[0]=w->la_id[j];
                tmpitem->key[1]=w->lo_id[j];
                w->location_la_lo[w->location_id[j]]=tmpitem;
            }
        }
    }
    w->user_pos.push_back(j);
    w->tree=new kdtree(&w->location_la_lo);
    w->tree->build();
    //sort_time();
    transfer_time();
    emit pro(100);
    emit workdone();
    //end=clock();
    //qDebug()<<"time"<<(double)end-start;
}

void readfile::transfer_time(){
    QVector<QDate> stdday;
    QDate tmp(2009,1,1);
    for(int i=0;i<731;i++){
        stdday.push_back(tmp);
        tmp=tmp.addDays(1);
    }
    int month[25]={0,31,28,31,30,30,31,31,31,30,31,30,31,31,28,31,30,30,31,31,31,30,31,30,31};
    int months[25];
    months[0]=31;
    for(int i=1;i<25;i++) months[i]=months[i-1]+month[i];
    for(int i=0;i<w->time_id.size();i++){
        std::string s=w->time_id[i].toStdString();
        int d=months[(s[2]-'0')*12+(s[5]-'0')*10+s[6]-'0'-2]+(s[8]-'0')*10+s[9]-'0'-1;
        w->day_time[d].push_back(i);
    }
}
