QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT +=charts

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dau.cpp \
    draw_c_info.cpp \
    draw_st_info.cpp \
    guide.cpp \
    hwidget.cpp \
    kdtree.cpp \
    main.cpp \
    mainwindow.cpp \
    manage_connects.cpp \
    readfile.cpp \
    removable_chartview.cpp \
    similarity.cpp

HEADERS += \
    dau.h \
    guide.h \
    hwidget.h \
    kdtree.h \
    mainwindow.h \
    readfile.h \
    removable_chartview.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
