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
end

function Wisp:onAdd()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "dynamic")
    self.physicsObject.shape = love.physics.newCircleShape(10)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setSensor(true)
    self.physicsObject.fixture:setUserData(self)
    table.insert(self.world.physicsObjects, self.physicsObject)
end

function Wisp:onUpdate(dt)
    if self.physicsObject ~= nil and self.physicsObject.body ~= nil then
        self.position = Vector(self.physicsObject.body:getX(), self.physicsObject.body:getY()) + (self.velocity * dt)
        self.physicsObject.body:setPosition(self.position.x, self.position.y)
        self.physicsObject.body:setAngle(self.rotation)
    else
        self.position = self.position + (self.velocity * dt)
        self.rotation = self.rotation + self.rotationSpeed * dt
    end

    self.particleSystem:update(dt)
    self.particleSystem:setPosition(self.position.x, self.position.y)
end

function Wisp:onDraw()
    love.graphics.setColor(255, 255, 255)
    love.graphics.circle("fill", self.position.x, self.position.y, 20)

    love.graphics.setBlendMode("additive")
    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(self.particleSystem)
    love.graphics.setBlendMode("alpha")
end

function Wisp:move(vec)
    -- self.position = self.position + vec
    self.physicsObject.body:applyLinearImpulse(vec.x, vec.y)
end
