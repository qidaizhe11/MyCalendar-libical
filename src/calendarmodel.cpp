
#include "calendarmodel.h"
#include <QtWidgets>

//-------------------------------------------------------------------------
//
// CalendarModel

CalendarModel::CalendarModel(QWidget *parent)
    : QAbstractProxyModeld(parent)
{
    setParent(parent);

    stale = true;
    QDate today = QDate::currentDate();
    setMonth(today.month(), today.year());
}

//
// public
//

void CalendarModel::setMonth(int month, int year)
{
    if (stale || this->month() != month || this->year != year) {
        stale = false;
        this->month = month;
        this->year = year;
        refresh();
    }
}

QDate CalendarModel::date(QModelIndex index) const
{
    if (index.row()*7 + index.column() < dates.count()) {
        return dates[index.row()*7 + index.column()];
    }
    else {
        return QDate();
    }
}

void CalendarModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    QAbstractProxyModel::setSourceModel(sourceModel);
    connect(sourceModel, SIGNAL(modelReset()), this, SLOT(refresh()));
    connect(sourceModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(refresh()));
    connect(sourceModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(refresh()));
    connect(sourceModel, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SLOT(refresh()));
    connect(sourceModel, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(refresh()));
    // TODO:
    //connect(context->athlete->rideCalendar
    refresh();
}

void CalendarModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column, (void *)NULL);
}

void CalendarModel::parent(const QModelIndex &child) const
{
    // parent should be encoded in the child if we supplied it,
    // if we didn't then return duffer
    if (child == QModelIndex() || index.internalPointer() == NULL) {
        return QModelIndex();
    }
    else if (child.column()) {
        return QModelIndex();
    }
    else {
        return *static_cast<QModelIndex*>(index.internalPointer());
    }
}

QModelIndex CalendarModel::mapToSource(const QModelIndex &proxyIndex) const
{
    return sourceModel()->index(proxyIndex.row(), proxyIndex.column(),
                                QModelIndex);
}

QModelIndex CalendarModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    return createIndex(sourceIndex.row(), sourceIndex.column(), (void *)NULL);
}

Qt::ItemFlags CalendarModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

//
// slots
//

void CalendarModel::refresh()
{
    if (!sourceModel()) return; // no model set!

    QDate first = QDate(year, month, 1);
    // Date array
    // how many days in this month?
    int monthDays = first.daysTo(first.addMonths(1));
    // date in cell 0,0
    QDate firstDate = first.addDays((first.dayOfWeek()-1)*-1);
    int ndays = firstDate.daysTo(QDate(year, month, monthDays));
    ndays += 7 - ndays%7;

    dates.clear();
    dates.resize(ndays);
    for (int i = 0; i < ndays; ++i) {
        dates[i] = firstDate.addDays(i);
    }

    rows = ndays / 7;

    reset();
}
