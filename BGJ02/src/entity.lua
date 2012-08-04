require("util/helper")
require("util/vector")

Entity = class("Entity")

function Entity:__init()
    self.position = Vector()
    self.rotation = 0
    self.velocity = Vector()
end

function Entity:update(dt)
    self.position = self.position + (self.velocity * dt)
end

function Entity:draw()
end
