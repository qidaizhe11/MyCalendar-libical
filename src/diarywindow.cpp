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

    QIcon preIcon(":images/toolbar/back_alt.png");
    QIcon nextIcon(":images/toolbar/forward_alt.png");
    next = new QPushButton(nextIcon, "", this);
    prev = new QPushButton(prevIcon, "", this);

    controls->addWidget(prev);
    controls->addWidget(next);
    controls->addStretch();
    controls->addWidget(title, Qt::AlignCenter | Qt::AlignVCenter);
    controls->addStretch();

    vlayout->addLayout(controls);

    // monthly view via QCalendarWidget
    // TODO:

    monthlyView = new QTableView(this);
//    monthlyView->setItemDelegate();
    monthlyView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    monthlyView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    monthlyView->viewport()->installEventFilter(this);
    monthlyView->setGridStyle(Qt::DotLine);
    monthlyView->setFrameStyle(QFrame::NoFrame);

    allViews = new QStackedWidget(this);
    allViews->addWidget(monthlyView);
    allViews->setCurrentIndex(0);

    vlayout->addWidget(allViews);

    connect(next, SIGNAL(clicked()), this, SLOT(nextClicked()));
    connect(prev, SIGNAL(clicked()), this, SLOT(prevClicked()));
}

//
// slots
//

void DiaryWindow::prevClicked()
{

}

void DiaryWindow::nextClicked()
{

}
