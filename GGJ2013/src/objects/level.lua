
-- game levels

require("core/object")
level = require("data/levels/test0")

Level = class("Level", Object)

Tile = class("Tile")

function Tile:__init(batch, id)
    self.image = resources.images["level_" .. batch.name]
    local x, y = id - self.image
    self.quad = love.graphics.newQuad(j * (level.tilewidth + spacing_left), i * (level.tileheight + spacing_top),
                                                        level.tilewidth, level.tileheight,
                                                        resources.images.level:getWidth(), resources.images.level:getHeight())
end

function Level:__init()
    self.x = 0
    self.y = 0
    self.z = 1
    self.angle = 0

    self.spritebatches = {}
    self.quads = {}

    self.tilesets = {}

    for t = 1, #level.tilesets do
        local tileset = level.tilesets[t]
        local name = tileset.name

        local image = resources.images["level_" .. name]
        if image then
            local batch = love.graphics.newSpriteBatch(image, level.width * level.height)
            self.spritebatches[name] = batch

            local spacing_top = 0
            local spacing_left = 0
            local tileset_tilewidth = ((tileset.imageheight - (tileset.imageheight % level.tileheight)) / level.tileheight)
            local tileset_tileheight = ((tileset.imagewidth - (tileset.imagewidth % level.tilewidth)) / level.tilewidth)
            for i = 0, tileset_tileheight - 1 do
                for j = 0, tileset_tilewidth - 1 do
                    if j ~= 0 then spacing_left = tileset.spacing else spacing_left = 0 end
                    if i ~= 0 then spacing_top = tileset.spacing else spacing_top = 0 end
                    local quad =
                        love.graphics.newQuad(j * (level.tilewidth + spacing_left),
                            i * (level.tileheight + spacing_top),
                            level.tilewidth, level.tileheight, image:getWidth(), image:getHeight())
                    self.quads[tileset.firstgid + j + (i * tileset_tileheight)] = {batch, quad}
                end
            end
        end
    end

    for l = 1, #level.layers do
        local layer = level.layers[l]
        print(layer.name)
        if layer.visible then
            if layer.type == "objectgroup" then

            else
                for i = 0, level.height - 1 do
                    for j = 0, level.width - 1 do
                        if layer.data[1 + j + (i * level.width)] ~= 0 then
                            local index = layer.data[1 + j + (i * level.width)]
                            if layer.name == "meta" then
                                print("Meta index " .. index)
                            else
                                local quad = self.quads[index]
                                print(quad[1])
                                quad[1]:addq(quad[2], j * level.tilewidth, i * level.tileheight)
                            end
                        end
                    end
                end
            end
        end
    end
end

function Level:update(dt)
end

function Level:draw()
    love.graphics.setColor(255, 255, 255)
    for k, v in pairs(self.spritebatches) do
        love.graphics.draw(v)
    end
end
