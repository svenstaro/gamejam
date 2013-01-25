#ifndef SOUND_HPP
#define SOUND_HPP

#include <QObject>
#include <QRunnable>
#include <QString>

#include <alure.h>

class Sound : public QObject, public QRunnable {
    Q_PROPERTY(float volume READ volume WRITE setVolume)

public:

    Sound(QString resource_path);
    static void eos_callback(void *sound, ALuint unused2);

    void run();

public slots:
    void setVolume(float volume);
    float volume();

private:
    QString mResourcePath;
    int mIsDone;
    float mVolume; // 0 .. 1
};

#endif // SOUND_HPP
