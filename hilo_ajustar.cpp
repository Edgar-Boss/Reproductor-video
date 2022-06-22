#include "hilo_ajustar.h"

hilo_ajustar::hilo_ajustar(QObject *parent) : QThread(parent)
{

}

void hilo_ajustar::run()
{

    emit env_ajuste();
}


