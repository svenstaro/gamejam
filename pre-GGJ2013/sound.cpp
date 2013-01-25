#include "sound.hpp"

#include <QFile>
#include <QTemporaryFile>
#include <QDebug>

void Sound::eos_callback(void* sound, ALuint unused2)
{
    static_cast<Sound*>(sound)->mIsDone = 1;
    (void)unused2;
}

Sound::Sound(QString resource_path) :
    mResourcePath(resource_path),
    mIsDone(0),
    mVolume(1.f) {}

void Sound::run() {
    QFile audio_file(mResourcePath);
    if(audio_file.open(QIODevice::ReadOnly)) {
        QByteArray audio_data = audio_file.readAll();
        audio_file.close();

        ALuint src, buf;

        buf = alureCreateBufferFromMemory((const unsigned char*)audio_data.constData(), audio_data.size());

        alGenSources(1, &src);
        if(alGetError() != AL_NO_ERROR) {
            qDebug() << "Failed to create OpenAL source!";
        }

        if(!buf) {
            qDebug() << "Could not load sound: " << alureGetErrorString();
            alDeleteSources(1, &src);
        }

        alSourcei(src, AL_BUFFER, buf);
        alSourcef(src, AL_GAIN, 0.8f * mVolume);
        if(alurePlaySource(src, eos_callback, this) == AL_FALSE) {
            qDebug() << "Failed to start source!";
            alDeleteSources(1, &src);
            alDeleteBuffers(1, &buf);
        }

        while(!mIsDone) {
            alureSleep(0.125);
            alureUpdate();
        }

        alDeleteSources(1, &src);
        alDeleteBuffers(1, &buf);
    }
}

void Sound::setVolume(float volume) {
    mVolume = volume;
    if(mVolume > 1.f) mVolume = 1.f;
    if(mVolume < 0.f) mVolume = 0.f;
}

float Sound::volume() {
    return mVolume;
}
