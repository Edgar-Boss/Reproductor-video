#ifndef HILO_TIEMPO_H
#define HILO_TIEMPO_H

#include <QThread>
#include <QObject>

class Hilo_tiempo : public QThread
{
    Q_OBJECT
public:
    explicit Hilo_tiempo(QObject *parent = nullptr);



signals:
    void env_tiempo();

public slots:

    void run();
};

#endif // HILO_TIEMPO_H
