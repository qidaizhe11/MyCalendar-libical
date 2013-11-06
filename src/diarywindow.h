#ifndef DIARYWINDOW_H
#define DIARYWINDOW_H

#include <QWidget>
#include <QCalendarWidget>
#include <QStackedWidget>

#include <QTableView>
// TODO:
//#include <GcCalendarModel.h>

class QLabel;
class QComboBox;
class QPushButton;

class DiaryWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DiaryWindow(QWidget *parent = 0);

signals:

public slots:
//    void rideSelected();
//    void configChanged();
    void nextClicked();
    void prevClicked();
//    void setDefaultView(int);
//    bool eventFilter(QObject *object, QEvent *e);

protected:
    QLabel *title;
    QPushButton *prev, *next;

    QComboBox *viewMode;
    QStackedWidget *allViews;

    QTableView *monthlyView;
    // TODO:
    // GcCalendarModel *calendarModel;

    bool active;
//    QList<filedd
};

#endif // DIARYWINDOW_H
