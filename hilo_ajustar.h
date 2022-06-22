#ifndef HILO_AJUSTAR_H
#define HILO_AJUSTAR_H

#include <QThread>
#include <QObject>

class hilo_ajustar : public QThread
{
    Q_OBJECT
public:
    explicit hilo_ajustar(QObject *parent = nullptr);

signals:
    void env_ajuste();


public slots:
    void run();
};

#endif // HILO_AJUSTAR_H
