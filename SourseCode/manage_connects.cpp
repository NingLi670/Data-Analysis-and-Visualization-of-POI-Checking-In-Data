#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::manage_connects(){

    connect(ui->userradio,&QRadioButton::clicked,this,&MainWindow::oneuser_slot);
    connect(ui->usersradio,&QRadioButton::clicked,this,&MainWindow::users_slot);
    connect(ui->poiradio,&QRadioButton::clicked,this,&MainWindow::onepoi_slot);
    connect(ui->poisradio,&QRadioButton::clicked,this,&MainWindow::pois_slot);
    connect(ui->rstchart,&removable_chartview::move_left,this,&MainWindow::drawchecking_ins_left);
    connect(ui->rstchart,&removable_chartview::move_right,this,&MainWindow::drawchecking_ins_right);
    connect(ui->display,&QPushButton::clicked,this,&MainWindow::stinfo_display_slot);


    connect(ui->cuser,&QRadioButton::clicked,this,&MainWindow::cuser_slot);
    connect(ui->cpoi,&QRadioButton::clicked,this,&MainWindow::cpoi_slot);
    connect(ui->line,&QRadioButton::clicked,this,&MainWindow::cline_slot);
    connect(ui->bar,&QRadioButton::clicked,this,&MainWindow::cbar_slot);
    connect(ui->cfromtime,&QDateEdit::dateChanged,this,&MainWindow::update_ctotime);
    connect(ui->cdisplay,&QPushButton::clicked,this,&MainWindow::cinfo_display_slot);

    connect(ui->hday,&QRadioButton::clicked,this,&MainWindow::hday_slot);
    connect(ui->htime,&QRadioButton::clicked,this,&MainWindow::htime_slot);
    connect(ui->hdisplay,&QPushButton::clicked,this,&MainWindow::heat_display_slot);

    connect(ui->displayn,&QPushButton::clicked,this,&MainWindow::simi_displayn_slot);
    connect(ui->displayg,&QPushButton::clicked,this,&MainWindow::simi_displayg_slot);
}

void MainWindow::heat_display_slot(){
    if(ui->hday->isChecked()){
        int i=0;
        while(i<day_time.size()&&day_time[i].size()==0) i++;
        if(i<day_time.size()) ui->widget->date=i;
        ui->widget->draw_day(i);
        update_hlabel();
    }
    else if(ui->htime->isChecked()){

        QString timea=ui->hfromtime->dateTime().toString("yyyy-MM-ddThh:mm:ssZ");
        QString timeb=ui->htotime->dateTime().toString("yyyy-MM-ddThh:mm:ssZ");
        if(timea>timeb){
            QMessageBox error(QMessageBox::Critical,"Error","The FromTime should not be larger than the ToTime.",QMessageBox::Close);
            error.exec();
            return;
        }
        ui->widget->draw_time(timea,timeb);
    }
}

void MainWindow::hday_slot(){
    ui->hflabel->setEnabled(false);
    ui->htlabel->setEnabled(false);
    ui->hfromtime->setEnabled(false);
    ui->htotime->setEnabled(false);
    ui->hlabel->setEnabled(true);
    ui->widget->flag=true;
}
void MainWindow::htime_slot(){
    ui->widget->flag=false;
    ui->hflabel->setEnabled(true);
    ui->htlabel->setEnabled(true);
    ui->hfromtime->setEnabled(true);
    ui->htotime->setEnabled(true);
    ui->hlabel->setEnabled(false);
}

void MainWindow::update_hlabel(){
    ui->hlabel->setText("Current Time:"+ui->widget->twoyear[ui->widget->date].toString("yyyy-MM-dd"));
}

void MainWindow::simi_displayn_slot(){
    ui->slabel->setText(similar(ui->susera->value(),ui->suserb->value()));
}
void MainWindow::simi_displayg_slot(){
    if(ui->sday->isChecked()){
        similar_hour(ui->susera->value(),ui->suserb->value(),true);
    }
    else if(ui->sweek->isChecked()){
        similar_week(ui->susera->value(),ui->suserb->value(),true);
    }
}
void MainWindow::cinfo_display_slot(){
    if(ui->cuser->isChecked()){
        draw_user_compare();
    }
    else if(ui->cpoi->isChecked()){
        if(ui->line->isChecked()){
            draw_dau();
        }
        else if(ui->bar->isChecked()){
            if(ui->cfromtime->date()>ui->ctotime->date()){
                QMessageBox error(QMessageBox::Critical,"Error","The FromTime should not be larger than the ToTime.",QMessageBox::Close);
                error.exec();
                return;
            }
            else draw_dau_bar();
        }
    }
}

void MainWindow::update_ctotime(){
    ui->ctotime->setDate(ui->cfromtime->date().addDays(9));
}

void MainWindow::cuser_slot(){
    ui->cusera->setEnabled(true);
    ui->cuserb->setEnabled(true);
    ui->cpoia->setEnabled(false);
    ui->cpoib->setEnabled(false);
    ui->line->setChecked(false);
    ui->bar->setChecked(true);
    ui->line->setEnabled(false);
    ui->cfromlabel->setVisible(false);
    ui->ctolabel->setVisible(false);
    ui->cfromtime->setVisible(false);
    ui->ctotime->setVisible(false);

}
void MainWindow::cpoi_slot(){
    ui->cusera->setEnabled(false);
    ui->cuserb->setEnabled(false);
    ui->cpoia->setEnabled(true);
    ui->cpoib->setEnabled(true);
    if(ui->bar->isChecked()){
    ui->cfromlabel->setVisible(true);
    ui->ctolabel->setVisible(true);
    ui->cfromtime->setVisible(true);
    ui->ctotime->setVisible(true);}
    ui->line->setEnabled(true);
}
void MainWindow::cbar_slot(){
    if(ui->cpoi->isChecked()){
        ui->cfromlabel->setVisible(true);
        ui->ctolabel->setVisible(true);
        ui->cfromtime->setVisible(true);
        ui->ctotime->setVisible(true);}
}

void MainWindow::cline_slot(){
    ui->cfromlabel->setVisible(false);
    ui->ctolabel->setVisible(false);
    ui->cfromtime->setVisible(false);
    ui->ctotime->setVisible(false);
}

void MainWindow::stinfo_display_slot(){
        if(ui->userradio->isChecked()){
            if(ui->fromtime->date()>ui->totime->date()){
                QMessageBox error(QMessageBox::Critical,"Error","The FromTime should not be larger than the ToTime.",QMessageBox::Close);
                error.exec();
                return;
            }
            state=1;
            ui->rstchart->setVisible(false);
            ui->stchart->setVisible(true);
            drawtopten(QString::fromStdString(std::to_string(ui->userspin->value())));
        }
        else if(ui->usersradio->isChecked()){
            if(ui->fromtime->date()>ui->totime->date()){
                QMessageBox error(QMessageBox::Critical,"Error","The FromTime should not be larger than the ToTime.",QMessageBox::Close);
                error.exec();
                return;
            }
            if(ui->usersedit->text()==""){
                QMessageBox error(QMessageBox::Critical,"Error","Please Enter User_ids first.",QMessageBox::Close);
                error.exec();
                return;
            }
            state=2;
            ui->rstchart->setVisible(false);
            ui->stchart->setVisible(true);
            drawtopten(ui->usersedit->text());
        }
        else if(ui->poiradio->isChecked()){
            state=3;
            ui->rstchart->setVisible(false);
            ui->stchart->setVisible(true);
            drawchecking_in();
        }
        else if(ui->poisradio->isChecked()){
            if(ui->fromtime->date()>ui->totime->date()){
                QMessageBox error(QMessageBox::Critical,"Error","The FromTime should not be larger than the ToTime.",QMessageBox::Close);
                error.exec();
                return;
            }
            if(ui->lamin->text().toDouble()>ui->lamax->text().toDouble()||ui->lamin->text().toDouble()<-90||ui->lamax->text().toDouble()>90){
                QMessageBox error(QMessageBox::Critical,"Error","Latitude range error.",QMessageBox::Close);
                error.exec();
                return;
            }
            else if(ui->lomin->text().toDouble()>ui->lomax->text().toDouble()||ui->lomin->text().toDouble()<-180||ui->lomax->text().toDouble()>180){
                QMessageBox error(QMessageBox::Critical,"Error","Longitude range error.",QMessageBox::Close);
                error.exec();
                return;
            }
            else {
            state=4;
            double min[2],max[2];
            min[0]=ui->lamin->text().toDouble();
            max[0]=ui->lamax->text().toDouble();
            min[1]=ui->lomin->text().toDouble();
            max[1]=ui->lomax->text().toDouble();
            tree->getpois(min,max);
            //qDebug()<<tree->pois;
            ui->rstchart->setVisible(true);
            ui->stchart->setVisible(false);
            drawchecking_ins_button();
            }
        }
    }


void MainWindow::oneuser_slot(){
    ui->sttimegroup->setEnabled(true);

}
void MainWindow::users_slot(){
    ui->sttimegroup->setEnabled(true);

}
void MainWindow::onepoi_slot(){
    ui->sttimegroup->setEnabled(false);

}
void MainWindow::pois_slot(){
    ui->sttimegroup->setEnabled(true);

}






