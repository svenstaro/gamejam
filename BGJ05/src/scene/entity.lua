require("util/helper")
require("util/vector")

Entity = class("Entity")

function Entity:__init()
    self.position = Vector()
    self.z = 0
    self.velocity = Vector()
    self.rotation = 0
    self.rotationSpeed = 0
    self.scale = 1
    self.lifetime = 0
    self.dead = false
end

function Entity:update(dt)
    self.lifetime = self.lifetime + dt

    if self.physicsObject ~= nil and self.physicsObject.body ~= nil then
        self.position = Vector(self.physicsObject.body:getX(), self.physicsObject.body:getY())-- + (self.velocity * dt) -- < TODO wut
        self.physicsObject.body:setPosition(self.position.x, self.position.y)
        self.physicsObject.body:setAngle(self.rotation)
    else
        self.position = self.position + (self.velocity * dt)
        self.rotation = self.rotation + self.rotationSpeed * dt
    end

    self:onUpdate(dt)
end

function Entity:draw()
    self:onDraw()
end


function Entity:kill()
    self.dead = true
end

function Entity:onDraw() end
function Entity:onUpdate(dt) end
function Entity:onAdd() end
function Entity:onRemove() end
function Entity:getPosition()
    return self.position
end
