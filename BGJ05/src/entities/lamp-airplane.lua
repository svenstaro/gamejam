require("util/helper")
require("util/vector")
require("entities/lamp")

LampAirplane = class("LampAirplane", Lamp)

function LampAirplane:__init()
    Lamp.__init(self)

    self.velocity = Vector(50, 0)
end

function LampAirplane:onUpdate(dt)
    Lamp.onUpdate(self, dt)

    self.position = self.position + self.velocity * dt
end

function LampAirplane:onDraw()
    Lamp.onDraw(self)

    local w = self.scale * 20
    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(resources.images.airplane, self.position.x, self.position.y, 0, self.scale, self.scale, 80, 150)
    love.graphics.setColor(0, 0, 0)
end
