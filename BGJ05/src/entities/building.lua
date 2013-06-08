require("util/helper")
require("util/vector")

require("scene/entity")

Building = class("Building", Entity)

function Building:__init(x, size)
    Entity.__init(self)
    self.position = Vector(x, 0)
    self.size = size
end

function Building:onAdd()
end

function Building:onUpdate(dt)
end

function Building:onDraw()
    love.graphics.setColor(100, 0, 0)
    love.graphics.rectangle("fill", self.position.x, 0, self.size.x, -self.size.y)
end
