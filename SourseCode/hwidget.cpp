#include "hwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <windows.h>
#include <stdio.h>
#include <ctime>
#include <set>

HWidget::HWidget(QWidget *& w)
{

    QDate tmp(2009,1,1);
    for(int i=0;i<731;i++){
        twoyear.push_back(tmp);
        tmp=tmp.addDays(1);
    }
    date=0;
    flag=false;
    setFixedSize(ImgWidth,ImgHeight);

    //data用alpha叠加
    _dataImg=QImage(ImgWidth,ImgHeight,QImage::Format_Alpha8);
    _dataImg.fill(Qt::transparent);
    //热力图通过alpha值查表
    _heatImg=QImage(ImgWidth,ImgHeight,QImage::Format_ARGB32);
    _heatImg.fill(Qt::transparent);

    //根据线性渐变色条得到颜色表
    QLinearGradient linear=QLinearGradient(QPoint(0,0),QPoint(255,0));
//    linear.setColorAt(0, Qt::blue);
//    linear.setColorAt(0.4, Qt::blue);
//    linear.setColorAt(0.5, Qt::cyan);
//    linear.setColorAt(0.6, Qt::green);
//    linear.setColorAt(0.8, Qt::yellow);
//    linear.setColorAt(0.95, Qt::red);
        linear.setColorAt(0, Qt::blue);
        linear.setColorAt(0.4, Qt::blue);
        linear.setColorAt(0.7, Qt::green);
        linear.setColorAt(0.98, Qt::yellow);
        linear.setColorAt(0.99, Qt::red);

    //把渐变色绘制到Img方便取颜色
    QImage img(256,1,QImage::Format_ARGB32);
    QPainter painter(&img);
    painter.fillRect(img.rect(),linear);

    //HeatAlpha为热力图整体透明度
    quint32 alpha=0;
    for(quint32 i=0;i<256;i++){
        //根据热力图透明度来计算颜色表的透明度
        alpha=HeatAlpha/255.0*i;
        //typedef unsigned int QRgb: format #AARRGGBB
        //颜色+透明度
        _colorList[i]=img.pixel(i,0)&0x00FFFFFF|(alpha<<24);
    }
}

void HWidget::addparent(MainWindow *_w){
    w=_w;
}

void HWidget::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    //绘制一个黑色网格，便于查看渐变色
    p.setPen(QPen(Qt::lightGray,1));

    QFontMetrics metrics = p.fontMetrics();
    p.drawRect(50,40,1080,540);
    p.drawLine(50,40,50,580);
    for(int i=40;i<=40+540;i+=90){
        p.drawLine(45,i,1130,i);
    }
    p.drawLine(50,580,1130,580);
    for(int i=50;i<=50+1080;i+=90){
        p.drawLine(i,40,i,585);
    }
    p.setPen(QPen(Qt::black,1));
    for(int i=-90;i<=90;i+=30){
        QString la=QString::fromStdString(std::to_string(i));
        la+=".0";
        if(i==0){la="  "+la;}
        p.drawText(15,(i+90)*3+45,la);
    }
    p.rotate(270);
    p.drawText(-300,10,"Latitude");
    p.rotate(90);
    for(int i=-18;i<=18;i+=3){
        QString lo=QString::fromStdString(std::to_string(i));
        lo+="0.0";
        if(i==0) lo=" "+lo+" ";
        p.drawText((i+18)*30+30,600,lo);
    }
    p.drawText(560,615,"Longitude");
    p.drawImage(0,0,_heatImg);
    QWidget::paintEvent(event);
}

void HWidget::mousePressEvent(QMouseEvent *event){
    if(flag){
        int i=date+1;
        while(i<w->day_time.size()&&w->day_time[i].size()==0) i++;
        if(i>=w->day_time.size()) return;
        date=i;
        draw_day(date);
        w->update_hlabel();
    }
}

void HWidget::draw_day(int day){
    _dataImg.fill(Qt::transparent);
    _heatImg.fill(Qt::transparent);
    _posList.clear();
    memset(_countTable,0,ImgWidth*ImgHeight*sizeof(int));
    _maxCount=0;

    std::map<int,int> m;
    for(int i=0;i<w->day_time[day].size();i++){
        m[w->location_id[w->day_time[day][i]]]++;
    }
    if(m.size()==0) return;
    std::map<int,int>::iterator p=m.begin();
    while(p!=m.end()){
        if((*p).second>_maxCount) _maxCount=(*p).second;
        Point pt{int((w->location_la_lo[(*p).first]->key[1]+180)*3+50),int((w->location_la_lo[(*p).first]->key[0]+90)*3+40),r,(*p).second};
        _posList.push_back(pt);
        _countTable[pt.posX+pt.posY*ImgWidth]+=pt.count;
        p++;
    }
    //_maxCount*=10;

    drawDataImg();
    drawHeatImg();
    update();
}

void HWidget::draw_time(QString &fromtime,QString &totime){
    //time_t start,end;
    //start=clock();
    _dataImg.fill(Qt::transparent);
    _heatImg.fill(Qt::transparent);
    _posList.clear();
    memset(_countTable,0,ImgWidth*ImgHeight*sizeof(int));
    _maxCount=0;
    std::map<int,int> m;
    for(int i=0;i<w->time_id.size();i++){
        if(w->time_id[i]>=fromtime&&w->time_id[i]<=totime){
            m[w->location_id[i]]++;
        }
    }
    if(m.size()==0){
        drawDataImg();
        drawHeatImg();
        update();
        return;
    }
    std::map<int,int>::iterator p=m.begin();
    while(p!=m.end()){
        if((*p).second>_maxCount) _maxCount=(*p).second;
        Point pt{int((w->location_la_lo[(*p).first]->key[1]+180)*3+50),int((w->location_la_lo[(*p).first]->key[0]+90)*3+40),r,(*p).second};
        _posList.push_back(pt);
        _countTable[pt.posX+pt.posY*ImgWidth]+=pt.count;
        p++;
    }
    _maxCount*=10;

    drawDataImg();
    drawHeatImg();
    update();
    //end=clock();
    //qDebug()<<"heatmap"<<(double)end-start;
}


void HWidget::drawDataImg()
{
    //重新绘制先清空
    _dataImg.fill(Qt::transparent);
    QPainter painter(&_dataImg);
    painter.setPen(Qt::transparent);
    //绘制点的部分可以调用drawpoint
    const double max_count=_maxCount;
    for(int i=0;i<_posList.count();i++)
    {
        const Point &pt=_posList.at(i);
        //以最大次数来计算该点的权重
        const uchar alpha=uchar(_countTable[pt.posX+pt.posY*ImgWidth]/max_count*255);
        QRadialGradient gradient(pt.posX,pt.posY,pt.radius);
        gradient.setColorAt(0,QColor(0,0,0,alpha));
        gradient.setColorAt(1,QColor(0,0,0,0));
        painter.setBrush(gradient);
        painter.drawEllipse(QPointF(pt.posX,pt.posY),pt.radius,pt.radius);
    }
}

void HWidget::drawHeatImg()
{
    //把alpha值转为颜色值
    for(int row=0;row<_dataImg.height();row++)
    {
        //dataimg QImage::Format_Alpha8，一个点1个字节
        const uchar *line_data=_dataImg.scanLine(row);
        //heatimg QImage::Format_ARGB32，一个点4个字节
        QRgb *line_heat=reinterpret_cast<QRgb*>(_heatImg.scanLine(row));
        for(int col=0;col<_dataImg.width();col++)
        {
            //根据alpha透明度从颜色表取颜色
            line_heat[col]=_colorList[line_data[col]];
        }
    }
}

void HWidget::clear()
{
    _dataImg.fill(Qt::transparent);
    _heatImg.fill(Qt::transparent);
    _posList.clear();
    //_maxCount=1;
    memset(_countTable,0,ImgWidth*ImgHeight*sizeof(int));
    update();
}

