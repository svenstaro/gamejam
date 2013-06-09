require("util/helper")
require("util/vector")
require("entities/lamp")

LampAntenna = class("LampAntenna", Lamp)

function LampAntenna:__init()
    Lamp.__init(self)

    self.size = Vector(randf(10, 30), randf(100, 400))
    self.glowColor = {randf(100, 255), randf(100, 255), randf(100, 255)}
    self.glowSize = 500
end

function LampAntenna:onUpdate(dt)
    Lamp.onUpdate(self, dt)
end

function LampAntenna:onDraw()
    Lamp.onDraw(self)

    love.graphics.setColor(0, 0, 0)
    love.graphics.rectangle("fill", self.position.x - self.size.x/2, self.position.y - self.size.y,
                            self.size.x, self.size.y)

    love.graphics.setColor(unpack(self.glowColor))
    love.graphics.rectangle("fill", self.position.x - self.size.x/2, self.position.y - self.size.y,
                            self.size.x, 20)
end

function LampAntenna:getPosition()
    return Vector(self.position.x, self.position.y - self.size.y + 10)
end
