
-- game levels

require("core/object")
level = require("data/levels/test0")

function hasBitFlag(set, flag)
    return set % (2*flag) >= flag
end

function removeBitFlag(set, flag)
    if set % (2*flag) >= flag then
        return set - flag
    end
    return set
end

function deepcopy(orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[deepcopy(orig_key)] = deepcopy(orig_value)
        end
        setmetatable(copy, deepcopy(getmetatable(orig)))
    else -- number, string, boolean, etc
        copy = orig
    end
    return copy
end

Level = class("Level", Object)

Tile = class("Tile")

function Tile:__init(batch, id, flip_x, flip_y)
    self.flip_x = flip_x or false
    self.flip_y = flip_y or false
    self.image = resources.images["level_" .. batch.name]
    local x, y = id - self.image
    self.quad = love.graphics.newQuad(j * (level.tilewidth + spacing_left), i * (level.tileheight + spacing_top),
                                                        level.tilewidth, level.tileheight,
                                                        resources.images.level:getWidth(), resources.images.level:getHeight())

    self.quad:flip(flip_x, flip_y)
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
                -- objectfactory:create()
            else
                for i = 0, level.height - 1 do
                    for j = 0, level.width - 1 do
                        if layer.data[1 + j + (i * level.width)] ~= 0 then
                            local index = layer.data[1 + j + (i * level.width)]
                            if layer.name == "meta" then
                                print("Meta index " .. index)
                            else

                                if not self.quads[index] then
                                    local flipHorizontallyFlag = 0x80000000
                                    local flipVerticallyFlag   = 0x40000000
                                    local flipHorizontal = false
                                    local flipVertical   = false

                                    if hasBitFlag(index, flipHorizontallyFlag) then
                                        flipHorizontal = true
                                    end

                                    if hasBitFlag(index, flipVerticallyFlag) then
                                        flipVertical = true
                                    end

                                    local realIndex = removeBitFlag(index, flipHorizontallyFlag)
                                          realIndex = removeBitFlag(realIndex, flipVerticallyFlag)

                                    print(realIndex)
                                    local quad = self.quads[realIndex]
                                    local quadCopy  = deepcopy(quad[2])
                                    quadCopy:flip(flipHorizontal, flipVertical)
                                        
                                    self.quads[index] = {quad[1], quadCopy}
                                end

                                local quad = self.quads[index]
                                print(quad[1])
                                print(quad[2])
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
