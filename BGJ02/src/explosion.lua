require("util/helper")
require("util/vector")
require("entity")

Explosion = class("Explosion", Entity)

function Explosion:__init(position, size)
    Entity.__init(self)
    self.position = position
    self.size = size

    self.particleSystem = love.graphics.newParticleSystem(resources.images["particle"], 128)
    self.particleSystem:start()
    self.particleSystem:setPosition(self.position.x, self.position.y)
    self.particleSystem:setSizes(0.5 * self.size, 3 * self.size)
    self.particleSystem:setColors(100, 100, 100, 70,
                                    0,   0,   0,  0)
    self.particleSystem:setEmissionRate(20)
    self.particleSystem:setParticleLife(2)
    self.particleSystem:setSpeed(50)
    print("const")
end

function Explosion:update(dt)
    Entity.update(self, dt)
    if self.lifetime >= 3 then
        self:kill()
    end

    self.particleSystem:update(dt)

    print(self.particleSystem:count())
end

function Explosion:draw()
    love.graphics.draw(self.particleSystem)
end
