require("util/helper")
require("util/vector")
require("polygonentity")

Bullet = class("Bullet", Entity)

function Bullet:__init(position, velocity)
    Entity.__init(self)
    self.position = position
    self.velocity = velocity
end

--[[function Bullet:update()
    Entity
end]]

function Bullet:draw()
    for i = 5, 1, -1 do
        self:drawPart(i)
    end
end

function Bullet:drawPart(size)
    love.graphics.setColor(200, 255, 200, 255 - (5-size) * 50)
    local pos = Vector(self.position.x, self.position.y)
    pos = pos - self.velocity:normalized() * (5 - size) * 8
    love.graphics.circle("fill", pos.x, pos.y, size, 8)
end
