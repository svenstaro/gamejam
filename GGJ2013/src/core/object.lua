require("core/helper")

Object = class("Object")

function Object:__init()
    self.x = 0
    self.y = 0
    self.z = 0
    self.angle = 0
    self.lifetime = 0
    self.group = nil
end

function Object:update(dt)
    self.lifetime = self.lifetime + dt
end

function Object:suicide()
    if self.group then self.group:remove(self) end
end

function Object:draw() end

function Object:onAdd(group) end
function Object:onRemove(group) end
