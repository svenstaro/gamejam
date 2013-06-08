require("util/helper")
require("util/vector")

require("entities/glowentity")

Wisp = class("Wisp", GlowEntity)

function Wisp:__init()
    GlowEntity.__init(self)
    self.physicsObject = {}
    self.z = -1000

    self.particleSystem = love.graphics.newParticleSystem(resources.images["ring"], 128)
    self.particleSystem:start()
    self.particleSystem:setSizes(0.2, 0.01)
    self.particleSystem:setColors(
        127, 20, 209, 250,
        15, 128, 215, 0)
    self.particleSystem:setEmissionRate(100)
    self.particleSystem:setParticleLife(0.5)
    self.particleSystem:setSpread(0.5)

    self.glowSize = 255

    self.nextLamp = nil
end

function Wisp:onAdd()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "dynamic")
    self.physicsObject.body:setLinearDamping(1)
    self.physicsObject.shape = love.physics.newCircleShape(10)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setSensor(true)
    self.physicsObject.fixture:setUserData(self)
    table.insert(self.world.physicsObjects, self.physicsObject)
end

function Wisp:onUpdate(dt)
    self.particleSystem:update(dt)
    self.particleSystem:setPosition(self.position.x, self.position.y)

    local dNext = (self.nextLamp == nil and 0 or self.position:dist(self.nextLamp.position))
    self.nextLamp = nil
    for k, lamp in pairs(self.world:findByType("Lamp", true)) do
        lamp.isNextLamp = false
        local d = self.position:dist(lamp.position)
        if d < LIGHTRANGE and lamp.burning and (self.nextLamp == nil or d <= dNext) then
            self.nextLamp = lamp
        end
    end

    if self.nextLamp then
        self.nextLamp.isNextLamp = true

        local f = self.nextLamp.position - self.position
        local l = math.max(10, f:len())
        f = f * (1 - l / LIGHTRANGE) * 3
        self.physicsObject.body:applyForce(f:unpack())
    end
end

function Wisp:onDraw()
    love.graphics.setColor(255, 255, 255)
    love.graphics.circle("fill", self.position.x, self.position.y, 20)

    love.graphics.setBlendMode("additive")
    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(self.particleSystem)
    love.graphics.setBlendMode("alpha")

    if DEBUG then
        love.graphics.setColor(255, 0, 0)
        love.graphics.circle("fill", self.physicsObject.body:getX(), self.physicsObject.body:getY(),
                             self.physicsObject.shape:getRadius())

        if self.nextLamp then
            love.graphics.setColor(255, 255, 255)
            love.graphics.setLineWidth(6)
            love.graphics.line(self.nextLamp.position.x, self.nextLamp.position.y, self.position.x, self.position.y)
            love.graphics.setLineWidth(1)
        end
    end
end

function Wisp:move(vec)
    -- self.position = self.position + vec
    --self.physicsObject.body:applyForce((vec*30):unpack())
end

function Wisp:jump(dir)
    if self.nextLamp then
        local b = self.physicsObject.body
        local v = (dir:len() == 0 and Vector(b:getLinearVelocity()):normalized() or dir)
        v = v * 30
        b:applyLinearImpulse(v:unpack())
        self.nextLamp:burnout()
    end
end
