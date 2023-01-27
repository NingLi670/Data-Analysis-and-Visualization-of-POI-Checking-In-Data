#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <set>

bool cmp(QPair<int,int> a,QPair<int,int> b){
    return a.second>b.second;
}

void MainWindow::drawtopten(QString users){
    users.remove(" ");
    for(int i=0;i<users.size();i++){
        if(users[i]!=','&&users[i]!='-'&&(users[i]<'0'||users[i]>'9')){
            QMessageBox error(QMessageBox::Critical,"Error","Please Enter correct format.",QMessageBox::Close);
            error.exec();
            return;
        }
    }
    std::set<int> usersid;
    QStringList usersl=users.split(",");
    for(int i=0;i<usersl.size();i++){
        if(usersl[i].size()!=0){
            QStringList tmp=usersl[i].split("-");
            if(tmp.size()==1&&tmp[0].toInt()>=user_id[0]&&tmp[0].toInt()<=user_id[user_id.size()-1]) usersid.insert(tmp[0].toInt());
            else if(tmp.size()==2){
                for(int j=tmp[0].toInt();j<=tmp[1].toInt();j++){
                    if(j>=user_id[0]&&j<=user_id[user_id.size()-1])
                    usersid.insert(j);
                }
            }
        }
    }
    if(usersid.size()==0){
        QMessageBox error(QMessageBox::Critical,"Error","Please Enter User_ids first.",QMessageBox::Close);
        error.exec();
        return;
    }
    QBarSet *set=new QBarSet("Checking_ins");

    QString timea=ui->fromtime->dateTime().toString("yyyy-MM-ddThh:mm:ssZ");
    QString timeb=ui->totime->dateTime().toString("yyyy-MM-ddThh:mm:ssZ");
    std::map<int,int> map;
    for(std::set<int>::iterator p=usersid.begin();p!=usersid.end();p++){
    for(int i=user_pos[*p];i<user_pos[(*p)+1];i++){
        if(timea<=time_id[i]&&timeb>=time_id[i])
        map[location_id[i]]++;
    }
    }
    if(map.size()==0){
        QChart *chart = new QChart();
        ui->stchart->setChart(chart);
        return;}
    QVector<std::pair<int,int>> unsort(map.begin(),map.end());
    std::sort(unsort.begin(),unsort.end(),cmp);
    QVector<double> top;
    for(int i=0;i<10;i++){
        if(unsort.size()>i)
            top.push_back(unsort[i].second);
    }
    set->append(top);
    QBarSeries *series = new QBarSeries;
    series->append(set);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);  //设置标签显示的位置
    series->setLabelsVisible(true);  //设置数据标签可见
    QChart *chart = new QChart();
    ui->stchart->setChart(chart);
    ui->stchart->setRubberBand(QChartView::HorizontalRubberBand);
    chart->addSeries(series);
    chart->setTitle("The Top 10 Popular POIs");


    QStringList categories;
    for(int i=0;i<10;i++){
        if(unsort.size()>i)
        categories.append(QString::number(unsort[i].first));
    }
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    QValueAxis *axisy = new QValueAxis;
    axisy->setRange(0,unsort[0].second);

    chart->setAnimationOptions(QChart::SeriesAnimations);
    axis->setTitleText("Location_id");
    axisy->setTitleText("The Number Of Checking_ins");
    chart->addAxis(axis,Qt::AlignBottom);
    chart->addAxis(axisy,Qt::AlignLeft);
    series->attachAxis(axisy);
}
int MainWindow::get_time_position(QString &time){
    int i;
    for(i=1;i<=24;i++){
        if(time<year_and_month[i]) return i;
    }
    return i;
}

void MainWindow::drawchecking_in(){
    //chart->removeSeries(lineSeriesc);
    dau_data->calculate(ui->poispin->value());
    int m=0;
    for(int i=0;i<dau_data->countc.size();i++){
        if(dau_data->countc[i]>m) m=dau_data->countc[i];
    }
    lineSeriesc=new QSplineSeries;
    //lineSeriesc->setName("POIa");
    QVector<QPointF> pointsc;
    for(int i=0;i<dau_data->countc.size();i++){
        pointsc.push_back(QPointF(dau_data->axisxvaluec[i].toMSecsSinceEpoch(),dau_data->countc[i]));
    }
    lineSeriesc->replace(pointsc);

    QDateTimeAxis *axisx=new QDateTimeAxis;
    QValueAxis *axisy=new QValueAxis;
    axisx->setTitleText("TIME");
    axisy->setTitleText("The Number Of Checking_ins");
    axisy->setRange(0,m+1);
    //lineSeries->setPointsVisible(true);
    //lineSeries->setPointLabelsVisible(true);
    auto chart1 = new QChart();
    chart1->addSeries(lineSeriesc);
    lineSeriesc->setUseOpenGL(true);
    //chart->createDefaultAxes();

    axisx->setFormat("yyyy-MM-dd");
    chart1->addAxis(axisx,Qt::AlignBottom);
    chart1->addAxis(axisy,Qt::AlignLeft);
    lineSeriesc->attachAxis(axisx);
    lineSeriesc->attachAxis(axisy);

    QString t1="The Number Of Checking_ins Of POI ";
    QString t2=QString::fromStdString(std::to_string(ui->poispin->value()));
    chart1->setTitle(t1+t2);
    chart1->legend()->hide();//隐藏图例
    ui->stchart->setChart(chart1);
    ui->stchart->setRubberBand(QChartView::HorizontalRubberBand);
    ui->stchart->setRenderHints(QPainter::Antialiasing);
}

void MainWindow::drawchecking_ins(bool flag){
    std::sort(tree->pois.begin(),tree->pois.end());
    QBarSet *set=new QBarSet("Checking_ins");
    QVector<double> charty;
    for(int i=chartleft;i<chartright;i++){
        int tmp=0;
        for(int j=0;j<location_count[tree->pois[i]].size();j++){
            if(time_id[j]<=ui->totime->dateTime().toString("yyyy-MM-ddThh:mm:ssZ")&&time_id[j]>=ui->fromtime->dateTime().toString("yyyy-MM-ddThh:mm:ssZ")){
                tmp++;
            }
        }
        charty.push_back(tmp);
    }

    set->append(charty);
    QBarSeries *series = new QBarSeries;
    series->append(set);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);  //设置标签显示的位置
    series->setLabelsVisible(true);  //设置数据标签可见
    QChart *chart = new QChart();
    if(flag){
        chart->setAnimationOptions(QChart::SeriesAnimations);
    }
    ui->rstchart->setChart(chart);
    ui->rstchart->setRubberBand(QChartView::HorizontalRubberBand);
    chart->addSeries(series);
    chart->setTitle("The Number Of Checking_ins");

    QStringList categories;
    for(int i=chartleft;i<chartright;i++){
        categories.append(QString::number(tree->pois[i]));
    }
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    axis->setTitleText("Location_id");
    QValueAxis *axisy = new QValueAxis;

    chart->addAxis(axis,Qt::AlignBottom);
    chart->addAxis(axisy,Qt::AlignLeft);
    series->attachAxis(axisy);
}


void MainWindow::drawchecking_ins_button(){
    chartleft=0;
    chartright=tree->pois.size()>10?10:tree->pois.size();
    drawchecking_ins(true);
}
void MainWindow::drawchecking_ins_left(){
    if(state==4&&tree->pois.size()>10&&chartright<tree->pois.size()){
        chartleft++;
        chartright++;
        drawchecking_ins(false);
    }
    else return;
}
void MainWindow::drawchecking_ins_right(){
    if(state!=4||chartleft==0) return;
        chartleft--;
        chartright--;
        drawchecking_ins(false);
}
