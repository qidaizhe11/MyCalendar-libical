
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
    // TODO: connect(context->athlete->rideCalendar
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

QVariant CalendarModel::data(const QModelIndex &proxyIndex, int role) const
{
    if (!proxyIndex.isValid())
        return QVariant();

    QVariant returning;

    switch (role) {
    case Qt::BackgroundRole:
    {
        QList<QColor> colors;
        QVector<int> *arr = dateToRows.value(date(proxyIndex), NULL);
        if (arr) {
            foreach (int i, *arr) {
                // TODO: if its the current ride
                //

                colors << QColor(sourceModel()->data(
                            index(i, colorIndex, QModelIndex())).toString());
            }
        }

        return QVariant::fromValue<QList<QColor> >(colors);
        break;
    }

    case Qt::ForegroundRole:
    {
        QList<QColor> colors;
        QVector<int> *arr = dateToRows.value(date(proxyIndex), NULL);
        if (arr) {
            foreach (int i, *arr) {
                QString fileName = sourceModel()->data(
                            index(i, filenameIndex, QModelIndex())).toString();
                colors << QColor(Qt::black);
            }
        }

        return QVariant::fromValue<QList<QColor> >(colors);
        break;
    }

    case Qt::EditRole:
    case Qt::DisplayRole:  // returns the string to display
    {
        QVector<int> *arr = dateToRows.value(date(proxyIndex), NULL);
        QStringList strings;
        if (arr) {
            foreach (int i, *arr) {
                strings << sourceModel()->data(
                               index(i, textIndex, QModelIndex())).toString();
            }
        }

        return strings;
        break;
    }

        // how should this date be described in the cell heading
    case DateStringRole:
    {
        QDate today = date(proxyIndex);
        if (today.month() != month &&
            (today.addDays(1).month() == month || today.addDays(-1).month()
                == month)) {
            return QString("%1 %2").arg(today.day()).arg(
                        QDate::shortMonthName(today.month()));
        }
        else {
            return QString("%1").arg(today.day());
        }
        break;
    }
    case DayRole:
    {
        QDate today = date(proxyIndex);
        if (today.month() != month &&
            (today.addDays(1).month() == month || today.addDays(-1).month()
                == month))
            return "";
        else
            return QString("%1").arg(today.day());
        break;
    }
    default:
        return QVariant();
        break;
    }
    return QVariant();
}

QVariant CalendarModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            return QDate::shortDayName(section + 1);
        }
        else {
            return QString("%1").arg(date(index(section, 0)).weekNumber());
        }
    }
    return QVariant();
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

//-------------------------------------------------------------------------
//
// CalendarDelegate

CalendarDelegate::CalendarDelegate(QTableView *parent)
    : QItemDelegate(parent)
{
}

void CalendarDelegate::createPainterPath(
        QPainterPath &emptyPath, const QRect &fullItemRect,
        const int iRoundTop, const int iRoundBottom) const
{
    emptyPath = QPainterPath();
    bool bRoundTop = iRoundTop > 0;
    bool bRoundBottom = iRoundBottom > 0;

    if (bRoundTop) {
        emptyPath.moveTo(fullItemRect.topLeft() + QPoint(0, iRoundTop));
        emptyPath.quadTo(fullItemRect.topLeft(), fullItemRect.topLeft() +
                         QPoint(iRoundTop, 0));
    }
    else {
        emptyPath.moveTo(fullItemRect.topLeft());
    }

    emptyPath.lineTo(fullItemRect.topRight() - QPoint(iRoundTop, 0));

    if (bRoundTop)
        emptyPath.quadTo(fullItemRect.topRight(),
                         fullItemRect.topRight() + QPoint(0, iRoundTop));

    emptyPath.lineTo(fullItemRect.bottomRight() - QPoint(0, iRoundBottom));

    if (bRountBottom)
        emptyPath.quadTo(fullItemRect.bottomRight(),
                         fullItemRect.bottomRight() - QPoint(iRoundBottom, 0));

    emptyPath.lineTo(fullItemRect.bottomLeft() + QPoint(iRoundBottom, 0));

    if (bRountBottom)
        emptyPath.quadTo(fullItemRect.bottomLeft(),
                         fullItemRect.bottomLeft() - QPoint(0, iRoundBottom));

    emptyPath.closeSubpath();
}

void CalendarDelegate::paint(QPainter *painter,
        const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    // font
    QVariant vfont = index.data(Qt::FontRole);
    painter->setFont(vfont.value<QFont>());

    // cell decoration
    QColor bg = index.data(CalendarModel::CellColorRole).value<QColor>();
    QColor hg = index.data(CalendarModel::HeaderColorRole).value<QColor>();

    // if selected then use selection color anyway
    if (option.state & QStyle::State_Selected)
        bg = option.palette.highlight().color();
    painter->fillRect(option.rect, bg);

    // still paint header
    QRect hd(option.rect.x(), option.rect.y(), option.rect.width(), 15);
    painter->fillRect(hd, hg);

    // date...
    QString dateString = index.data(CalendarModel::DateStringRole).toString();
    QTextOption textOption(Qt::AlignRight);
    painter->drawText(hd, dateString, textOption);

    // text
    QStringList texts = index.data(Qt::DisplayRole).toStringList();
    QList<QColor> colors =
            index.data(Qt::BackgroundRole).value<QList<QColor> >();
    QList<QColor> pens =
            index.data(Qt::ForegroundRole).value<QList<QColor> >();
    if (texts.count()) {
        int height = (option.rect.height()-21) / texts.count();
        int y = option.rect.y() + 17;
        int i = 0;
        foreach (QString text, texts) {
            QRect bd(option.rect.x()+2, y, option.rect.width() - 4, height);
            y += height + 2;

            QPainterPath cachePath;
            createPainterPath(cachePath, bd, 4, 4);

            QPen pen;
            QColor color = pens[i];
            pen.setColor(color);

            // if not black use it for border
            if (color != QColor(Qt::black))
                painter->setPen(pen);
            else
                painter->setPen(Qt::NoPen);
            QColor fillColor = colors[i++];
            fillColor.setAlpha(200);
            painter->setBrush(fillColor);
            painter->setRenderHint(QPainter::Antialiasing);
            painter->drawPath(cachePath);

            // text always needs a pen
            painter->setPen(pen);
            painter->drawText(bd, text);
        } // end foreach
    } // end if

    painter->restore();
}
