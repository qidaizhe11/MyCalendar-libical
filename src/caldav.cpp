#include "caldav.h"
#include <libical/ical.h>
#include <QUuid>
#include <QDateTime>

CalDAV::CalDAV(QObject *parent) :
    QObject(parent)
{
}

// utility function to create a VCALENDAR from a single RideItem
static icalcomponent *createEvent(RideItem *rideItem)
{
    // calendar
    icalcomponent *root = icalcomponent_new(ICAL_VCALENDAR_COMPONENT);

    // calendar version
    icalproperty *version = icalproperty_new_version("2.0");
    icalcomponent_add_property(root, version);

    icalcomponent *event = icalcomponent_new(ICAL_VEVENT_COMPONENT);

    //
    // Unique ID
    //
    QString id = rideItem->ride()->id();
    if (id == "") {
        id = QUuid::createUuid().toString() + "@" + "goldencheetah.org";
    }
    icalproperty *uid = icalproperty_new_uid(id.toLatin1());
    icalcomponent_add_property(event, uid);

    //
    // START DATE
    //
    struct icaltimetype atime;
    QDateTime utc = QDateTime::currentDateTime().toUTC();
    atime.year = utc.date().year();
    atime.month = utc.date().month();
    atime.day = utc.date().day();
    atime.hour = utc.time().hour();
    atime.minute = utc.time().minute();
    atime.second = utc.time().second();
    // this is UTC is_utc is redundant but kept for completeness
    atime.is_utc = 1;
    atime.is_date = 0; // this is a date AND time
    atime.is_daylight = 0; // no daylight savings - its UTC
    atime.zone = icaltimezone_get_utc_timezone(); // set UTC timezone
    icalproperty *dtstart = icalproperty_new_dtstart(atime);
    icalcomponent_add_property(event, dtstart);

    //
    // Duration
    //

    // override values?
    QMap<QString, QString> lookup;
    lookup = rideItem->ride()->metricOverrides.value("workout_time");
    int secs = lookup.value("value", "0.0").toDouble();

    // from last - first timestamp?
    if (!rideItem->ride()->datePoints().isEmpty() &&
            rideItem->ride()->datePoints().last() != NULL) {
        if (!secs) {
            secs = rideItem->ride()->datePoints().last()->secs;
        }
    }

    // ok, got seconds so now create in vcard
    struct icaldurationtype dur;
    dur.is_neg = 0;
    dur.days = dur.weeks = 0;
    dur.hours = secs / 3600;
    dur.minutes = secs % 3600 / 60;
    dur.seconds = secs % 60;
    icalcomponent_set_duration(event, dur);

    // set title & description
    QString title = rideItem->ride()->getTag("Title", "");
    if (title == "") {
        title = rideItem->ride()->getTag("Sport", "") + " Workout";
    }
    icalcomponent_set_summary(event, title.toLatin1());
    icalcomponent_set_description(event,
              rideItem->ride()->getTag("Calendar Text", "").toLatin1());

    // put the event into root
    icalcomponent_add_component(root, event);
    return root;
}
