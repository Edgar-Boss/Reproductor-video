#include "hilo_tiempo.h"

Hilo_tiempo::Hilo_tiempo(QObject *parent) : QThread(parent)
{

}

void Hilo_tiempo::run()
{
    while(true)
    {
        emit env_tiempo();
        sleep(1);

    }
}
