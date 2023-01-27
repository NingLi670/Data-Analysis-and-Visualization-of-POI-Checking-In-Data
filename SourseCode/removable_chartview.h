#ifndef REMOVABLE_CHARTVIEW_H
#define REMOVABLE_CHARTVIEW_H

#include <QChartView>
#include <QMouseEvent>

class removable_chartview : public QChartView
{
    Q_OBJECT
public:
    removable_chartview(QWidget *parent = Q_NULLPTR);
private:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    QPoint last_mouse_position_;
signals:
    void move_right();
    void move_left();
};

#endif // REMOVABLE_CHARTVIEW_H
