#include "ResourceManager.hpp"
#include <algorithm>

ResourceManager::ResourceManager() {}
ResourceManager::~ResourceManager() {}

bool ResourceManager::AddTexture(const boost::filesystem::path& path,
							   const std::string& imgname,
							   const float width,
							   const float height) {

	if(!boost::filesystem::is_regular_file(path/imgname)) {
		std::cerr << "Tried loading image path '" << (path/imgname).string() << "' but this image path doesn't exist!" << std::endl;
		exit(1);
	}

	// convert coords
	const Vector2D size(Coordinates::WorldFloatToWorldPixel(Vector2D(width, height)));

    // create Original file path
    std::string originalFile = (path / imgname).string();

    // if the optional param key is not given, use the basename as key
	std::string image_key = boost::filesystem::basename(originalFile);

    // Create Cache Paths
	boost::filesystem::path cacheDir = (path / "cached").string();
    std::string cacheFile = (cacheDir / image_key ).string()+".png";

    // if an image with that key already exists in the dictionary, return
    if(mTextures.count(image_key) != 0) {
        return true;
    }

	sf::Texture sftexture;
	bool cache = true;

	if(boost::filesystem::is_regular_file(cacheFile)) {
		// Load cached file
		bool success = sftexture.LoadFromFile(cacheFile);
		if (success && (int)sftexture.GetHeight() == (int)size.x && (int)sftexture.GetWidth() == (int)size.y) {
			cache = false;
			std::cout << "Texture " << originalFile << " already exists. Not caching. "<< std::endl;
		} else if (success) {
			std::cout << "Texture " << originalFile << " does not exist in the resolution "
					<< size.x << "x" << size.y << " but in " << sftexture.GetHeight() << "x" << sftexture.GetWidth() << "." << std::endl;
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
		sftexture.LoadFromFile(cacheFile);
	}


	sftexture.SetSmooth(true);


	//std::cout << "  Added image: "<<image_key << std::endl;
	// Save loaded Texture in Dictionary
	mTextures[image_key] = sftexture;

    return true;
}

const sf::Texture& ResourceManager::GetTexture(const std::string& img) {
	if(mTextures.count(img) >= 1) {
		return mTextures[img];
	} else {
		std::cerr << "Tried getting image '"<<img<<"' but this image doesn't exist!"<<std::endl;
		exit(1);
	}
}

const std::vector<std::string> ResourceManager::GetTextureKeys() {
	std::vector<std::string> keys;

	for(auto iter = mTextures.begin(); iter != mTextures.end(); iter++) {
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
