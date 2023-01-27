#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ctime>
#include <cstdlib>
#include <QAreaSeries>
//#include <QPoint>



void MainWindow::draw_user_compare(){
    chart->removeAllSeries();
    //int usera=196,userb=256;
    int usera=ui->cusera->value();
    int userb=ui->cuserb->value();
    QVector<QVector<int>> a(25);
    QVector<QVector<int>> b(25);
    for(int i=user_pos[usera];i<user_pos[usera+1];i++){
        int tmp=get_time_position(time_id[i]);
        if(std::find(a[tmp].begin(),a[tmp].end(),location_id[i])==a[tmp].end()){
            a[tmp].push_back(location_id[i]);
        }
    }
    for(int i=user_pos[userb];i<user_pos[userb+1];i++){
        int tmp=get_time_position(time_id[i]);
        if(std::find(b[tmp].begin(),b[tmp].end(),location_id[i])==b[tmp].end())
        b[tmp].push_back(location_id[i]);
    }
    QVector<double> counta,countb,common;
    for(int i=1;i<25;i++){
        if(a[i].size()!=0||b[i].size()!=0){
        counta.push_back(a[i].size());
        countb.push_back(b[i].size());
        int commoncount=0;
        for(int j=0;j<a[i].size();j++){
            if(std::find(b[i].begin(),b[i].end(),a[i][j])!=b[i].end()){
                commoncount++;
            }
        }
        common.push_back(commoncount);
        }
        //qDebug()<<counta.size()<<countb.size()<<common.size();
        //qDebug()<<counta[i]<<countb[i]<<common[i];
    }

    QBarSet *seta=new QBarSet("The Number Of POIs Visited By Uesr A");
    seta->append(counta);
    QBarSet *setb=new QBarSet("The Number Of POIs Visited By Uesr B");
    setb->append(countb);
    QBarSet *set=new QBarSet("The Common POIs Visited By Uesr A and B");
    set->append(common);
    QBarSeries *series = new QBarSeries;
    series->append(seta);
    series->append(setb);
    series->append(set);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);  //设置标签显示的位置
    series->setLabelsVisible(true);  //设置数据标签可见
    chart = new QChart();
    ui->cchart->setChart(chart);
    ui->cchart->setRubberBand(QChartView::HorizontalRubberBand);
    chart->addSeries(series);
    chart->setTitle("The Difference Of Visited POIs");

    QStringList categories;
    for(int i=0;i<24;i++){
        if(a[i+1].size()!=0||b[i+1].size()!=0){
        //qDebug()<<"axis"<<i;
        categories.append(year_and_month[i].left(7));
        }
    }
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    axis->setTitleText("Time");
    axis->setLabelsAngle(60);
    QValueAxis *axisy = new QValueAxis;
    axisy->setTitleText("The Number Of POIs");

    chart->setAnimationOptions(QChart::SeriesAnimations);

    chart->addAxis(axis,Qt::AlignBottom);
    chart->addAxis(axisy,Qt::AlignLeft);
    series->attachAxis(axisy);
    series->attachAxis(axis);
    connect(ui->corigin,&QPushButton::clicked,this->chart,&QChart::zoomReset);

}


void MainWindow::draw_dau(){
    //time_t start,end;
    //start=clock();
    chart->removeSeries(lineSeriesa);
    chart->removeSeries(lineSeriesb);
    ui->cchart->resetCachedContent();
    //int poia=14,poib=32;
    dau_data->calculate(ui->cpoia->value(),ui->cpoib->value());
    int m=0;
    for(int i=0;i<dau_data->counta.size();i++){
        if(dau_data->counta[i]>m) m=dau_data->counta[i];
    }
    for(int i=0;i<dau_data->countb.size();i++){
        if(dau_data->countb[i]>m) m=dau_data->countb[i];
    }
    lineSeriesa=new QSplineSeries;
    lineSeriesb=new QSplineSeries;
    lineSeriesa->setName("POIa");
    lineSeriesb->setName("POIb");
    QVector<QPointF> pointsa;
    QVector<QPointF> pointsb;
    for(int i=0;i<dau_data->counta.size();i++){
        pointsa.push_back(QPointF(dau_data->axisxvalue[i].toMSecsSinceEpoch(),dau_data->counta[i]));
        pointsb.push_back(QPointF(dau_data->axisxvalue[i].toMSecsSinceEpoch(),dau_data->countb[i]));
    }
    lineSeriesa->replace(pointsa);
    lineSeriesb->replace(pointsb);
    QDateTimeAxis *axisx=new QDateTimeAxis;
    QValueAxis *axisy=new QValueAxis;
    axisx->setTitleText("TIME");
    axisy->setTitleText("DAU");
    axisy->setRange(0,m);
    //lineSeries->setPointsVisible(true);
    //lineSeries->setPointLabelsVisible(true);
    chart = new QChart();
    chart->addSeries(lineSeriesa);
    chart->addSeries(lineSeriesb);
    lineSeriesa->setUseOpenGL(true);//GPU绘制
    lineSeriesb->setUseOpenGL(true);

    axisx->setFormat("yyyy-MM-dd");
    chart->addAxis(axisx,Qt::AlignBottom);
    chart->addAxis(axisy,Qt::AlignLeft);
    lineSeriesa->attachAxis(axisx);
    lineSeriesb->attachAxis(axisx);
    lineSeriesb->attachAxis(axisy);
    lineSeriesa->attachAxis(axisy);

    chart->setTitle("The DAU Of Two POIs");
    //chart->setAnimationOptions(QChart::SeriesAnimations);
    //mChart->legend()->hide();//隐藏图例
    ui->cchart->setChart(chart);
    ui->cchart->setRubberBand(QChartView::HorizontalRubberBand);
    ui->cchart->setRenderHints(QPainter::Antialiasing);
    connect(ui->corigin,&QPushButton::clicked,this->chart,&QChart::zoomReset);
    //end=clock();
    //qDebug()<<"dau"<<(double)end-start;
}

void MainWindow::draw_dau_bar(){
    chart->removeAllSeries();

    ui->cchart->resetCachedContent();
    //int poia=14,poib=32;
    dau_data->calculate(ui->cpoia->value(),ui->cpoib->value());
    QDateTime fromtime(ui->cfromtime->date(),QTime(0,0,0));
    QDateTime totime(ui->ctotime->date(),QTime(0,0,0));
    //qDebug()<<fromtime<<totime;
    QVector<double>ca,cb;
    QStringList timelist;
    for(int i=0;i<dau_data->axisxvalue.size();i++){
        if(dau_data->axisxvalue[i]>=fromtime&&dau_data->axisxvalue[i]<=totime){
            ca.push_back(dau_data->counta[i]);
            cb.push_back(dau_data->countb[i]);
            timelist.push_back(dau_data->axisxvalue[i].toString("yyyy-MM-dd"));
        }
    }
    QBarSet *seta=new QBarSet("POIa");
    seta->append(ca);
    QBarSet *setb=new QBarSet("POIb");
    setb->append(cb);
    QBarSeries *series=new QBarSeries;
    series->append(seta);
    series->append(setb);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);  //设置标签显示的位置
    series->setLabelsVisible(true);  //设置数据标签可见

    QDateTimeAxis *axisx=new QDateTimeAxis;
    QBarCategoryAxis *axis=new QBarCategoryAxis;
    axis->append(timelist);
    axis->setLabelsAngle(60);
    QValueAxis *axisy=new QValueAxis;
    axisx->setTitleText("TIME");
    axisy->setTitleText("DAU");

    chart = new QChart();
    chart->addSeries(series);

    axisx->setFormat("yyyy-MM-dd");
    chart->addAxis(axis,Qt::AlignBottom);
    chart->addAxis(axisy,Qt::AlignLeft);
    series->attachAxis(axis);
    series->attachAxis(axisy);

    chart->setTitle("The DAU Of Two POIs");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    //mChart->legend()->hide();//隐藏图例
    ui->cchart->setChart(chart);
    ui->cchart->setRubberBand(QChartView::HorizontalRubberBand);
    ui->cchart->setRenderHints(QPainter::Antialiasing);
    connect(ui->corigin,&QPushButton::clicked,this->chart,&QChart::zoomReset);
}
