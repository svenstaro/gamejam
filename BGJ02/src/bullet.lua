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
    love.graphics.setColor(200, 255, 200)
    love.graphics.circle("fill", self.position.x, self.position.y, 4, 8)
end
