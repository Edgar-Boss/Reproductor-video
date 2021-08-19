#ifndef HILOS_H
#define HILOS_H

#include <QThread>
#include <QObject>


class Hilos : public QThread
{
    Q_OBJECT
public:
    explicit Hilos(QObject *parent = nullptr);
signals:
    void env_num();


public slots:
    void run();

private:


};

#endif // HILOS_H
