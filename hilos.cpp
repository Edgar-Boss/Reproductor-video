#include "hilos.h"


Hilos::Hilos(QObject *parent) : QThread(parent)
{

}

void Hilos::run()
{
    emit env_num();
}

