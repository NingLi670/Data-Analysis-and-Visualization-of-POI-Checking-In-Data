#ifndef HWIDGET_H
#define HWIDGET_H
#include<QWidget>
#include<QFrame>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#include <QLinearGradient>
#include <QRadialGradient>
#include <QDateTime>

class MainWindow;

class HWidget :public QWidget
{
    Q_OBJECT
    struct Point
    {
        int posX;
        int posY;
        int radius;
        int count;
    };
public:
    HWidget(QWidget *&);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void drawDataImg();
    void drawHeatImg();

    //清空
    void clear();

    void addparent(MainWindow *_w);

    void draw_day(int day);
    void draw_time(QString &fromtime,QString &totime);

    int date;
    bool flag;
    QVector<QDate> twoyear;

private:
    static constexpr int HeatAlpha=200;
    //固定宽高的演示
    static constexpr int ImgWidth=630*2;
    static constexpr int ImgHeight=630;
    //最大权重
    int _maxCount=1;
    //权重统计表（把权重单独拿出来，就可以不用遍历数据点来计算了）
    //加()初始化为0
    int *_countTable=new int[ImgWidth*ImgHeight]();

    //数据点
    QList<Point> _posList;
    //绘制透明度
    QImage _dataImg;
    //最终热力图
    QImage _heatImg;
    //颜色表，透明度为0
    QRgb _colorList[256];
    MainWindow *w;
    int r=50;


};

#endif // HWIDGET_H
