require("util/helper")
require("util/vector")
require("scene/entity")

LampChain = class("LampChain", GlowEntity)

function LampChain:__init()
    GlowEntity.__init(self)
    self.physicsObject = {}
    self.z = -100
    self.scale = 0.1
    self.image = resources.images.lantern

    self.particleSystem = love.graphics.newParticleSystem(resources.images.particle, 128)
    self.particleSystem:start()
    self.particleSystem:setSizes(0.2, 0.9)
    self.particleSystem:setColors(
        240, 250, 50, 250,
        250, 10, 10, 0)
    self.particleSystem:setEmissionRate(100)
    self.particleSystem:setParticleLife(0.5)
    self.particleSystem:setSpread(0.5)

    self.glowColor = {255, 200, 0}
end

function LampChain:onAdd()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "dynamic")
    self.physicsObject.shape = love.physics.newRectangleShape(10, 10)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setSensor(true)
    self.physicsObject.fixture:setUserData(self)
    table.insert(self.world.physicsObjects, self.physicsObject)
end

function LampChain:onUpdate(dt)
    self.particleSystem:update(dt)
    self.particleSystem:setPosition(self.position.x, self.position.y)
end

function LampChain:onDraw()
    love.graphics.draw(resources.images.lantern,
                       self.position.x - (self.image:getWidth() / 2) * self.scale,
                       self.position.y - (self.image:getHeight() / 2) * self.scale,
                       self.rotation, self.scale)

    love.graphics.setBlendMode("additive")
    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(self.particleSystem)
    love.graphics.setBlendMode("alpha")

    if DEBUG then
        love.graphics.setColor(255, 0, 0)
        love.graphics.polygon("fill", self.physicsObject.body:getWorldPoints(self.physicsObject.shape:getPoints()))
        love.graphics.setColor(255, 255, 255)
    end
end
