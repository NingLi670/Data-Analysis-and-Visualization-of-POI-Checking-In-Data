#include "mainwindow.h"
#include "guide.h"
//#include <QGerRoute>


#include <QApplication>
#include <QThread>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    LoadAndFilter l;
    l.show();

    a.connect(&l,&LoadAndFilter::call,&w,&MainWindow::display);
    a.connect(&l,&LoadAndFilter::state,&w,&MainWindow::guide_la_slot);

    return a.exec();
}
