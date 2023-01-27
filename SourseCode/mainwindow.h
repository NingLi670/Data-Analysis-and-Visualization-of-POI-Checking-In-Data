#ifndef MAINWINDOW_H
#define MAINWINDOW_H


//#include "stinfo.h"
#include "readfile.h"
#include "removable_chartview.h"
#include "kdtree.h"
#include "dau.h"
#include "guide.h"

#include <QMainWindow>
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QThread>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QPieSeries>
#include <QPieSlice>
#include <QMouseEvent>
#include <QDoubleValidator>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QSplineSeries>

#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>

#include <QLinearGradient>
#include <QRadialGradient>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openfirst();
    void display(QString name);
    void loaddata();
    void createstinfo();
    void changepro(int v);
    void threadreadfile();
    void drawtopten(QString);
    void drawchecking_in();
    void drawchecking_ins(bool flag);
    void drawchecking_ins_button();
    void drawchecking_ins_left();
    void drawchecking_ins_right();
    void draw_user_compare();
    int get_time_position(QString &time);
    void draw_poi_compare();
    void manage_connects();
    void stinfo_display_slot();
    void oneuser_slot();
    void users_slot();
    void onepoi_slot();
    void pois_slot();
    void update_parameters();
    void move_left_slot();
    void move_right_slot();
    void draw_dau();
    void draw_dau_bar();
    void cinfo_display_slot();
    void cuser_slot();
    void cpoi_slot();
    void cbar_slot();
    void cline_slot();
    void simi_displayn_slot();
    void simi_displayg_slot();
    void heat_display_slot();
    void hday_slot();
    void htime_slot();
    void prepare_draw_dau();
    void update_ctotime();
    void update_hlabel();
    QString similar(int usera,int userb);
    int similar_hour(int usera,int userb,bool flag);
    double similar_week(int usera,int userb,bool flag);
    QVector<double> similar_poi(int usera,int userb);

    void guide_time_slot();
    void guide_la_slot(QVector<Qt::CheckState> state);

    QString filename;
    QVector<QString> data;
    QVector<int> user_id;
    QVector<int> location_id;
    QVector<QString> time_id;
    QVector<double> la_id;
    QVector<double> lo_id;
    QVector<int> user_pos;
    QVector<Item *> location_la_lo;
    kdtree *tree;
    QVector<QVector<int>> location_count;//location_id_pos
    QVector<QVector<int>> day_time;
private:
    int count;
    int chartleft,chartright;
    //removable_chartview *stchart;
    QStringList year_and_month;
    int state;
    QChart *chart;
    DAU *dau_data;

    QLineSeries *lineSeriesa;
    QLineSeries *lineSeriesb;
    QLineSeries *lineSeriesc;
signals:
    void startthread(QThread::Priority);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
