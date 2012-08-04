require("util/helper")
require("util/vector")

Entity = class("Entity")

function Entity:__init()
    self.position = Vector()
    self.velocity = Vector()
    self.rotation = 0
    self.rotationSpeed = 0
    self.lifetime = 0
end

function Entity:update(dt)
    self.lifetime = self.lifetime + dt
    self.position = self.position + (self.velocity * dt)
    self.rotation = self.rotation + self.rotationSpeed * dt

    if self.physicsObject ~= nil and self.physicsObject.body ~= nil then
        self.position = Vector(self.physicsObject.body:getX(), self.physicsObject.body:getY())
    end
end

function Entity:draw()
end
