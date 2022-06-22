#ifndef PLAYERVLC_H
#define PLAYERVLC_H

#include <QObject>
#include <vlc/vlc.h>
#include <QWidget>
#include <QTimer>
#include <QUrl>
#include <QFrame>
#include <QDebug>
#include <QThread>
#include <QList>



class QTimer;
using VlcMedia = struct libvlc_media_t;
using VlcInstance = struct libvlc_instance_t;
using VlcMediaPlayer = struct libvlc_media_player_t;
using AudioDescription = struct libvlc_track_description_t;

class PlayerVLC : public QObject
{
    Q_OBJECT
public:
    enum Status
    {
        NONE,
        OPENING,
        BUFFERING,
        PLAYING,
        PAUSED,
        STOPPED,
        END_OF_MEDIA,
        ERROR
    };
    PlayerVLC( QObject *parent = nullptr );
    ~PlayerVLC();
    void setVideoOutput(QWidget *videoWidget);
    void setVideoOutputFrame(QFrame *videoWidget);
    void setMedia(const QUrl &url);
    qint64 mediaLenght();
    qint64 duration();
    float position();
    bool isPlaying();
    bool hasMedia();
    bool isMute();
    int volume();
    int status();
    /*Nuevas*/
    int getWidthVideo();
    int getHeightVideo();
    float getFpsVideo();
    int getLengthVideo();
    int getChapterVideo();
    void getAudio();
    void nextframe();
    void releaseMedia();
public slots:
    void setPosition(float pos);
    void setVolume(int volume);
    void setMute(bool mute);
    void setSlaveAudio(const QUrl &url);
    void play();
    void pause();
    void stop();
    void stop_mouse();
    QStringList get_tiempo();

signals:
    void positionChanged(float);
    void mediaStatusChanged(int);
    void durationChanged(qint64);

private:
    QTimer *mTimer;
    VlcMedia *mMedia;
    QWidget *mVideoWidget;
    QFrame *mvideoframe;
    VlcInstance *mInstance;
    VlcMediaPlayer *mMediaPlayer;
    AudioDescription *audio;
    int horas;
    int min;
    int seg;
    int dur;
};

#endif // PLAYERVLC_H
