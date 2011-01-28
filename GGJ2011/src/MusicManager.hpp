#ifndef MUSICMANAGER_HPP
#define MUSICMANAGER_HPP

#include <boost/filesystem.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <string>
#include <map>

#include <SFML/Audio.hpp>

class MusicManager {
public:
	MusicManager();
	~MusicManager();
	void Play(const std::string& trackname);
	void Pause();
	void Stop();

	bool Register(const boost::filesystem::path& path, 
				  const std::string& trackname,
				  const float volume=100.f,
				  const float pitch=1.f);

private:
	std::string mCurrentMusic;

	// We can't use a boost::ptr_container here because sf::Music can't be
	// stored like that. :(
	std::map<std::string, boost::shared_ptr<sf::Music> > mMusicTracks;
};

#endif
