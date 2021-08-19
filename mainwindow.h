#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMediaPlayer>
#include <QFileInfo>
#include <QVideoWidget>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QThread>
#include <QDir>


#include "PlayerVLC.h"
#include "hilos.h"
#include "hilo_ajustar.h"
#include "archivo.h"
#include "hilo_tiempo.h"




QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(int argc,char **argv,QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


public slots:
    void Reproductor(QString name,int index);
    void Ajustar_elementos();
    void recnumeros();
    void Ocultar_lista();
    void Mostrar_lista();
    void colocar_posicion(float posicion); //coloca el video en la posicion deseada
    void obtener_medidas();
    void Declarar_objetos();
    void minimizar();
    void Colocar_dur();
    void Imprimir_tiempo();
    void guardar_tiempo();
    void get_frames(QString path);
    QStringList Conv_sm_min();
    QStringList  Elim_rep(QStringList lista, QString path, int &ind);






private slots:
    void on_btn_close_clicked();

    void on_btn_min_clicked();

    void on_btn_max_clicked();

    void on_lstv_directorio_clicked(const QModelIndex &index);

    void on_lstv_directorio_doubleClicked(const QModelIndex &index);

    void on_btn_stop_clicked();

    void on_btn_atras_clicked();

    void on_btn_res_clicked();

    void on_btn_output_clicked();

    void on_btn_vid_clicked();

    void on_btn_play_clicked();

    void on_sld_dur_sliderMoved(int position);

    void on_btn_atras__clicked();

    void on_btn_adelante_clicked();

    void on_btn_full_clicked();

    void on_btn_back_clicked();

    void on_btn_ext_clicked();

    void on_btn_docs_clicked();

    void on_btn_rec_clicked();

private:
    const QString ffprobe = "API/ffmpeg/bin/ffprobe";
    Ui::MainWindow *ui;

    QVideoWidget *_viwid;
    PlayerVLC *_player;
    QDir _direc;
    QFileSystemModel *directorio;
    QFileSystemModel *archivo;
    QFileInfoList list_vid;
    QPropertyAnimation *ani_lst;
    QPropertyAnimation *ani_btn1;
    QPropertyAnimation *ani_play;
    QPropertyAnimation *ani_atras;
    QPropertyAnimation *ani_adelante;
    QPropertyAnimation *ani_sld;
    QPropertyAnimation *ani_full;
    QPropertyAnimation *ani_docs;
    QPropertyAnimation *ani_ext;
    QPropertyAnimation *ani_rec;
    QPropertyAnimation *ani_lbldur;
    QPropertyAnimation *ani_tiempo;
    QParallelAnimationGroup *animationgroup;
    Hilos *hilo_1;
    hilo_ajustar *hilo_2;
    Hilo_tiempo *time;
    Archivo *_file;//objeto para majo de archivos
    QStringListModel *model;


    bool list_hide;//estado del la lista (oculta /visible)
    bool full; //si la ventana se encuentra full
    bool recientes; // si se estan usando videos en la lista de recientes
    bool hilo;//si algun hilo esta corriendo

    int ancho_vent;
    int alto_vent;
    float dur_max; //variable para guardar duracion del video
    int total_frames;
    int most_list;//guarda el valor para mostrar la lista
    int pos;//posicion actual del video
    QString bus;
    QStringList L_nombres; //lista para guardar nombres
    QStringList L_paths; //Lista para guardar paths
    QList<int> L_tiempos; //lista para guardar ms

};
#endif // MAINWINDOW_H
