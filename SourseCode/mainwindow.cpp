#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->rstchart->setVisible(false);
    dau_data=new DAU(this);
    lineSeriesa=new QLineSeries;
    lineSeriesb=new QLineSeries;
    chart=new QChart;
    state=0;
    day_time.resize(731);
    location_la_lo.resize(50835);
    location_count.resize(50835);
    year_and_month<<"2009-01-01T00:00:00Z"<<"2009-02-01T00:00:00Z"<<"2009-03-01T00:00:00Z"<<"2009-04-01T00:00:00Z"
                  <<"2009-05-01T00:00:00Z"<<"2009-06-01T00:00:00Z"<<"2009-07-01T00:00:00Z"<<"2009-08-01T00:00:00Z"
                  <<"2009-09-01T00:00:00Z"<<"2009-10-01T00:00:00Z"<<"2009-11-01T00:00:00Z"<<"2009-12-01T00:00:00Z"
                  <<"2010-01-01T00:00:00Z"<<"2010-02-01T00:00:00Z"<<"2010-03-01T00:00:00Z"<<"2010-04-01T00:00:00Z"
                  <<"2010-05-01T00:00:00Z"<<"2010-06-01T00:00:00Z"<<"2010-07-01T00:00:00Z"<<"2010-08-01T00:00:00Z"
                  <<"2010-09-01T00:00:00Z"<<"2010-10-01T00:00:00Z"<<"2010-11-01T00:00:00Z"<<"2010-12-01T00:00:00Z"
                  <<"2011-01-01T00:00:00Z";
    ui->cfromlabel->setVisible(false);
    ui->ctolabel->setVisible(false);
    ui->cfromtime->setVisible(false);
    ui->ctotime->setVisible(false);
    ui->userspin->setRange(0,22324);
    ui->usersedit->setPlaceholderText("Format Example: 1,6-10,88,92,120-167");
    ui->cusera->setRange(0,22324);
    ui->cuserb->setRange(0,22324);
    ui->susera->setRange(0,22324);
    ui->suserb->setRange(0,22324);
    ui->poispin->setRange(0,50834);
    ui->cpoia->setRange(0,50834);
    ui->cpoib->setRange(0,50834);
    ui->progress->setRange(0,100);
    ui->progress->setValue(0);
    ui->display->setEnabled(false);
    ui->cdisplay->setEnabled(false);
    ui->displayg->setEnabled(false);
    ui->displayn->setEnabled(false);
    ui->hdisplay->setEnabled(false);
    ui->progress->setFormat("Loading Data: %p%");
    ui->progress->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QDoubleValidator *loValidator =  new  QDoubleValidator(-180.0, 180.0 ,5);
    loValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->lomin->setValidator(loValidator);
    ui->lomin->setText("0.0");
    ui->lomax->setValidator(loValidator);
    ui->lomax->setText("180.0");
    QDoubleValidator *laValidator =  new  QDoubleValidator(-90.0, 90.0 ,5);
    laValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->lamin->setValidator(laValidator);
    ui->lamin->setText("0.0");
    ui->lamax->setValidator(laValidator);
    ui->lamax->setText("90.0");

    ui->fromtime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->totime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->hfromtime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->htotime->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->cfromtime->setDisplayFormat("yyyy-MM-dd");
    ui->ctotime->setDisplayFormat("yyyy-MM-dd");
    QFont font;
    font.setPointSize(11);//字体大小
    ui->slabel->setFont(font);//其他控件一样
    ui->slabel->setText("The similarity between two users will be shown here.");
    ui->hlabel->setText("Current Time:yyyy-MM-dd");
    manage_connects();

    ui->hflabel->setEnabled(false);
    ui->htlabel->setEnabled(false);
    ui->hfromtime->setEnabled(false);
    ui->htotime->setEnabled(false);
    ui->hlabel->setEnabled(false);

    //bgroup1->

}



void MainWindow::display(QString name){
    filename=name;
    show();
    threadreadfile();
}

void MainWindow::changepro(int v){
    ui->progress->setValue(v);
}

void MainWindow::threadreadfile(){
    auto thread=new QThread;
    auto data=new readfile(this);
    connect(thread,&QThread::started,data,&readfile::loaddata);
    connect(data,&readfile::workdone,thread,&QThread::quit);
    connect(data,&readfile::workdone,this,&MainWindow::update_parameters);
    data->moveToThread(thread);
    //connect(this,&MainWindow::startthread,thread,&QThread::start);
    connect(data,&readfile::pro,this,&MainWindow::changepro);
    thread->start();
}
void MainWindow::update_parameters(){
    ui->display->setEnabled(true);
    ui->cdisplay->setEnabled(true);
    ui->displayg->setEnabled(true);
    ui->displayn->setEnabled(true);
    ui->hdisplay->setEnabled(true);
    ui->progress->setVisible(false);
    ui->widget->addparent(this);
}

void MainWindow::guide_la_slot(QVector<Qt::CheckState> state){
    if(state[0]==Qt::Unchecked){
        ui->sttimegroup->setVisible(false);
        ui->poiradio->setEnabled(false);
        ui->poispin->setEnabled(false);

        ui->tabWidget->setTabEnabled(1,false);
        ui->tabWidget->setTabEnabled(2,false);
        ui->tabWidget->setTabEnabled(3,false);
    }
    if(state[1]==Qt::Unchecked){
    ui->tabWidget->setTabEnabled(2,false);
    ui->poisradio->setEnabled(false);
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

