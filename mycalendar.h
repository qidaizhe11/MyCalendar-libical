#ifndef MYCALENDAR_H
#define MYCALENDAR_H

#include <QCalendarWidget>
#include <QMultiMap>

class MainWindow;

class MyCalendar : public QCalendarWidget
{
    Q_OBJECT
public:
    MyCalendar(MainWindow *parent);
    QSize sizeHint() const;
    void setHome(const QDir&); // TODO: QDir
    void addWorkoutCode(QString, QColor);

protected:
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

signals:

public slots:
    void configUpdate();

private:
    QMap<QString, QColor> workoutCode;
    QDir home;
    QColor defaultColor;
    MainWindow *mainWindow;
};

#endif // MYCALENDAR_H
