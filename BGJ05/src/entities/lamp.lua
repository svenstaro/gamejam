require("util/helper")
require("util/vector")
require("scene/entity")

Lamp = class("Lamp", GlowEntity)

function Lamp:__init()
    GlowEntity.__init(self)
    self.z = -100

    self.burning = true
    self.wasActive = false
    self.energy = 1
    self.energyDrainSpeed = 0.8
    self.chargeRate = 1

    self.particleSystem = love.graphics.newParticleSystem(resources.images.particle, 128)
    self.particleSystem:start()
    self.particleSystem:setSizes(0.2, 0.9)
    self.particleSystem:setColors(
        240, 250, 50, 250,
        250, 10, 10, 0)
    self.particleSystem:setEmissionRate(100)
    self.particleSystem:setParticleLife(0.5)
    self.particleSystem:setSpread(0.5)

    self.glowColor = {255, 230, 0}
    self.maxGlowSize = 300
    self.burnoutFade = 1

    self.onOffPattern = {}
    x = 0
    while x < 1 do
        table.insert(self.onOffPattern, x)
        x = x + randf(0, 0.3) * randf(0, 0.3)
    end
end

function Lamp:burnout(mute)
    if self.burning then
        self.burning = false

        if not mute then
            local s = resources:makeSound("bell")
            states.game.bellStreak = states.game.bellStreak + 1
            states.game.bellTimeout = 1.5
            s:setPitch(0.5 + states.game.bellStreak * (1/12))
            s:play()
        end
    end
end

function Lamp:onUpdate(dt)
    if not self.burning then
        self.burnoutFade = math.max(0, self.burnoutFade - dt * 2)
    end

    if self.isNextLamp then
        self.wasActive = true
        self.energy = self.energy - dt * self.energyDrainSpeed
        if self.energy <= 0 then
            self:burnout()
        end
    end

    self.glowSize = self.maxGlowSize * self.energy

    if not self.isNextLamp and self.wasActive then
        self:burnout()
    end

    self.glow = self.burnoutFade > 0
    if not self.burning then
        local on = true
        for k, v in pairs(self.onOffPattern) do
            if v>self.burnoutFade then
                break
            end
            on = not on
        end
        self.glowStrength = on and self.burnoutFade or 0
    end

    self.particleSystem:setEmissionRate(self.glow and 20 or 0)
    self.particleSystem:update(dt)
    self.particleSystem:setPosition(self:getPosition().x, self:getPosition().y)
end

function Lamp:onDraw()
    love.graphics.setBlendMode("additive")
    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(self.particleSystem)
    love.graphics.setBlendMode("alpha")
end
