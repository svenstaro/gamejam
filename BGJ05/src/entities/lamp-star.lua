require("util/helper")
require("util/vector")
require("entities/lamp")

LampStar = class("LampStar", Lamp)

function LampStar:__init()
    Lamp.__init(self)
    self.scale = randf(20, 50)
    self.glowColor = {255, 255, 255}
end

function LampStar:onUpdate(dt)
    Lamp.onUpdate(self, dt)
end

function LampStar:onDraw()
    Lamp.onDraw(self)

    love.graphics.setColor(255, 255, 255)
    love.graphics.rectangle("fill", self.position.x - self.scale/2, self.position.y - self.scale/2, self.scale, self.scale)
    love.graphics.setColor(0, 0, 0)
end
