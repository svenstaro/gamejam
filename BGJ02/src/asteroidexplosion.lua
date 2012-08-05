require("util/helper")
require("util/vector")
require("entity")

AsteroidExplosion = class("AsteroidExplosion", Entity)

function AsteroidExplosion:__init(position, size)
    Entity.__init(self)
    self.position = position
    self.size = size

    self.particleSystem = love.graphics.newParticleSystem(resources.images["particle"], 128)
    self.particleSystem:start()
    self.particleSystem:setPosition(self.position.x, self.position.y)
    self.particleSystem:setSizes(0.2 * self.size, 1 * self.size)
    self.particleSystem:setSizeVariation(0.5)
    self.particleSystem:setColors(100, 100, 100, 70,
                                    0,   0,   0,  0)
    self.particleSystem:setEmissionRate(50)
    self.particleSystem:setParticleLife(1)
    self.particleSystem:setLifetime(0.2)
    self.particleSystem:setSpread(2 * math.pi)
    --self.particleSystem:setRadialAcceleration(500)
    self.particleSystem:setSpeed(50)
end

function AsteroidExplosion:update(dt)
    Entity.update(self, dt)
    if self.lifetime >= 3 then
        self:kill()
    end

    self.particleSystem:update(dt)
end

function AsteroidExplosion:draw()
    love.graphics.setBlendMode("additive")
    love.graphics.draw(self.particleSystem)
    love.graphics.setBlendMode("alpha")
end
