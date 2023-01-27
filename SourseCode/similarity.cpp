#include "mainwindow.h"
#include "ui_mainwindow.h"



int common(QVector<int> &a,QVector<int> &b){
    int c=0;
    for(int i=0;i<6;i++){
        for(int j=0;j<6;j++){
            if(a[i]==b[j]){
                c++;
                break;
            }
        }
    }
    return c;
}

int MainWindow::similar_hour(int usera,int userb,bool flag){
    QVector<double> hoursa(24),hoursb(24);
    QVector<int> topsa(24),topsb(24);
    std::iota(topsa.begin(),topsa.end(),0);
    std::iota(topsb.begin(),topsb.end(),0);
    for(int i=user_pos[usera];i<user_pos[usera+1];i++){
        hoursa[time_id[i][11].digitValue()*10+time_id[i][12].digitValue()]++;
    }
    for(int i=user_pos[userb];i<user_pos[userb+1];i++){
        hoursb[time_id[i][11].digitValue()*10+time_id[i][12].digitValue()]++;
    }
    std::sort(topsa.begin(),topsa.end(),[&hoursa](int x,int y){return hoursa[x]>hoursa[y];});
    std::sort(topsb.begin(),topsb.end(),[&hoursb](int x,int y){return hoursb[x]>hoursb[y];});
    if(flag){
        auto *seta=new QBarSet("Usera");
        seta->append(hoursa);
        auto *setb=new QBarSet("Userb");
        setb->append(hoursb);
        auto barseries=new QBarSeries;
        barseries->append(seta);
        barseries->append(setb);
        QChart *chart = new QChart();
        ui->schart->setChart(chart);
        ui->schart->setRubberBand(QChartView::HorizontalRubberBand);
        chart->addSeries(barseries);
        chart->setTitle("The Number Of Checking_ins between two users in a day");
        QValueAxis *axisy = new QValueAxis;
        axisy->setTitleText("The Number Of Checking_ins");

        QStringList hour;
        for(int i=0;i<24;i++){
            hour.append(QString::fromStdString(std::to_string(i)));
        }
        QBarCategoryAxis *axisx = new QBarCategoryAxis();
        axisx->append(hour);
        axisx->setTitleText("Hour");
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->addAxis(axisx,Qt::AlignBottom);
        chart->addAxis(axisy,Qt::AlignLeft);
        barseries->attachAxis(axisy);
        barseries->attachAxis(axisx);
    }

    return common(topsa,topsb);
}

double MainWindow::similar_week(int usera,int userb,bool flag){
    QVector<double> counta(8),countb(8);
    for(int i=user_pos[usera];i<user_pos[usera+1];i++){
        counta[QDate::fromString(time_id[i].left(10),"yyyy-MM-dd").dayOfWeek()]++;
    }
    for(int i=user_pos[userb];i<user_pos[userb+1];i++){
        countb[QDate::fromString(time_id[i].left(10),"yyyy-MM-dd").dayOfWeek()]++;
    }
    int maxa=1,submaxa=1,maxb=1,submaxb=1;
    for(int i=2;i<=7;i++){
        if(counta[i]>counta[maxa]){ submaxa=maxa;maxa=i;}
        else if(counta[i]>counta[submaxa]) submaxa=i;
        if(countb[i]>countb[maxb]){ submaxb=maxb;maxb=i;}
        else if(countb[i]>countb[submaxb]) submaxb=i;
    }
    if(flag){
        counta.erase(counta.begin());
        countb.erase(countb.begin());
        auto *seta=new QBarSet("Usera");
        seta->append(counta);
        auto *setb=new QBarSet("Userb");
        setb->append(countb);
        auto barseries=new QBarSeries;
        barseries->append(seta);
        barseries->append(setb);
        QChart *chart = new QChart();
        ui->schart->setChart(chart);
        ui->schart->setRubberBand(QChartView::HorizontalRubberBand);
        chart->addSeries(barseries);
        chart->setTitle("The Number Of Checking_ins between two users in a week");
        QValueAxis *axisy = new QValueAxis;
        axisy->setTitleText("The Number Of Checking_ins");
        QStringList week;
        week<<"Monday"<<"Tuesday"<<"Wednesday"<<"Thursday"<<"Friday"<<"Saturday"<<"Sunday";
        QBarCategoryAxis *axisx = new QBarCategoryAxis();
        axisx->append(week);
        chart->setAnimationOptions(QChart::SeriesAnimations);
        chart->addAxis(axisx,Qt::AlignBottom);
        chart->addAxis(axisy,Qt::AlignLeft);
        barseries->attachAxis(axisy);
        barseries->attachAxis(axisx);
    }
    if(maxa==maxb&&submaxa==submaxb) return 1;
    else if(maxa==submaxb&&maxb==submaxa) return 0.75;
    else if(maxa==maxb||maxa==submaxb||submaxa==maxb||submaxa==submaxb) return 0.5;
    else return 0;
}

QVector<double> MainWindow::similar_poi(int usera,int userb){
    double common=0;
    for(int i=user_pos[usera];i<user_pos[usera+1];i++){
        if(std::find(location_id.begin()+user_pos[userb],location_id.begin()+user_pos[userb+1],location_id[i])!=location_id.begin()+user_pos[userb+1]){
            common++;
        }
    }
    QVector<double> r;
    r.push_back(common);
    r.push_back((common/(user_pos[usera+1]-user_pos[usera])+common/(user_pos[userb+1]-user_pos[userb]))/2);
    return r;
}

QString MainWindow::similar(int usera,int userb){
    int h=similar_hour(usera,userb,false);
    double w=similar_week(usera,userb,false);
    int ww=0;
    if(w==1||w==0.75) ww=2;
    else if(w==0.5) ww=1;
    QVector<double> p=similar_poi(usera,userb);
    int pp=p[0];
    double result=double(h)/6*24/31*0.9+w*7/31*0.9+p[1]*0.1;
    result*=100;
    int r=result;
    std::string text="Usera and userb share ";
    text+=std::to_string(h);
    text+=" of their top six active hours in a day.\nUsera and userb share ";
    text+=std::to_string(ww);
    text+=" of their top two active days in a week.\nUsera and userb had visited the same POIs for ";
    text+=std::to_string(pp);
    text+=" times.\nThe similarity between usera and userb is ";
    text+=std::to_string(r);
    text+="%.";
    return QString::fromStdString(text);
}
