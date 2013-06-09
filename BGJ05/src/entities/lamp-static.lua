require("util/helper")
require("util/vector")
require("entities/lamp")

LampStatic = class("LampStatic", Lamp)

function LampStatic:__init()
    Lamp.__init(self)
end

function LampStatic:onUpdate(dt)
    Lamp.onUpdate(self, dt)

    self.z = - 100 - self.position.y / 100000
end

function LampStatic:onDraw()
    Lamp.onDraw(self)

    local img = resources.images.streetlamp1
    local scale = 0.4
    local w = scale * 20
    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(img, self.position.x, self.position.y, 0, scale, scale, 80, 150)
    love.graphics.setColor(0, 0, 0)
    love.graphics.rectangle("fill", self.position.x - w/2, self.position.y + scale * 150, w, -self.position.y)
end
