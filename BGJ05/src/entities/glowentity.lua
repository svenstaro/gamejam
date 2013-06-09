require("util/helper")
require("util/vector")

GlowEntity = class("GlowEntity", Entity)

function GlowEntity:__init()
    Entity.__init(self)
    self.glow = true
    self.glowSize = 100
    self.glowColor = {255, 255, 255}
    self.glowStrength = 1
    self.glowVariance = 0.2
    self.glowOffset = randf(0, 100)
    self.glowSpeed = 1
    self.glowAngle = 0
    self.glowImage = resources.images.particle_square
end

function GlowEntity:draw()
    if self.glow then
        setLightRendering(true)

        local c = pack(unpack(self.glowColor))
        c[4] = (c[4] or 255) * self.glowStrength
        love.graphics.setColor(unpack(c))

        local s = self.glowImage:getWidth()
        local scale = self.glowSize / s * (math.sin((self.glowOffset + self.lifetime) * self.glowSpeed) * self.glowVariance + 1)
        love.graphics.draw(self.glowImage, self:getPosition().x, self:getPosition().y, self.glowAngle,
            scale, scale, s / 2, s / 2)

        setLightRendering(false)
    end

    Entity.draw(self)
end
