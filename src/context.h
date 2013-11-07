#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>

class RideItem;
class Athlete;

class Context : public QObject
{
    Q_OBJECT
public:
    explicit Context(QWidget *parent = 0);

signals:

public slots:

};

#endif // CONTEXT_H
