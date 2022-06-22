#include "archivo.h"
#include <QDebug>

Archivo::Archivo()
{



}

QStringList Archivo::get_nombres()
{
    return L_nombres;
}

QStringList Archivo::get_paths()
{
    return L_path;
}

QList<int> Archivo::obtener_tiempos()
{
    L_tiempos.clear();
    arch.setFileName(txt_tiempos);
    arch.open(QIODevice::Text | QIODevice::ReadOnly);
    io.setDevice(&arch);
    int ms;
    while(!io.atEnd())
    {
        ms= (io.readLine()).toInt();
        L_tiempos.append(ms);
    }
    arch.close();
    return L_tiempos;
}

void Archivo::Vaciar_tiempos(QList<int> tiempos)
{
    QFile::remove(txt_tiempos);
    _arch.setFileName(txt_tiempos);
    _arch.open(QIODevice::Text | QIODevice::Append);
    io.setDevice(&_arch);


    for(int k=0;k<tiempos.count();k++)
    {
        if(k==0)
            io<<tiempos.at(k);
        else

            io<<"\n"<<tiempos.at(k);
    }
    _arch.close();
}

void Archivo::Escribir(QString path)
{
    arch.setFileName(txt_recientes);
    arch.open(QIODevice::Text | QIODevice::Append);
    io.setDevice(&arch);
    io<<path;
    arch.close();
}


QStringListModel *Archivo::get_datos()
{

    Leer_archivo();
    QStringListModel *model;// = new QStringListModel();
    model = new QStringListModel;
    model->setStringList(L_nombres);

    return model;
}

void Archivo::Leer_archivo()
{
    L_path.clear();
    L_nombres.clear();
    arch.setFileName(txt_recientes);
    arch.open(QIODevice::Text | QIODevice::ReadOnly);
    io.setDevice(&arch);
    QString   contenido;

    int cut;
    while(!io.atEnd())
    {
        contenido=io.readLine();
        L_path.append(contenido);
        cut=contenido.lastIndexOf("/");
        contenido.remove(0,cut+1);
        L_nombres.append(contenido);
    }
    arch.close();
}

void Archivo::Vaciar_datos(QStringList paths)
{
    QFile::remove(txt_recientes);

    arch.setFileName(txt_recientes);
    arch.open(QIODevice::Text | QIODevice::Append);
    io.setDevice(&arch);
    QString n="\n";

    for(int k=0;k<paths.count();k++)
    {
        if(k==0)
            io<<paths.at(k);
        else
            io<< n +paths.at(k);
    }
    arch.close();
}




