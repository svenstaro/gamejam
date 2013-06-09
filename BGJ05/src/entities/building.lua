require("util/helper")
require("util/vector")

require("scene/entity")

Building = class("Building", Entity)

function Building:__init(x, size)
    Entity.__init(self)
    self.position = Vector(x, 0)
    self.size = size
    self.z = self.size.y / MAX_HEIGHT
    self.windows = math.floor(size.x / 70)
end

function Building:onAdd()
    self.antenna = LampAntenna()
    self.antenna.position = Vector(self.position.x + randf(0.1, 0.9) * self.size.x, -self.size.y)
    self.world:add(self.antenna)

    for i=1,math.random(0, 2) do
        local flipped = randf() < 0.5
        local lantern = LampChain(flipped)
        local x = self.position.x
        if not flipped then x = x + self.size.x end
        lantern.position = Vector(x + (flipped and -110 or 110), randf(0.4, 0.95) * -self.size.y)
        self.world:add(lantern)
    end
end

function Building:onUpdate(dt)
end

function Building:onDraw()
    local a = (1 - self.size.y / MAX_HEIGHT) * 0.5 * 255 + 100
    local a = 50
    love.graphics.setColor(a, a, a)
    love.graphics.rectangle("fill", self.position.x, 0, self.size.x, -self.size.y)

    setLightRendering(true, true)
    love.graphics.setColor(0, 0, 0)
    love.graphics.rectangle("fill", self.position.x, 0, self.size.x, -self.size.y)
    setLightRendering(false)

    local r = 3
    local s = self.size.x / (self.windows * (r+1) + 1)

    local S = r * s
    for x=0,self.windows-1 do
        for y=-100,-self.size.y,-S-s do
            local id = math.abs(math.floor(x * 27 + y * 31 + math.floor(self.position.x * 13)))
            local id2 = id % (string.len(WINDOWDATA)-1) + 1
            local on = string.byte(WINDOWDATA, id2)

            if on % 2 == 1 then
                local light = (on % 7 < 2 and 30 or -30)
                love.graphics.setColor(a+light, a+light, a+light)
                love.graphics.rectangle("fill", self.position.x + x * (r+1) * s + s, y, S, S)
            end
        end
    end
end
