#include "removable_chartview.h"

removable_chartview::removable_chartview(QWidget *parent)
{
}

void removable_chartview::mousePressEvent(QMouseEvent* event){
    if(event->button()==Qt::LeftButton){
        //stchart->setVisible(false);

        last_mouse_position_ = event->globalPos();
        setCursor(Qt::ClosedHandCursor);
    }
}
void removable_chartview::mouseMoveEvent(QMouseEvent* event){
    if (!event->buttons().testFlag(Qt::LeftButton)){
        setCursor(Qt::ArrowCursor);
        return;
    }

    const QPoint position =event->globalPos();
    if(position.x()-last_mouse_position_.x()>=50){
        last_mouse_position_ = event->globalPos();
        emit move_right();
    }
    if(last_mouse_position_.x()-position.x()>=50){
        last_mouse_position_ = event->globalPos();
        emit move_left();
    }
}
