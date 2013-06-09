-- resources

require("util/helper")

Resources = class("Resources")

function Resources:__init(prefix)
    self.prefix = prefix
    self.imageQueue = {}
    self.musicQueue = {}
    self.fontQueue = {}
    self.soundQueue = {}

    self.images = {}
    self.music = {}
    self.fonts = {}
    self.sounds = {}
end

function Resources:addFont(name, src, size)
    self.fontQueue[name] = {src, size}
end

function Resources:addImage(name, src)
    self.imageQueue[name] = src
end

function Resources:addMusic(name, src)
    self.musicQueue[name] = src
end

function Resources:addSound(name, src)
    self.soundQueue[name] = src
end

function Resources:load(threaded)
    for name, pair in pairs(self.fontQueue) do
        self.fonts[name] = love.graphics.newFont(self.prefix .. pair[1], pair[2])
        self.fontQueue[name] = nil
    end

    for name, src in pairs(self.imageQueue) do
        self.images[name] = love.graphics.newImage(self.prefix .. src)
        self.imageQueue[name] = nil
    end

    for name, src in pairs(self.musicQueue) do
        self.music[name] = love.audio.newSource(self.prefix .. src)
        self.musicQueue[name] = nil
    end

    for name, src in pairs(self.soundQueue) do
        self.sounds[name] = love.sound.newSoundData(self.prefix .. src)
        self.soundQueue[name] = nil
    end
end

function Resources:makeSound(name)
    return love.audio.newSource(self.sounds[name])
end

function Resources:makeGradientImage(name, from, to, horizontal)
    local data = love.image.newImageData(2, 2)
    data:setPixel(0, 0, from[1], from[2], from[3], from[4] or 255)
    data:setPixel(horizontal and 0 or 1, horizontal and 1 or 0, from[1], from[2], from[3], from[4] or 255)
    data:setPixel(horizontal and 1 or 0, horizontal and 0 or 1, to[1], to[2], to[3], to[4] or 255)
    data:setPixel(1, 1, to[1], to[2], to[3], to[4] or 255)
    self.images[name] = love.graphics.newImage(data)
end
