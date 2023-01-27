#ifndef GUIDE_H
#define GUIDE_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QFile>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>



class LoadAndFilter: public QDialog
{
    Q_OBJECT


public:
    LoadAndFilter();
    void OpenFile();
    void CheckFile();
    void Finish();
    QString filename;
    QCheckBox *time;
    QCheckBox *latitude;
    QPushButton *okbutton;
    QVector<Qt::CheckState> s;

private:
    QLabel *tip;
    QLineEdit *path;
    QPushButton *openbutton;
    QGroupBox *filter;
signals:
    void call(QString name);
    void state(QVector<Qt::CheckState> s);
};


#endif // GUIDE_H
