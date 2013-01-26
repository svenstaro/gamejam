require("core/helper")

Object = class("Object")

function Object:__init()
    self.x = 0
    self.y = 0
    self.z = 0
    self.angle = 0
    self.lifetime = 0
    self.group = nil
    self.name = ""
end

function Object:update(dt)
    self.lifetime = self.lifetime + dt

    if self.physicsObject ~= nil and self.physicsObject.body ~= nil then
        self.x = self.physicsObject.body:getX()
        self.y = self.physicsObject.body:getY()
        -- which direction do we wanna do this?
        --self.physicsObject.body:setPosition(self.x, self.y)
        --self.physicsObject.body:setAngle(self.angle)
    end
end

function Object:suicide()
    if self.group then self.group:remove(self) end
end

function Object:draw() end

function Object:onAdd(group) end
function Object:onRemove(group) end
