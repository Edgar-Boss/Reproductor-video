#ifndef ARCHIVO_H
#define ARCHIVO_H
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QList>
#include <QStringListModel>
#include <QCoreApplication>

class Archivo
{

private:
    QTextStream io;
    QFile arch;
    QFile _arch;
    QString contenido;
    QString txt_recientes = QCoreApplication::applicationDirPath() + "/Recursos/recientes.txt";
    QString txt_tiempos = QCoreApplication::applicationDirPath() + "/Recursos/tiempo.txt";
    QStringList L_nombres;
    QStringList L_path;
    QList<int> L_tiempos;


public:
    Archivo();
    void Escribir(QString path);
    void Vaciar_datos(QStringList paths);
    QStringListModel *get_datos();
    void Leer_archivo();
    QStringList get_nombres();
    QStringList get_paths();
    QList<int> obtener_tiempos();
    void Vaciar_tiempos(QList<int> tiempos);

};

#endif // ARCHIVO_H
