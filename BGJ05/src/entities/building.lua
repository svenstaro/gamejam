require("util/helper")
require("util/vector")

require("scene/entity")

Building = class("Building", Entity)

function Building:__init(x, size)
    Entity.__init(self)
    self.position = Vector(x, 0)
    self.size = size
    self.z = self.size.y / MAX_HEIGHT
end

function Building:onAdd()
end

function Building:onUpdate(dt)
end

function Building:onDraw()
    local a = (1 - self.size.y / MAX_HEIGHT) * 0.2 * 255
    love.graphics.setColor(a, a, a)
    love.graphics.rectangle("fill", self.position.x, 0, self.size.x, -self.size.y)
end
