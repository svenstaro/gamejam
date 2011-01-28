#include "MusicManager.hpp"
#include <iostream>

MusicManager::MusicManager() {}

MusicManager::~MusicManager() {}

void MusicManager::Play(const std::string& trackname) {
	// Only attempt to do something if the trackname can be found in the map,
	// i.e. if it has been already been registered.
	if(mMusicTracks.count(trackname)) {

		// Only attempt to do something if we are not trying to play the
		// current track again.
		if(mCurrentMusic != trackname) {

			// Do not stop music if there is no current music, i.e., if this 
			// is the first track to play since starting the game.
			if(!mCurrentMusic.empty()) {
				std::cout << "Stopping music " << mCurrentMusic << std::endl;
				mMusicTracks[mCurrentMusic]->Stop();
			}

			mCurrentMusic = trackname;
			std::cout << "Playing track '" << trackname << "'" << std::endl;
			mMusicTracks[mCurrentMusic]->Play();
		}
	} else {
		std::cerr << "Tried playing unregistered music track." << std::endl;
		exit(1);
	}
}

void MusicManager::Pause() {
	mMusicTracks[mCurrentMusic]->Pause();
}

void MusicManager::Stop() {
	mMusicTracks[mCurrentMusic]->Stop();
}

bool MusicManager::Register(const boost::filesystem::path& path,
							const std::string& trackname,
							const float volume,
							const float pitch) {
	if(!boost::filesystem::is_regular_file(path/trackname)) {
		std::cerr << "Tried registering music path '"+(path/trackname).string()+"' but this music path doesn't exist!" << std::endl;
		exit(1);
	}

    // if music already is registered, return
    if(mMusicTracks.count((path/trackname).string()) != 0) {
        return true;
    }

	// log output
	std::cout << "Registering music track '" << (path/trackname).string() << "'" << std::endl;

	boost::shared_ptr<sf::Music> music(new sf::Music());
	mMusicTracks[trackname] = music;
	mMusicTracks[trackname]->OpenFromFile((path/trackname).string());
	mMusicTracks[trackname]->SetPitch(pitch);
	mMusicTracks[trackname]->SetVolume(volume);
	mMusicTracks[trackname]->SetLoop(true);
	return true;
}
