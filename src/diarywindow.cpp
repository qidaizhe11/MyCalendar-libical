#include "diarywindow.h"
#include <QtWidgets>

DiaryWindow::DiaryWindow(QWidget *parent) :
    QWidget(parent), active(false)
{
    QVBoxLayout *vlayout = new QVBoxLayout(this);

    //controls
    QHBoxLayout *controls = new QHBoxLayout;
    QFont bold;
    bold.setPointSize(14);
    bold.setWeight(QFont::Bold);
    title = new QLabel("", this);
    title->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    title->setFont(bold);
}

