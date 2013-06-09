require("util/helper")
require("util/vector")
require("entities/lamp")

LampAirplane = class("LampAirplane", Lamp)

function LampAirplane:__init()
    Lamp.__init(self)

    self.z = 1000
    self.mirror = math.random(0, 1) == 0
    self.glowSize = 500
    self.glowColor = {255, 255, 255}
    self.glowAngle = self.mirror and math.pi or 0
    self.glowImage = resources.images.spotlight
    self.scale = randf(0.3, 1.0)
    self.velocity = Vector(randf(50, 100) / self.scale * (self.mirror and -1 or 1), 0)
end

function LampAirplane:onUpdate(dt)
    Lamp.onUpdate(self, dt)

    self.position = self.position + self.velocity * dt
end

function LampAirplane:onDraw()
    Lamp.onDraw(self)

    local w = self.scale * 20
    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(resources.images.airplane, self.position.x, self.position.y, 0,
        self.scale * (self.mirror and -1 or 1), self.scale, 400, 200)
    love.graphics.setColor(0, 0, 0)
end
