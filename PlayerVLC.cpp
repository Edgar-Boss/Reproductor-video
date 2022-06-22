#include "PlayerVLC.h"

PlayerVLC::PlayerVLC(QObject *parent)
{
    const char *const vlc_args [] = {//ui->btn_play->click();
        "--log-verbose", "0"
        ,"--quiet"
        ,"--no-stats"
        ,"--no-file-logging"
        ,"--quiet-synchro"
        ,"--avcodec-hw", "any"
        ,"--verbose", "0"
        //!,"--aout", "any" //!directsound
        ,"--avcodec-debug", "0"
    };

    mMedia = nullptr;
    mTimer = new QTimer(this);
    mInstance = libvlc_new (sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);//libvlc_new(0, nullptr);
    mMediaPlayer = libvlc_media_player_new(mInstance);

//    connect(mTimer, &QTimer::timeout, [&]() {
//        emit positionChanged(position());
//        emit mediaStatusChanged(status());
//        if (status() == END_OF_MEDIA) {
//            mTimer->stop();
//        }
//    });
    connect(mTimer, &QTimer::timeout, [&]() {
        emit positionChanged(position());
        emit mediaStatusChanged(status());
        if (status() == END_OF_MEDIA) {
            //QThread::sleep(  );
            emit mediaStatusChanged( 10 );
            mTimer->stop();
        }
    });
}

PlayerVLC::~PlayerVLC()
{
    libvlc_media_player_stop(mMediaPlayer);
    libvlc_media_release(mMedia);
    libvlc_media_player_release(mMediaPlayer);
    libvlc_release(mInstance);
}

void PlayerVLC::setVideoOutput(QWidget *videoWidget)
{
    mVideoWidget = videoWidget;

#if defined (__WIN32__)
    libvlc_media_player_set_hwnd(mMediaPlayer, reinterpret_cast<HWND *>(mVideoWidget->winId()));
    //libvlc_media_player_set_drawable()
#endif
#if defined(__linux__)
    libvlc_media_player_set_xwindow(mMediaPlayer,mVideoWidget->winId());
#endif
}

void PlayerVLC::setVideoOutputFrame(QFrame *videoWidget)
{

}

void PlayerVLC::setMedia(const QUrl &url)
{
    libvlc_media_release(mMedia);
    mMedia = libvlc_media_new_location(mInstance,
                                       url.url().toStdString().c_str());

    libvlc_media_player_set_media(mMediaPlayer, mMedia);

    libvlc_media_parse(mMedia);

}

qint64 PlayerVLC::mediaLenght()
{
    return libvlc_media_player_get_length(mMediaPlayer);
}

qint64 PlayerVLC::duration()
{
    dur=libvlc_media_get_duration(mMedia);

    return dur;
}

float PlayerVLC::position()
{
    return libvlc_media_player_get_position(mMediaPlayer);
}

bool PlayerVLC::isPlaying()
{
    return libvlc_media_player_is_playing(mMediaPlayer);
}

bool PlayerVLC::hasMedia()
{
    return libvlc_media_player_get_media(mMediaPlayer);
}

bool PlayerVLC::isMute()
{
    return libvlc_audio_get_mute(mMediaPlayer);
}

int PlayerVLC::volume()
{
    return libvlc_audio_get_volume(mMediaPlayer);
}

int PlayerVLC::status()
{
    return libvlc_media_player_get_state(mMediaPlayer);
}

int PlayerVLC::getWidthVideo()
{
    /*return Width del video*/
    return libvlc_video_get_width(mMediaPlayer);
}

int PlayerVLC::getHeightVideo()
{
    /*return Height del video*/
    return libvlc_video_get_height(mMediaPlayer);
}

float PlayerVLC::getFpsVideo()
{
    /*return FPS del video*/
    return libvlc_media_player_get_rate(mMediaPlayer);
}

int PlayerVLC::getLengthVideo()
{
    /*return Length del video*/
    return libvlc_media_player_get_length(mMediaPlayer);
}

int PlayerVLC::getChapterVideo()
{
    return libvlc_media_player_get_chapter_count(mMediaPlayer);
}

void PlayerVLC::getAudio()
{
    libvlc_media_track_t** mtArray;
    unsigned int iTracks;

    if ((iTracks = libvlc_media_tracks_get(mMedia, &mtArray)) > 0)
    {
        qDebug() << "Language: 2" << iTracks;
        for (int i = 0; i < iTracks; i++)
        {
            qDebug() << "Language: " << mtArray[i]->psz_language << ", Descr.: " << mtArray[i]->psz_description << endl;
        }
        libvlc_media_tracks_release(mtArray, iTracks);
    }
}

void PlayerVLC::nextframe()
{
    libvlc_media_player_next_frame(mMediaPlayer);
}

void PlayerVLC::releaseMedia()
{
    libvlc_media_release(mMedia);
    //libvlc_media_player_release(mMediaPlayer);
    //libvlc_release(mInstance);
}

void PlayerVLC::setPosition(float pos)
{
    libvlc_media_player_set_position(mMediaPlayer, pos);
}

void PlayerVLC::setVolume(int volume)
{
    libvlc_audio_set_volume(mMediaPlayer, volume);
}

void PlayerVLC::setMute(bool mute)
{
    libvlc_audio_set_mute(mMediaPlayer, mute);
}

void PlayerVLC::setSlaveAudio(const QUrl &url)
{
    QString option = ":input-slave="+url.url();
    libvlc_media_add_option(mMedia,option.toStdString().c_str());
    option.clear();
    option = ":file-caching=300";
    libvlc_media_add_option(mMedia,option.toStdString().c_str());
    option.clear();
    option = ":audio-track=1";
    libvlc_media_add_option(mMedia,option.toStdString().c_str());
    qDebug() << "fin slave audio";
}

void PlayerVLC::play()
{
    libvlc_media_player_play(mMediaPlayer);
    emit mediaStatusChanged(status());
    mTimer->start(100);
}

void PlayerVLC::pause()
{
    libvlc_media_player_pause(mMediaPlayer);
    emit mediaStatusChanged(status());
    mTimer->stop();
}

void PlayerVLC::stop()
{
    mTimer->stop();
    libvlc_media_player_stop(mMediaPlayer);
    emit mediaStatusChanged(status());
}

void PlayerVLC::stop_mouse()
{

    libvlc_video_set_mouse_input(mMediaPlayer, 0);
    libvlc_video_set_key_input(mMediaPlayer, 0);


}

QStringList PlayerVLC::get_tiempo()
{
    int aux;
    int ms = dur % 1000;
    aux=dur/1000;
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
