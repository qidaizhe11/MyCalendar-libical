#ifndef ICALENDAR_H
#define ICALENDAR_H

#include <QWidget>
#include <libical/ical.h>
#include <QDate>
#include "context.h"

class ICalendar : public QWidget
{
    Q_OBJECT
public:
    explicit ICalendar(Context *context);

    // to get access to from the Calendar
    QVariant data(QDate date, int role = Qt::DisplayRole);
    void clearCalendar(QMap<QDate, QList<icalcomponent*> *>&calendar,
                       bool signal = true);
    void refreshRemote(QString);

signals:
    void dateChanged(); // when we get updated or refreshed

private slots:
    void parse(QString fulltext, QMap<QDate, QList<icalcomponent*> *>&calendar);

private:
    static void setICalendarProperties();

    Context *context;

    // we store the calendar as an associative array
    // for each date we have a list of events that occur on that day.
    // one for the local calendar stored in cyclisthome/calendar.ics
    // and one for a remote calendar we config in options
    QMap<QDate, QList<icalcomponent*> *> localCalendar;
    QMap<QDate, QList<icalcomponent*> *> remoteCalendar;
};

#endif // ICALENDAR_H
