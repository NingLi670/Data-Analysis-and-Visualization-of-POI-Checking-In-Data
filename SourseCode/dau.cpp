#include "dau.h"
#include "mainwindow.h"
#include <set>

DAU::DAU(MainWindow *_w):w(_w),poia(-1),poib(-1),poic(-1)
{
}

void DAU::calculate(int a,int b){
    if(a==poia&&b==poib) return;
    counta.clear();
    countb.clear();
    axisxvalue.clear();
    poia=a;
    poib=b;
    QStringList timesa,timesb;
    QVector<QDateTime> stdtimesa,stdtimesb;
    counta.resize(730);
    countb.resize(730);
    QDateTime tmp(QDate(2009,1,1),QTime(0,0,0));
    for(int i=0;i<731;i++){
        axisxvalue.push_back(tmp);
        tmp=tmp.addDays(1);
    }
    //QVector<int> indexb;
    QVector<int> ia,ib;
    for(int i=0;i<w->location_count[poia].size();i++){
        //qDebug()<<w->location_count[poia][i];
        timesa.push_back(w->time_id[w->location_count[poia][i]]);
        ia.push_back(w->location_count[poia][i]);
    }
    std::vector<int> indexa(timesa.size());
    std::iota(indexa.begin(), indexa.end(), 0);
    //std::sort(timesa.begin(),timesa.end());
    std::sort(indexa.begin(), indexa.end(),[&timesa](int index_1, int index_2) { return timesa[index_1] < timesa[index_2]; });
    for(int i=0;i<w->location_count[poib].size();i++){
        timesb.push_back(w->time_id[w->location_count[poib][i]]);
        ib.push_back(w->location_count[poib][i]);
    }
    std::vector<int> indexb(timesb.size());
    std::iota(indexb.begin(), indexb.end(), 0);
    //std::sort(timesb.begin(),timesb.end());
    std::sort(indexb.begin(), indexb.end(),[&timesb](int index_1, int index_2) { return timesb[index_1] < timesb[index_2]; });
    //qDebug()<<w->location_count[poia].size()<<w->time_id.size();
    for(int i=0;i<timesa.size();i++){
        stdtimesa.push_back(QDateTime::fromString(timesa[indexa[i]].left(10),"yyyy-MM-dd"));
    }
    for(int i=0;i<timesb.size();i++){
        stdtimesb.push_back(QDateTime::fromString(timesb[indexb[i]].left(10),"yyyy-MM-dd"));
    }
    QVector<QDateTime>::iterator p=stdtimesa.begin();
    for(int i=0;i<730;i++){
        std::set<int> tmp;
        while(p!=stdtimesa.end()&&*p<axisxvalue[i+1]){
            tmp.insert(w->user_id[ia[indexa[p-stdtimesa.begin()]]]);
            p++;
        }
        counta[i]=tmp.size();
    }
    p=stdtimesb.begin();
    for(int i=0;i<730;i++){
        std::set<int> tmp;
        while(p!=stdtimesb.end()&&*p<axisxvalue[i+1]){
            tmp.insert(w->user_id[ib[indexb[p-stdtimesb.begin()]]]);
            p++;
        }
        countb[i]=tmp.size();
    }
    axisxvalue.pop_back();
    int min=0,max=729;
    while(min<counta.size()&&counta[min]==0&&countb[min]==0) min++;
    //qDebug()<<counta.size()<<counta[min]<<countb[min];
    while(max>=0&&counta[max]==0&&countb[max]==0) max--;
    if(min>=max) return;
    //qDebug()<<min<<max;
    counta.erase(counta.begin()+max,counta.end());
    countb.erase(countb.begin()+max,countb.end());
    axisxvalue.erase(axisxvalue.begin()+max,axisxvalue.end());
    counta.erase(counta.begin(),counta.begin()+min+1);
    countb.erase(countb.begin(),countb.begin()+min+1);
    axisxvalue.erase(axisxvalue.begin(),axisxvalue.begin()+min+1);
    //qDebug()<<counta.size()<<countb.size()<<axisxvalue.size();
}


void DAU::calculate(int c){
    if(c==poic) return;
    countc.clear();
    axisxvaluec.clear();
    poic=c;
    QStringList timesc;
    QVector<QDateTime> stdtimesc;
    countc.resize(730);
    QDateTime tmp(QDate(2009,1,1),QTime(0,0,0));
    for(int i=0;i<731;i++){
        axisxvaluec.push_back(tmp);
        tmp=tmp.addDays(1);
    }

    for(int i=0;i<w->location_count[poic].size();i++){
        //qDebug()<<w->location_count[poia][i];
        timesc.push_back(w->time_id[w->location_count[poic][i]]);
    }
    std::sort(timesc.begin(),timesc.end());
    for(int i=0;i<timesc.size();i++){
        stdtimesc.push_back(QDateTime::fromString(timesc[i].left(10),"yyyy-MM-dd"));
    }
    QVector<QDateTime>::iterator p=stdtimesc.begin();
    for(int i=0;i<730;i++){
        while(p!=stdtimesc.end()&&*p<axisxvaluec[i+1]){
            p++;
            countc[i]++;
        }
    }
    axisxvaluec.pop_back();
    int min=0,max=729;
    while(min<countc.size()&&countc[min]==0) min++;
    //qDebug()<<counta.size()<<counta[min]<<countb[min];
    while(max>=0&&countc[max]==0) max--;
    if(min>=max) return;
    //qDebug()<<min<<max;
    countc.erase(countc.begin()+max,countc.end());
    axisxvaluec.erase(axisxvaluec.begin()+max,axisxvaluec.end());
    countc.erase(countc.begin(),countc.begin()+min+1);
    axisxvaluec.erase(axisxvaluec.begin(),axisxvaluec.begin()+min+1);
    //qDebug()<<counta.size()<<countb.size()<<axisxvaluec.size();
}
