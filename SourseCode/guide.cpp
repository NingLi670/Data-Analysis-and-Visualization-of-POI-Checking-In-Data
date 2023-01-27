#include "guide.h"

LoadAndFilter::LoadAndFilter()
{
    setWindowTitle("Dialog");
    this->resize(230,230);
    tip=new QLabel("Select the datset file.");
    path=new QLineEdit;
    openbutton=new QPushButton("Open...");
    auto hlayout=new QHBoxLayout;
    hlayout->addWidget(path);
    hlayout->addWidget(openbutton);
    okbutton=new QPushButton("OK");
    auto hlayout2=new QHBoxLayout;
    hlayout2->addStretch(2);
    hlayout2->addWidget(okbutton);
    auto group=new QGroupBox("Fliter");
    auto user=new QCheckBox("user_id",group);
    user->setChecked(true);
    user->setEnabled(false);
    auto location=new QCheckBox("location_id",group);
    location->setChecked(true);
    location->setEnabled(false);
    time=new QCheckBox("time",group);
    time->setChecked(true);
    latitude=new QCheckBox("latitude and longitude",group);
    latitude->setChecked(true);
    auto subvbox=new QVBoxLayout(group);
    subvbox->addWidget(user);
    subvbox->addWidget(location);
    subvbox->addWidget(time);
    subvbox->addWidget(latitude);
    auto outlayout=new QVBoxLayout(this);
    outlayout->addWidget(tip);
    outlayout->addLayout(hlayout);
    outlayout->addWidget(group);
    //outlayout->addStretch(1);
    outlayout->addLayout(hlayout2);
    connect(openbutton,&QPushButton::clicked,this,&LoadAndFilter::OpenFile);
    //connect(path,&QLineEdit::editingFinished,this,&LoadAndFilter::CheckFile);
    connect(okbutton,&QPushButton::clicked,this,&LoadAndFilter::Finish);
}

void LoadAndFilter::OpenFile(){
    QString name=QFileDialog::getOpenFileName(this,"Select File","/","*.csv");
    path->setText(name);
}
void LoadAndFilter::CheckFile(){
    filename=path->text();
    qDebug()<<filename;
    if(filename ==""){
        QMessageBox error(QMessageBox::Critical,"Error","Please select a file first.",QMessageBox::Close);
        error.exec();
        return;
    }
    QDir dir=QDir::current();
    QFile file(dir.filePath(filename));
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox error(QMessageBox::Critical,"Error","Opening file failed.",QMessageBox::Close);
        error.exec();
        path->clear();
        return;
    }
    else {
        file.close();
        s.push_back(time->checkState());
        s.push_back(latitude->checkState());
        emit state(s);
        this->hide();
        emit call(filename);

    }
}
void LoadAndFilter::Finish(){
    CheckFile();
}
