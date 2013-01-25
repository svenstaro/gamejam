-- resources

require("core/helper")

Resources = class("Resources")

function Resources:__init(prefix)
    self.prefix = prefix
    self.imageQueue = {}
    self.musicQueue = {}
    self.fontQueue = {}
    self.shaderQueue = {}

    self.images = {}
    self.music = {}
    self.fonts = {}
    self.shaders = {}
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

function Resources:addShader(name, src)
    self.shaderQueue[name] = src
end

function Resources:createShader(name, source)
    if love.graphics.isSupported("pixeleffect") then
        self.shaders[name] = love.graphics.newPixelEffect(source)
    else
        self.shaders[name] = nil
    end
end

function Resources:sendShaderValue(name, key, value)
    if love.graphics.isSupported("pixeleffect") then
        self.shaders[name]:send(key, value)
    end
end

function Resources:makeGradientImage(name, from, to, horizontal)
    local data = love.image.newImageData(2, 2)
    data:setPixel(0, 0, from[1], from[2], from[3], from[4] or 255)
    data:setPixel(horizontal and 0 or 1, horizontal and 1 or 0, from[1], from[2], from[3], from[4] or 255)
    data:setPixel(horizontal and 1 or 0, horizontal and 0 or 1, to[1], to[2], to[3], to[4] or 255)
    data:setPixel(1, 1, to[1], to[2], to[3], to[4] or 255)
    self.images[name] = love.graphics.newImage(data)
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

    for name, src in pairs(self.shaderQueue) do
        content = ""
        for line in love.filesystem.lines(self.prefix .. src) do
            content = content .. line .. "\n"
        end
        self:createShader(name, content)
        self.shaderQueue[name] = nil
    end
end
