#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <iostream>
#include <queue>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "Coordinates.hpp"

class ResourceManager {
public:
    ResourceManager();

    ~ResourceManager();

	bool AddImage(const boost::filesystem::path& path, const std::string& imgname,
		const float width, const float height);
	const sf::Image& GetImage(const std::string& filename);
	const std::vector<std::string> GetImageKeys();

	bool AddSoundBuffer(const boost::filesystem::path& path, const std::string& sound, const std::string& key="");
	const sf::SoundBuffer& GetSoundBuffer(const std::string& sound);

    void AddFont(sf::Font& font, std::string key);
    const sf::Font& GetFont(const std::string& key);

	void PlaySound(std::string key);
private:
	boost::ptr_map<std::string, sf::Image> mImages;
	boost::ptr_map<std::string, sf::Font> mFonts;
	boost::ptr_map<std::string, sf::SoundBuffer> mSoundBuffers;
	boost::ptr_map<std::string, sf::Sound> mSounds;
};

#endif
