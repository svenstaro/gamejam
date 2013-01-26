
-- game levels

require("core/object")
require("objects/walltile")

Level = class("Level", Object)

function Level:__init(file, group)
    level = require("data/levels/" .. file)

    self.x = 0
    self.y = 0
    self.z = 0
    self.angle = 0

    self.tiles = ObjectGroup()
    self.spritebatches = {}
    self.quads = {}

    self.tilesets = {}

    local meta_firstgid = 0

    for t = 1, #level.tilesets do
        local tileset = level.tilesets[t]
        local name = tileset.name

        if name == "meta" then
            meta_firstgid = tileset.firstgid
        end

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
        if layer.visible then
            if layer.type == "objectgroup" then
                for i = 1, #layer.objects do
                    -- name, x, y, properties, type, width, height
                    local obj = layer.objects[i]
                    local cx, cy = obj.x + obj.width / 2, obj.y + obj.height / 2

                    local object = nil

                    if obj.type == "door" then
                        object = Door(obj.width > obj.height and 0 or math.pi / 2)
                        object.x = cx
                        object.y = cy
                    end

                    if object then
                        object.name = obj.name
                        group:add(object)
                    else
                        print("Unhandled object type: " .. obj.type)
                    end

                end
            else
                for i = 0, level.height - 1 do
                    for j = 0, level.width - 1 do
                        if layer.data[1 + j + (i * level.width)] ~= 0 then
                            local index = layer.data[1 + j + (i * level.width)]
                            if layer.name == "meta" then
                                index = index - meta_firstgid

                                -- wall tile
                                if index == 1 then
                                    group:add(WallTile(j * level.tilewidth, i * level.tileheight))
                                end
                            else
                                local quad = self.quads[index]
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
