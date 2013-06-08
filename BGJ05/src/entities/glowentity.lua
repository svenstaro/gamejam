require("util/helper")
require("util/vector")

GlowEntity = class("GlowEntity", Entity)

function GlowEntity:__init()
    Entity.__init(self)
    self.glow = true
    self.glowSize = 100
    self.glowColor = {255, 255, 255}
    self.glowStrength = 1
end

function GlowEntity:draw()
    if self.glow then
        setLightRendering(true)

        local c = pack(unpack(self.glowColor))
        c[4] = (c[4] or 255) * self.glowStrength
        love.graphics.setColor(unpack(c))

        local s = resources.images.particle:getWidth()
        love.graphics.draw(resources.images.particle_square, self:getPosition().x, self:getPosition().y, 0,
            self.glowSize / s, self.glowSize / s, s / 2, s / 2)

        setLightRendering(false)
    end

    Entity.draw(self)
end
