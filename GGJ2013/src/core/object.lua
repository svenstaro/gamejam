require("core/helper")

Object = class("Object")

function Object:__init()
    self.x = 0
    self.y = 0
    self.z = 0
    self.angle = 0
    self.group = nil
    self.name = ""
end

function Object:update(dt) end

function Object:draw()end

function Object:onAdd(group) end
function Object:onRemove(group) end
