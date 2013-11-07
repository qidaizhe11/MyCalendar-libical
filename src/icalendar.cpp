#include "icalendar.h"
#include <QtWidgets>

ICalendar::ICalendar(QWidget *parent) :
    QWidget(parent)
{
}

void ICalendar::setICalendarProperties()
{
}

// convert property to a string
static QString propertyToString(icalproperty *p)
{
    if (p) {
        icalvalue *v = icalproperty_get_value(p);
        QString converted(icalvalue_as_ical_string(v));

        // some special cahracters are escaped in the text
        converted.replace("\\n", "\n");
        converted.replace("\\;", ";");

        return converted;
    }
    else {
        return QString("");
    }
}

// convert property to a DateTime
static QDateTime propertyToDate(icalproperty *p)
{
    if (p) {
        icalvalue *v = icalproperty_get_value(p);
        struct icaltimetype date = icalvalue_get_datetime(v);
        QDateTime when(QDate(date.year, date.month, date.day),
                       QTime(date.hour, date.minute, date.second));
        return when;
    }
    else {
        return QDateTime();
    }
}

void ICalendar::parse(QString fulltext,
                      QMap<QDate, QList<icalcomponent *> *> &calendar)
{
    // parse the contents using libical
    icalcomponent *root = icalparser_parse_string(
                fulltext.toLatin1().constData());

    clearCalendar(calendar, false);

    if (root) {
        // iterate over events (not interested in the rest for now)
        for (icalcomponent *event = icalcomponent_get_first_component(
                 root, ICAL_VEVENT_COMPONENT);
             event != NULL;
             event = icalcomponent_get_next_component(
                 root, ICAL_VEVENT_COMPONENT))
        {
            // get start date..
            icalproperty *date = icalcomponent_get_first_property(
                        event, ICAL_DTSTART_PROPERTY);
            if (date) { // ignore events with no date
                QDate startDate = propertyToDate(date).date();
                QList<icalcomponent*> *events = calendar.vaule(startDate, NULL);
                if (!events) {
                    events = new QList<icalcomponent*>;
                    calendar.insert(startDate, events);
                }
                events->append(event);
            }
        } // end for
    } // end if

    emit dateChanged();
}

void ICalendar::clearCalendar(QMap<QDate, QList<icalcomponent *> *> &calendar,
                              bool signal)
{
    QMapIterator<QDate, QList<icalcomponent*> *>i(calendar);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }
    calendar.clear();

    if (signal)
        emit dateChanged();
}

// for models to pass straight through to access and set the calendar data
QVariant ICalendar::data(QDate date, int role)
{
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole: // returns a list for the given date
    {
        QStringList strings;

        // local
        QList<icalcomponent*> *p = localCalendar.value(date, NULL);
        if (p) {
            foreach (icalcomponent *event, *p) {
                QString desc;
                icalproperty *summary = icalcomponent_get_first_property(
                            event, ICAL_SUMMARY_PROPERTY);
                desc = propertyToString(summary);
                icalproperty *property = icalcomponent_get_first_property(
                            event, ICAL_DESCRIPTION_PROPERTY);
                desc += " ";
                dsec += propertyToString(property);
                strings << desc;
            }
        }

        // remote
        p = remoteCalendar.value(date, NULL);
        if (p) {
            foreach (icalcomponent *event, *p) {
                QString desc;
                icalproperty *summary = icalcomponent_get_first_property(
                            event, ICAL_SUMMARY_PROPERTY);
                desc = propertyToString(summary);
                icalproperty *property = icalcomponent_get_first_property(
                            event, ICAL_DESCRIPTION_PROPERTY);
                desc += " ";
                dsec += propertyToString(property);
                strings << desc;
            }
        }

        return strings;
    }
        break;
    default:
        break;
    } // end switch

    return QVariant();
}
