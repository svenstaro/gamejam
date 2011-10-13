#include <algorithm>

#include "ResourceManager.hpp"

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

bool ResourceManager::AddImage(const boost::filesystem::path& path,
							   const std::string& imgname,
							   const float width,
							   const float height) {

	if(!boost::filesystem::is_regular_file(path/imgname)) {
		std::cerr << "Tried loading image path '" << (path/imgname).string() << "' but this image path doesn't exist!" << std::endl;
		exit(1);
	}

    // create Original file path
    std::string originalFile = (path / imgname).string();

    // if the optional param key is not given, use the basename as key
	std::string image_key = boost::filesystem::basename(originalFile);

    // Create Cache Paths
	//boost::filesystem::path cacheDir = (path / "cached").string();
	//std::string cacheFile = (cacheDir / image_key ).string()+".png";

    // if an image with that key already exists in the dictionary, return
    if(mImages.count(image_key) != 0) {
        return true;
    }

	sf::Image sfimage;
	bool success = sfimage.LoadFromFile(originalFile);
	if(!success) {
		std::cerr << "Image " << originalFile << " does not exists. Exit. "<< std::endl;
		exit(1);
	}
	/*bool cache = true;

	if(boost::filesystem::is_regular_file(cacheFile)) {
		// Load cached file
		bool success = sfimage.LoadFromFile(cacheFile);
		if (success && (int)sfimage.GetHeight() == (int)size.x && (int)sfimage.GetWidth() == (int)size.y) {
			cache = false;
			std::cout << "Image " << originalFile << " already exists. Not caching. "<< std::endl;
		} else if (success) {
			std::cout << "Image " << originalFile << " does not exist in the resolution "
					<< size.x << "x" << size.y << " but in " << sfimage.GetHeight() << "x" << sfimage.GetWidth() << "." << std::endl;
		}
	}

	if(cache){
		std::cout << ":: Caching image " << originalFile << std::endl;

		// Create cache directory
		boost::filesystem::create_directory(cacheDir.string());

		// Load, convert and save image (originalFile > cacheFile)
		Magick::Image mimage;
		mimage.backgroundColor(Magick::Color(0, 0, 0, 65535));
		mimage.density(Magick::Geometry(144, 144));
		mimage.read(originalFile);


		mimage.zoom(Magick::Geometry(std::max(size.x,size.y), std::max(size.x,size.y)));
		mimage.depth(8);
		mimage.write(cacheFile);

		// Load cached file
		sfimage.LoadFromFile(cacheFile);
	}
*/

	sfimage.SetSmooth(true);

	std::cout << "  Added image: "<<image_key << std::endl;
	// Save loaded Image in Dictionary
	mImages[image_key] = sfimage;

    return true;
}

const sf::Image& ResourceManager::GetImage(const std::string& img) {
	if(mImages.count(img) >= 1) {
		return mImages[img];
	} else {
		std::cerr << "Tried getting image '"<<img<<"' but this image doesn't exist!"<<std::endl;
		exit(1);
	}
}

const std::vector<std::string> ResourceManager::GetImageKeys() {
	std::vector<std::string> keys;

	for(auto iter = mImages.begin(); iter != mImages.end(); iter++) {
		if (iter->first != "view_border") {
			keys.push_back(iter->first);
		}
	}

	return keys;
}

bool ResourceManager::AddSoundBuffer(const boost::filesystem::path& path, const std::string& sound_name, const std::string& key) {
	if(!boost::filesystem::is_regular_file(path/sound_name)) {
		std::cerr << "Tried loading sound path'"+(path/sound_name).string()+"' but this sound path doesn't exist!" << std::endl;
		exit(1);
	}

    // create Original file path
    std::string originalFile = (path / sound_name).string();


    // if the optional param key is not given, use the basename as key
    std::string sound_key = "";
    if(key == "") {
        sound_key = boost::filesystem::basename(originalFile);
    } else {
        sound_key = key;
    }

    // if a sound with that key already exists in the dictionary, return
    if(mSoundBuffers.count(sound_key) != 0) {
        return true;
    }

	sf::SoundBuffer sound_buffer;
	if(!sound_buffer.LoadFromFile(originalFile)) {
		std::cerr << "Tried loading sound '"+(originalFile)+"' but it failed hardly!" << std::endl;
		exit(1);
	}

	mSoundBuffers[sound_key] = sound_buffer;

    return true;
}

const sf::SoundBuffer& ResourceManager::GetSoundBuffer(const std::string& sound) {
	if(mSoundBuffers.count(sound) >= 1) {
		return mSoundBuffers[sound];
	} else {
		std::cerr << "Tried getting sound '"+(sound)+"' but this sound doesn't exist!" << std::endl;
		exit(1);
	}
}

void ResourceManager::AddFont(sf::Font& font, std::string key) {
    mFonts[key] = font;
}

const sf::Font& ResourceManager::GetFont(const std::string& key) {
    return mFonts[key];
}

void ResourceManager::PlaySound(std::string key) {
	sf::Sound& s = mSounds[key];
	if(s.GetStatus() != sf::Sound::Playing) {
		// std::cout << "playing sound" << std::endl;
		s.SetBuffer(GetSoundBuffer(key));
		s.Play();
	}
}

void ResourceManager::StopSound(std::string key) {
	mSounds[key].Stop();
}

void ResourceManager::StopSounds() {
    BOOST_FOREACH(auto sound, mSounds) {
        sound->second->Stop();
    }
}
