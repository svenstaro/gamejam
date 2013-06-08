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

    if self.physicsObject ~= nil and self.physicsObject.body ~= nil then
        self.position = Vector(self.physicsObject.body:getX(), self.physicsObject.body:getY()) + (self.velocity * dt)
        self.physicsObject.body:setPosition(self.position.x, self.position.y)
        self.physicsObject.body:setAngle(self.rotation)
    else
        self.position = self.position + (self.velocity * dt)
        self.rotation = self.rotation + self.rotationSpeed * dt
    end
end

function Entity:kill()
    if self.world then self.world:remove(self) end
end

function Entity:draw()
end
