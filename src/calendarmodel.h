#ifndef CALENDARMODEL_H
#define CALENDARMODEL_H

#include <QAbstractProxyModel>
#include <QItemDelegate>
#include <QTableView>

//-------------------------------------------------------------------------
//
// CalendarModel

class CalendarModel : public QAbstractProxyModel
{
    Q_OBJECT
public:
    enum UserRoles {
        DateStringRole = Qt::UserRole + 1,
        HeaderColorRole,
        CellColorRole,
        EventCountRole,
        FilenamesRole,
        DayRole
    };

    CalendarModel(QWidget *parent = 0);
    ~CalendarModel() {}

    void setMonth(int month, int year);
    int getMonth() { return month; }
    int getYear() { return year; }

    QDate date(QModelIndex index) const;

    void setSourceModel(QAbstractItemModel *sourceModel);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;

    // we override the standard version to make our virtual column zero
    // selectalbe. If we don't do that then the arrow keys don't work
    // since there are no valid rows to cursor up or down to.
    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant data(
        const QModelIndex &proxyIndex, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    bool setHeaderData(int , Qt::Orientation ,
                       const QVariant &, int = Qt::EditRole) {
        return true;
    }

    int columnCount(const QModelIndex &parent) const {
        return 7;
    }
    int rowCount(const QModelIndex &parent) const {
        return rows;
    }

    bool hasChildren(const QModelIndex &parent) const {
        return false;
    }

public slots:
    void setStale() { stale = true; }
//    void rideChange(RideItem *rideItem);
    void refresh();

private:
    int month, year;  // current month and year, default to this month
    bool stale;
    QVector<QDate> dates;  // dates for each cell from zero onwards
    int rows;

    QMap<QDate, QVector<int>*> dateToRows; // map a date to SQL rows

    QList<QString> columns; // what columns in the sql model
    int filenameIndex, durationIndex, dateIndex, textIndex, colorIndex;

};

//-------------------------------------------------------------------------
//
// CalendarDelegate

class CalendarDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    CalendarDelegate(QTableView *parent = 0);

    void createPainterPath(QPainterPath &empthPath, const QRect &fullItemRect,
            const int iRoundTop, const int iRoundBottom) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
};

#endif // CALENDARMODEL_H
