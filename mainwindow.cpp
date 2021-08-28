#include <QDebug>
#include <QSizePolicy>
#include <QApplication>
#include <time.h>
#include <QGraphicsVideoItem>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QProcess>
#include <QScreen>
#include <QFile>
#include <QTextStream>
#include <QStringListModel>
#include <QKeyEvent>


#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(char **argv, QWidget *parent)
    : QMainWindow(parent,Qt::FramelessWindowHint)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    Declarar_objetos();
    Colocar_imagenes();
    _viwid->setMouseTracking(true);//hablilitar trazado del mouse en video
    _player->setVideoOutput(_viwid);
    _player->stop_mouse();// detiene eventos del mose de vlc
    ui->stack_vid->insertWidget(0,_viwid);//coloca el reproductor en stacked

    // filtros
    QStringList filtros;
    filtros << "*.mp4"
            << "*.avi"
            << "*.mkv";

    _direc.setPath(QDir::homePath());//coloca path del home
    archivo->setFilter(QDir::NoDotAndDotDot|QDir::AllEntries|QDir::AllDirs);
    archivo->setNameFilters(filtros);
    archivo->setNameFilterDisables(false);
    archivo->setRootPath(_direc.path());
    //acomodar icono



    //stylesheet del la lista del directorio
    ui->lstv_directorio->setStyleSheet("QListView::item:hover{background-color: rgb(162, 255, 176,50);}"
                                        "QListView::item:selected{background-color: rgb(146,252,45,150);}"
                                        "QListView {color:#b4b4b4;border: 1px solid #232323;;alternate-background-color: #232323;show-decoration-selected: 1;background: #232323;}");



    model = _file->get_datos();//obtiene los nombres de los archivos recientes
    L_paths = _file->get_paths();//obtiene los datos de los paths recientes
    //ui->label->setText(QCoreApplication::applicationDirPath());
    ui->lstv_directorio->setModel(model);//coloca model de los nombres al listview
    L_tiempos = _file->obtener_tiempos();//obtiene los tiempos guardados


    most_list=1040; //a partir de 1040 pixeles para mostrar lista
    pos=0;//inicializa video en 0
    list_hide=false;
    recientes=true;
    full=false;
    hilo = false;

    L_paths = _file->get_paths();//obitne los paths usados recientemente
    L_tiempos=_file->obtener_tiempos();//obtiene los tiempos usados recientemente

     if (argv[1]!=nullptr)
     {
        QString arg= argv[1];

        if(arg.endsWith(".mp4") || arg.endsWith(".mkv") || arg.endsWith(".avi"))
        {
            arg.replace("\\","/");//remplaza \ por // para maejo de los metodos del _file
            Reproductor(arg,0);
        }
     }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Colocar_imagenes()
{


    QIcon icon(*px_close);
    QSize iconSize(30,30);
    ui->btn_close->setIcon(icon);
    ui->btn_close->setIconSize(iconSize);

    QIcon icon_max(*px_max);
    QSize iconSize_max(18,18);
    ui->btn_max->setIcon(icon_max);
    ui->btn_max->setIconSize(iconSize_max);

    QIcon icon_min(*px_min);
    QSize iconSize_min(18,18);
    ui->btn_min->setIcon(icon_min);
    ui->btn_min->setIconSize(iconSize_min);

    QIcon icon_full(*px_full);
    QSize iconSize_full(15,15);
    ui->btn_full->setIcon(icon_full);
    ui->btn_full->setIconSize(iconSize_full);
}

void MainWindow::Pausar_reanudar()
{

    if(_player->isPlaying())
        _player->pause();
    else
    {
         time->start();
        _player->play();
    }
}

void MainWindow::retrasar_segundos()
{
    double aux=(pos-5000)/dur_max;
    _player->setPosition(aux);
}

void MainWindow::adelantar_segundos()
{
    double aux=(pos+5000)/dur_max;
    _player->setPosition(aux);
}

void MainWindow::Declarar_objetos()
{
    px_close = new QPixmap(":/icon/Icon/close.png");
    px_max = new QPixmap(":/icon/Icon/window_maximize.png");
    px_rest = new QPixmap(":/icon/Icon/restaurar.png");
    px_min = new QPixmap(":/icon/Icon/minimize_.png");
    px_full= new QPixmap(":/icon/Icon/full_screen.png");
    px_exit_full = new QPixmap(":/icon/Icon/exit_full_screen.png");
    ani_lst = new QPropertyAnimation(ui->lstv_directorio,"geometry");
    ani_play = new QPropertyAnimation(ui->btn_play,"geometry");
    ani_atras = new QPropertyAnimation(ui->btn_atras_,"geometry");
    ani_adelante = new QPropertyAnimation(ui->btn_adelante,"geometry");
    ani_full= new QPropertyAnimation(ui->btn_full,"geometry");
    ani_sld= new QPropertyAnimation(ui->sld_dur,"geometry");
    ani_btn1 = new QPropertyAnimation(ui->btn_back,"geometry");
    ani_ext = new QPropertyAnimation(ui->btn_ext,"geometry");
    ani_docs = new QPropertyAnimation(ui->btn_docs,"geometry");
    ani_rec = new QPropertyAnimation(ui->btn_rec,"geometry");
    ani_lbldur = new QPropertyAnimation(ui->lbl_dur,"geometry");
    ani_tiempo = new QPropertyAnimation(ui->lbl_tiempo,"geometry");
    _player =new PlayerVLC(this);
    _viwid = new QVideoWidget(this);
    archivo = new QFileSystemModel(this);
    _file = new Archivo();
    model= new QStringListModel;
}

void MainWindow::Reproductor(QString path, int index)//metodo para reproducir video
{
    double posi;
    if(hilo==true)
    {
        guardar_tiempo();
        time->terminate();
        delete time;
    }
    time = new Hilo_tiempo();
    hilo=true;

    connect(time,&Hilo_tiempo::env_tiempo,this,&MainWindow::Imprimir_tiempo);
    connect(_player,&PlayerVLC::positionChanged,this,&MainWindow::colocar_posicion);
    _player->setMedia(QUrl::fromLocalFile(path));
    _player->play();
    dur_max=_player->duration();
    posi=agregar_a_listas(path,index);


    _file->Vaciar_tiempos(L_tiempos);//escribe en archivo los tiempos
    _file->Vaciar_datos(L_paths);//escribe en archivo los paths
    L_nombres = _file->get_nombres();//lee archivos y obtiene nombres
    model->setStringList(QStringList{});
    model = _file->get_datos();
    ui->lstv_directorio->setModel(model);  //coloca nombres en lista de recientes de listview

    _player->setPosition(posi);
    ui->sld_dur->setMaximum(dur_max);

    get_frames(path);

    recientes=true;


    Colocar_dur();
    time->start();


}

double MainWindow::agregar_a_listas(QString path,int index)
{
    int ind_r=0;
    int aux;
    bool res;
    double posi;
    L_paths = Elim_rep(L_paths,path,ind_r,res);//elimina repetidos si los hay y regresa el indice (ind_r) donde lo encontro
    L_paths.insert(0,path);//agrega al principio de la lista el nuevo path


    if(res)//si el video esta en recientes
    {
        posi=L_tiempos.at(index)/dur_max;
        aux=L_tiempos.at(ind_r);//obtiene el tiempo del indice
        L_tiempos.removeAt(ind_r);//remueve el tiempo en el indice de tiempos
        L_tiempos.insert(0,aux);//inserta en la posicion 0 el tiempo de aux

    }
    else//si el video no esta en recientes
    {

        posi=0;
        L_tiempos.insert(0,0);//agrega un 0 a la primera posicion

    }

    return posi;

}

void MainWindow::minimizar()
{
    this->showMinimized();
}

void MainWindow::colocar_posicion(float posicion)//colocar posicion en video
{
    pos=posicion*dur_max;
    ui->sld_dur->setSliderPosition(pos);

}

void MainWindow::obtener_medidas()
{
    ancho_vent = this->width();
    alto_vent = this->height();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->x() > 0 && event->x()<ancho_vent && event->y()>25 && event->y()<alto_vent)
    {
        if(list_hide==0)
            Ocultar_lista();
        else
        {
            Pausar_reanudar();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->x() > most_list && list_hide ==true)
    {
        Mostrar_lista();
    }

}

QStringList MainWindow::Elim_rep(QStringList lista,QString path, int &ind, bool &res)
{
    res=false;
    for(int k=0;k<lista.count();k++)
    {
        if(lista.at(k)==path)
        {
            res= true;
            lista.removeAt(k);
            ind=k;
        }

    }
    return lista;
}

void MainWindow::Colocar_dur()
{
    QStringList tiempo=_player->get_tiempo();

    ui->lbl_dur->setText(tiempo.at(0)+":"+tiempo.at(1)+":"+tiempo.at(2));
}

void MainWindow::Imprimir_tiempo()
{

    if(_player->isPlaying())
        qDebug("reproduciendo");
    else
    {

        qDebug("Detenido");
        time->terminate();
    }

    QStringList tiempo = Conv_sm_min();

    ui->lbl_tiempo->setText(tiempo.at(0)+":"+tiempo.at(1)+":"+tiempo.at(2)+" /");

}

void MainWindow::guardar_tiempo()
{

    L_tiempos.removeAt(0);
    L_tiempos.insert(0,pos);
    _file->Vaciar_tiempos(L_tiempos);

}

void MainWindow::get_frames(QString path)
{
    QString Cad_inf_Videos;
    QProcess *Get_info_Video= new QProcess();
    QStringList arguments;
    arguments <<"-v" <<"error"<<"-select_streams"<<"v:0"<<"-show_entries"<<"stream=r_frame_rate"<<"-of"<<"default=noprint_wrappers=1:nokey=1"<<path<<"-hide_banner";

    Get_info_Video->setProcessChannelMode(QProcess::MergedChannels);
    Get_info_Video->start(ffprobe,arguments);
    Get_info_Video->waitForFinished();
    Cad_inf_Videos = Get_info_Video->readAllStandardOutput();
    int a= Cad_inf_Videos.mid(0,Cad_inf_Videos.indexOf("/")).toInt();
    int b= Cad_inf_Videos.mid(Cad_inf_Videos.indexOf("/")+1,(Cad_inf_Videos.indexOf('\r')-Cad_inf_Videos.indexOf("/"))-1).toUInt();
    total_frames= (int)(a/b)*_player->duration()/1000.00;
}

QStringList MainWindow::Conv_sm_min()
{
    int aux;
    int ms = pos % 1000;
    aux=pos/1000;
    int s = aux % 60;
    aux=aux/60;
    int m = aux % 60;
    aux= aux/60;
    int h = aux % 60;

    QStringList tiempo;

    tiempo.append( QString::number(h));
    if(m<10)
        tiempo.append( "0"+QString::number(m));
    else
        tiempo.append(QString::number(m));
    if(s<10)
        tiempo.append( "0" + QString::number(s));
    else
        tiempo.append(QString::number(s));
    if(ms<10)
        tiempo.append( "00" + QString::number(ms));
    else if(ms<100)
        tiempo.append("0" + QString::number(ms));
    else
        tiempo.append( QString::number(ms));

    return tiempo;
}

void MainWindow::on_btn_min_clicked()
{
    this->setWindowState(Qt::WindowMinimized) ;
    Ajustar_elementos();

}

void MainWindow::recnumeros()
{

    this->showMaximized();

}

void MainWindow::Mostrar_lista()
{
    int y=21;
    if (full==true)
        y=0;


    obtener_medidas();

    ani_lst->setDuration(300);
    ani_lst->setStartValue(ui->lstv_directorio->geometry());
    ani_lst->setEndValue(QRect(ancho_vent-200,y,200,alto_vent-94-y));
    ani_lst->start();

    ani_play->setDuration(400);
    ani_play->setStartValue(ui->btn_play->geometry());
    ani_play->setEndValue(QRect((ancho_vent/2)-40,alto_vent-35,80,21));
    ani_play->start();

    ani_atras->setDuration(400);
    ani_atras->setStartValue(ui->btn_atras_->geometry());
    ani_atras->setEndValue(QRect((ancho_vent/2)-120,alto_vent-35,80,21));
    ani_atras->start();

    ani_adelante->setDuration(400);
    ani_adelante->setStartValue(ui->btn_adelante->geometry());
    ani_adelante->setEndValue(QRect((ancho_vent/2)+40,alto_vent-35,80,21));
    ani_adelante->start();

    ani_full->setDuration(450);
    ani_full->setStartValue(ui->btn_full->geometry());
    ani_full->setEndValue(QRect(ancho_vent-25,alto_vent-25,20,20));
    ani_full->start();

    ani_sld->setDuration(300);
    ani_sld->setStartValue(ui->sld_dur->geometry());
    ani_sld->setEndValue(QRect(0,alto_vent-50,ancho_vent,15));
    ani_sld->start();

    ani_btn1->setDuration(300);
    ani_btn1->setStartValue(ui->btn_back->geometry());
    ani_btn1->setEndValue(QRect(ancho_vent-200,alto_vent-94,20,10));
    ani_btn1->start();

    ani_ext->setDuration(300);
    ani_ext->setStartValue(ui->btn_ext->geometry());
    ani_ext->setEndValue(QRect(ancho_vent-20,alto_vent-94,20,10));
    ani_ext->start();

    ani_docs->setDuration(300);
    ani_docs->setStartValue(ui->btn_docs->geometry());
    ani_docs->setEndValue(QRect(ancho_vent-40,alto_vent-94,20,10));
    ani_docs->start();

    ani_rec->setDuration(300);
    ani_rec->setStartValue(ui->btn_rec->geometry());
    ani_rec->setEndValue(QRect(ancho_vent-60,alto_vent-94,20,10));
    ani_rec->start();

    ani_lbldur->setDuration(300);
    ani_lbldur->setStartValue(ui->lbl_dur->geometry());
    ani_lbldur->setEndValue(QRect(47,alto_vent-38,47,13));
    ani_lbldur->start();

    ani_tiempo->setDuration(300);
    ani_tiempo->setStartValue(ui->lbl_tiempo->geometry());
    ani_tiempo->setEndValue(QRect(0,alto_vent-38,47,13));
    ani_tiempo->start();




    list_hide=false;

}

void MainWindow::Ocultar_lista()
{
   int y=21;
   if (full==true)
       y=0;

   obtener_medidas();


    ani_lst->setDuration(300);
    ani_lst->setStartValue(ui->lstv_directorio->geometry());
    ani_lst->setEndValue(QRect(ancho_vent+10,y,200,alto_vent-94-y));
    ani_lst->start();


    ani_play->setDuration(300);
    ani_play->setStartValue(ui->btn_play->geometry());
    ani_play->setEndValue(QRect((ancho_vent/2)-40,alto_vent,80,21));
    ani_play->start();

    ani_atras->setDuration(300);
    ani_atras->setStartValue(ui->btn_atras_->geometry());
    ani_atras->setEndValue(QRect((ancho_vent/2)-120,alto_vent,80,21));
    ani_atras->start();

    ani_adelante->setDuration(300);
    ani_adelante->setStartValue(ui->btn_adelante->geometry());
    ani_adelante->setEndValue(QRect((ancho_vent/2)+40,alto_vent,80,21));
    ani_adelante->start();

    ani_full->setDuration(300);
    ani_full->setStartValue(ui->btn_full->geometry());
    ani_full->setEndValue(QRect(ancho_vent-25,alto_vent,15,15));
    ani_full->start();

    ani_sld->setDuration(400);
    ani_sld->setStartValue(ui->sld_dur->geometry());
    ani_sld->setEndValue(QRect(0,alto_vent,ancho_vent,15));
    ani_sld->start();

    ani_btn1->setDuration(300);
    ani_btn1->setStartValue(ui->btn_back->geometry());
    ani_btn1->setEndValue(QRect(ancho_vent,alto_vent-94,20,10));
    ani_btn1->start();


    ani_ext->setDuration(300);
    ani_ext->setStartValue(ui->btn_ext->geometry());
    ani_ext->setEndValue(QRect(ancho_vent,alto_vent-94,201,10));
    ani_ext->start();

    ani_docs->setDuration(300);
    ani_docs->setStartValue(ui->btn_docs->geometry());
    ani_docs->setEndValue(QRect(ancho_vent,alto_vent-94,201,10));
    ani_docs->start();

    ani_lbldur->setDuration(300);
    ani_lbldur->setStartValue(ui->lbl_dur->geometry());
    ani_lbldur->setEndValue(QRect(47,alto_vent,47,13));
    ani_lbldur->start();

    ani_tiempo->setDuration(300);
    ani_tiempo->setStartValue(ui->lbl_tiempo->geometry());
    ani_tiempo->setEndValue(QRect(0,alto_vent,47,13));
    ani_tiempo->start();

    ani_rec->setDuration(300);
    ani_rec->setStartValue(ui->btn_rec->geometry());
    ani_rec->setEndValue(QRect(ancho_vent,alto_vent-94,201,10));
    ani_rec->start();



    list_hide=true;



}

void MainWindow::Ajustar_elementos()
{

    obtener_medidas();
    int x;
    int y=21;
    if(full==true)
        y=0;
    if(list_hide)
        x=0;
    else
        x=200;


    ui->lbl_barra->setGeometry(0,0,ancho_vent-60,20);
    ui->btn_min->setGeometry(ancho_vent-60,0,20,20);
    ui->btn_max->setGeometry(ancho_vent-40,0,20,20);
    ui->btn_close->setGeometry(ancho_vent-20,0,20,20);
    ui->lstv_directorio->setGeometry(ancho_vent-x,y,200,alto_vent-94-y);
    ui->stack_vid->setGeometry(0,y,ancho_vent,alto_vent-y);
    ui->sld_dur->setGeometry(0,alto_vent-40,ancho_vent,15);
    ui->btn_play->setGeometry((ancho_vent/2)-40,alto_vent-25,80,21);
    ui->btn_atras_->setGeometry((ancho_vent/2)-120,alto_vent-25,80,21);
    ui->btn_adelante->setGeometry((ancho_vent/2)+40,alto_vent-25,80,21);
    ui->btn_full->setGeometry(ancho_vent-25,alto_vent-25,25,21);
    ui->btn_back->setGeometry(ancho_vent-200,alto_vent-94,20,10);
    ui->btn_ext->setGeometry(ancho_vent-20,alto_vent-94,20,10);
    ui->btn_docs->setGeometry(ancho_vent-40,alto_vent-94,20,10);
    ui->btn_rec->setGeometry(ancho_vent-60,alto_vent-94,20,10);
    ui->lbl_dur->setGeometry(47,alto_vent-28,47,13);
    ui->lbl_tiempo->setGeometry(0,alto_vent-28,47,13);
    most_list=ancho_vent*0.99;

}

void MainWindow::on_sld_dur_sliderMoved(int position)//metodo para cambiar la posicion del video desde el slider
{
    double  pos= position/dur_max;
    _player->setPosition(pos);

}

void MainWindow::on_btn_close_clicked()
{
    if(_player->isPlaying())
        _player->stop();
    L_tiempos.removeAt(0);
    L_tiempos.insert(0,pos);
    _file->Vaciar_tiempos(L_tiempos);
    QStringList tiempo = Conv_sm_min();
    qDebug()<<"close: "<<tiempo.at(0)<<":"<<tiempo.at(1)<<":"<<tiempo.at(2);
    this->close();
}

void MainWindow::on_btn_max_clicked()
{

    if(!this->isMaximized())
    {
        QIcon icon_max(*px_rest);
        QSize iconSize_max(13,13);
        ui->btn_max->setIcon(icon_max);
        ui->btn_max->setIconSize(iconSize_max);


        if(!list_hide)
        {
            hilo_1 = new Hilos;
            hilo_2 = new hilo_ajustar;
            connect(hilo_1,&Hilos::env_num,this,&MainWindow::recnumeros);
            connect(hilo_2,&hilo_ajustar::env_ajuste,this,&MainWindow::Ajustar_elementos);

            hilo_1->start();
            hilo_1->setPriority(QThread::HighestPriority);
            hilo_1->wait();

            hilo_2->start();
            hilo_2->setPriority(QThread::LowestPriority);

        }else
        {
            hilo_1 = new Hilos;
            hilo_2 = new hilo_ajustar;
            connect(hilo_1,&Hilos::env_num,this,&MainWindow::recnumeros);
            connect(hilo_2,&hilo_ajustar::env_ajuste,this,&MainWindow::Ajustar_elementos);

            hilo_1->start();
            hilo_1->setPriority(QThread::HighestPriority);
            hilo_1->wait();

            hilo_2->start();
            hilo_2->setPriority(QThread::LowestPriority);
        }
    }



}

void MainWindow::on_lstv_directorio_clicked(const QModelIndex &index)
{

//    L_paths = _file->get_paths();
//    qDebug()<<L_paths.at(index.row());


}

void MainWindow::on_lstv_directorio_doubleClicked(const QModelIndex &index)//Seleccionar video para reproducir, de list view
{
    //Reproducir
    L_paths = _file->get_paths();
    QString rep;


    if(recientes)
    {

        rep = L_paths.at(index.row());
    }
    else
    {
        rep =archivo->fileInfo(index).absoluteFilePath();
    }

    if(rep.endsWith(".mp4") || rep.endsWith(".mkv") || rep.endsWith(".avi"))
    {

        Reproductor(rep,index.row());
        Ocultar_lista();
    }
    else
    {
      ui->lstv_directorio->setRootIndex(archivo->setRootPath(archivo->fileInfo(index).canonicalFilePath()));
      bus= archivo->fileInfo(index).canonicalFilePath();
    }

}

void MainWindow::on_btn_stop_clicked()
{
//    _player->stop();
//    ui->lstv_directorio->show();
}

void MainWindow::on_btn_atras_clicked()
{
    int recort[2];

    recort[0]=bus.lastIndexOf("/");
    recort[1]=bus.size();
    bus.remove(recort[0],recort[1]);
    ui->lstv_directorio->setRootIndex(archivo->setRootPath(bus));

}

void MainWindow::on_btn_res_clicked()
{
   Mostrar_lista();
}

void MainWindow::on_btn_output_clicked()
{
     Ocultar_lista();

}

void MainWindow::on_btn_vid_clicked()
{
    Ocultar_lista();
}

void MainWindow::on_btn_play_clicked()
{
    Pausar_reanudar();
}

void MainWindow::on_btn_atras__clicked()
{
    retrasar_segundos();
}

void MainWindow::on_btn_adelante_clicked()
{
   adelantar_segundos();
}

void MainWindow::on_btn_full_clicked()
{


    if(full==false)
    {




        QIcon icon_full(*px_exit_full);
        QSize iconSize_full(15,15);
        ui->btn_full->setIcon(icon_full);
        ui->btn_full->setIconSize(iconSize_full);

        QScreen *screen = QApplication::screens().at(0);

        int ancho = screen->size().width();
        int alto = screen->size().height();

        this->setGeometry(0,0,ancho,alto);

        ui->lbl_barra->hide();
        ui->btn_min->hide();
        ui->btn_max->hide();
        ui->btn_close->hide();
        ui->stack_vid->setGeometry(0,0,ancho,alto);
        full = true;
        Ajustar_elementos();

    }
    else
    {



        QIcon icon_full(*px_full);
        QSize iconSize_full(15,15);
        ui->btn_full->setIcon(icon_full);
        ui->btn_full->setIconSize(iconSize_full);
        ui->lbl_barra->show();
        ui->btn_min->show();
        ui->btn_max->show();
        ui->btn_close->show();
        full=false;

        bool min=false;
        bool max=false;
        bool ajust=false;

        while(min==false || max==false || ajust==false)
        {
            if(this->isMinimized())
            {
                min=true;
                if(this->isMaximized())
                {
                    max=true;
                    Ajustar_elementos();
                    ajust=true;
                }
                else
                    this->showMaximized();
            }
            else
                this->showMinimized();
        }
        this->showMaximized();
    }
}

void MainWindow::on_btn_back_clicked()
{
    int recort[2];

    recort[0]=bus.lastIndexOf("/");
    recort[1]=bus.size();
    bus.remove(recort[0],recort[1]);
    ui->lstv_directorio->setRootIndex(archivo->setRootPath(bus));
}

void MainWindow::on_btn_ext_clicked()
{

    ui->lstv_directorio->setModel(archivo);
    ui->lstv_directorio->setRootIndex(archivo->index(""));
    recientes = false;
}

void MainWindow::on_btn_docs_clicked()
{
    ui->lstv_directorio->setModel(archivo);
    ui->lstv_directorio->setRootIndex(archivo->index(_direc.path()));
    recientes = false;
}

void MainWindow::on_btn_rec_clicked()
{
    L_paths = _file->get_paths();
    model->setStringList(QStringList{});
    model = _file->get_datos();
    ui->lstv_directorio->setModel(model);

    recientes=true;

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

    switch (event->key())
    {
        case Qt::Key_Space:
            Pausar_reanudar();
        break;
        case Qt::Key_Left:
             retrasar_segundos();
        break;
        case Qt::Key_Right:
             adelantar_segundos();
        break;

    }
}
