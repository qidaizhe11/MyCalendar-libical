//#include "mainwindow.h"
#include "diarywindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DiaryWindow w;
    w.show();

    return a.exec();
}
