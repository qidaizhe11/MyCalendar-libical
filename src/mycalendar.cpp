//
// This file has been deprecated!
//
// It would be remove not long.
//
#include <QCalendarWidget>
#include <QMultiMap>
#include <QPainter>
#include <QObject>
#include <QDate>
#include <QAbstractItemView>
#include <QSize>
#include <QTextCharFormat>
#include <QPen>

#include "mycalendar.h"
#include "mainwindow.h"

#include <algorithm>

MyCalendar::MyCalendar(MainWindow *parent) :
    QCalendarWidget(parent), mainWindow(parent)
{
    defaultColor = Qt::white;
    QFont font;
    font.setPointSize(font.pointSize() - 2);
    setFont(font);
    this->setFirstDayOfWeek(Qt::Monday);

    // setup workout codes
    configUpdate();

    connect(mainWindow, SIGNAL(configChanged()), this, SLOT(configUpdate()));
}

struct RideIter
{
    typedef std::random_access_iterator_tag iterator_category;
    typedef int difference_type;
};

void MyCalendar::paintCell(QPainter *painter, const QRect &rect,
                           const QDate &date) const
{
    painter->save();

    // fill in the text and color to some default
    QRect textRect;
    QPen pen(Qt::SolidLine);
    painter->setPen(pen);
    pen.capStyle(Qt::SquareCap);
    QFont font = painter->font();
    font.setPointSize(font.pointSize() - 2);
    painter->setFont(font);

    if (date == selectedDate()) {
        pen.setColor(Qt::black);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawRect(rect.x() + 1, rect.y() + 1,
                          rect.width() - 2, rect.height() - 2);
    }

    // display the date
    pen.setColor(QColor(0, 0, 0, 63));
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    painter->setPen(pen);
    QString textDate = QString::number(date.day());
    painter->drawRect(rect,
                      Qt::AlignHCenter | Qt::TextWordWrap | Qt::AlignVCenter,
                      textDate);

    painter->restore();
}
