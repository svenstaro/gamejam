
-- game levels

require("core/object")
level = require("data/levels/test0")

Level = class("Level", Object)

function Level:__init()
    self.x = 0
    self.y = 0
    self.z = 1
    self.angle = 0
    self.spritebatch = love.graphics.newSpriteBatch(resources.images.level, level.width * level.height)
    self.quads = {}

    for t=1, #level.tilesets do
        local spacing_top = 0
        local spacing_left = 0
        local tileset_tilewidth = ((level.tilesets[t].imageheight - (level.tilesets[t].imageheight % level.tileheight)) / level.tileheight)
        local tileset_tileheight = ((level.tilesets[t].imagewidth - (level.tilesets[t].imagewidth % level.tilewidth)) / level.tilewidth)
        for i=0, tileset_tileheight - 1 do
            for j=0, tileset_tilewidth - 1 do
                if j ~= 0 then spacing_left = level.tilesets[t].spacing else spacing_left = 0 end
                if i ~= 0 then spacing_top = level.tilesets[t].spacing else spacing_top = 0 end
                self.quads[level.tilesets[t].firstgid + j + (i * tileset_tileheight)] = love.graphics.newQuad(j * (level.tilewidth + spacing_left), i * (level.tileheight + spacing_top),
                                                        level.tilewidth, level.tileheight,
                                                        resources.images.level:getWidth(), resources.images.level:getHeight())
            end
        end
    end

    for l=1, #level.layers do
        for i=0, level.height - 1 do
            for j=0,level.width - 1 do
                --print(level.layers[1].data[1 + j + (i * level.width)])
                if level.layers[l].data[1 + j + (i * level.width)] ~= 0 then
                    self.spritebatch:addq(self.quads[level.layers[l].data[1 + j + (i * level.width)]], j * level.tilewidth, i * level.tileheight)
                end
            end
        end
    end
end

function Level:update(dt)
end

function Level:draw()
    love.graphics.draw(self.spritebatch)
end
