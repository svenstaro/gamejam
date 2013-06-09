require("util/helper")
require("util/vector")
require("scene/entity")
require("entities/lamp")

LampChain = class("LampChain", Lamp)

function LampChain:__init(flipped)
    Lamp.__init(self)
    self.flipped = flipped
    self.positionChain0 = Vector()
    self.positionChain1 = Vector()
    self.positionChain2 = Vector()
    self.positionChain3 = Vector()
    self.positionLantern = Vector()
    self.rotationChain0 = 0
    self.rotationChain1 = 0
    self.rotationChain2 = 0
    self.rotationChain3 = 0
    self.rotationLantern = 0
    self.physicsObjectAttachment = {}
    self.physicsObjectChain0 = {}
    self.physicsObjectChain1 = {}
    self.physicsObjectChain2 = {}
    self.physicsObjectChain3 = {}
    self.physicsObjectLantern = {}
    self.z = -100
    self.mirror = false
    self.scale = 0.1
    self.chain_scale = 0.3
    self.impulse = randf(-10, 10)
    self.image_attachment = resources.images.chainlamp
    self.image_lantern = resources.images.lantern
    self.image_chain0 = resources.images.chain0
    self.image_chain1 = resources.images.chain1

    self.glowColor = {255, 200, 0}

    self.particleSystem = love.graphics.newParticleSystem(resources.images.particle, 1000)
    self.particleSystem:start()
    self.particleSystem:setSizes(1, 0.9)
    self.particleSystem:setColors(
        230, 230, 100, 100,
        200, 120, 100, 0)
    self.particleSystem:setParticleLife(1.0)
end

function LampChain:onAdd()
    self.positionChain0 = self.position + Vector(0, 20)
    self.positionChain1 = self.positionChain0 + Vector(0, 20)
    self.positionChain2 = self.positionChain1 + Vector(0, 20)
    self.positionChain3 = self.positionChain2 + Vector(0, 20)
    self.positionLantern = self.positionChain3 + Vector(0, 20)

    local love08bugfix = 64 -- TODO
    local offset = -5

    self.physicsObjectAttachment.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "static")
    self.physicsObjectAttachment.shape = love.physics.newRectangleShape(10, 10)
    self.physicsObjectAttachment.fixture = love.physics.newFixture(self.physicsObjectAttachment.body, self.physicsObjectAttachment.shape, 1)
    self.physicsObjectAttachment.fixture:setSensor(true)
    self.physicsObjectAttachment.fixture:setUserData(self)

    self.physicsObjectChain0.body = love.physics.newBody(self.world.physicsWorld, self.positionChain0.x, self.positionChain0.y, "dynamic")
    self.physicsObjectChain0.shape = love.physics.newRectangleShape(10, 10)
    self.physicsObjectChain0.fixture = love.physics.newFixture(self.physicsObjectChain0.body, self.physicsObjectChain0.shape, 1)
    self.physicsObjectChain0.fixture:setSensor(true)
    self.physicsObjectChain0.fixture:setUserData(self)
    self.physicsObjectChain0.joint = love.physics.newRopeJoint(self.physicsObjectAttachment.body, self.physicsObjectChain0.body,
                                                                    self.position.x * love08bugfix, self.position.y * love08bugfix,
                                                                    self.positionChain0.x * love08bugfix, self.positionChain0.y * love08bugfix + offset,
                                                                    20)

    self.physicsObjectChain1.body = love.physics.newBody(self.world.physicsWorld, self.positionChain1.x, self.positionChain1.y, "dynamic")
    self.physicsObjectChain1.shape = love.physics.newRectangleShape(10, 10)
    self.physicsObjectChain1.fixture = love.physics.newFixture(self.physicsObjectChain1.body, self.physicsObjectChain1.shape, 1)
    self.physicsObjectChain1.fixture:setSensor(true)
    self.physicsObjectChain1.fixture:setUserData(self)
    self.physicsObjectChain1.joint = love.physics.newRopeJoint(self.physicsObjectChain0.body, self.physicsObjectChain1.body,
                                                                    self.positionChain0.x * love08bugfix, self.positionChain0.y * love08bugfix,
                                                                    self.positionChain1.x * love08bugfix, self.positionChain1.y * love08bugfix + offset,
                                                                    20)

    self.physicsObjectChain2.body = love.physics.newBody(self.world.physicsWorld, self.positionChain2.x, self.positionChain2.y, "dynamic")
    self.physicsObjectChain2.shape = love.physics.newRectangleShape(10, 10)
    self.physicsObjectChain2.fixture = love.physics.newFixture(self.physicsObjectChain2.body, self.physicsObjectChain2.shape, 1)
    self.physicsObjectChain2.fixture:setSensor(true)
    self.physicsObjectChain2.fixture:setUserData(self)
    self.physicsObjectChain2.joint = love.physics.newRopeJoint(self.physicsObjectChain1.body, self.physicsObjectChain2.body,
                                                                    self.positionChain1.x * love08bugfix, self.positionChain1.y * love08bugfix,
                                                                    self.positionChain2.x * love08bugfix, self.positionChain2.y * love08bugfix + offset,
                                                                    20)

    self.physicsObjectChain3.body = love.physics.newBody(self.world.physicsWorld, self.positionChain3.x, self.positionChain3.y, "dynamic")
    self.physicsObjectChain3.shape = love.physics.newRectangleShape(10, 10)
    self.physicsObjectChain3.fixture = love.physics.newFixture(self.physicsObjectChain3.body, self.physicsObjectChain3.shape, 1)
    self.physicsObjectChain3.fixture:setSensor(true)
    self.physicsObjectChain3.fixture:setUserData(self)
    self.physicsObjectChain3.joint = love.physics.newRopeJoint(self.physicsObjectChain2.body, self.physicsObjectChain3.body,
                                                                    self.positionChain2.x * love08bugfix, self.positionChain2.y * love08bugfix,
                                                                    self.positionChain3.x * love08bugfix, self.positionChain3.y * love08bugfix + offset,
                                                                    20)

    self.physicsObjectLantern.body = love.physics.newBody(self.world.physicsWorld, self.positionLantern.x, self.positionLantern.y, "dynamic")
    self.physicsObjectLantern.shape = love.physics.newRectangleShape(10, 10)
    self.physicsObjectLantern.fixture = love.physics.newFixture(self.physicsObjectLantern.body, self.physicsObjectLantern.shape, 1)
    self.physicsObjectLantern.fixture:setSensor(true)
    self.physicsObjectLantern.fixture:setUserData(self)
    self.physicsObjectLantern.joint = love.physics.newRopeJoint(self.physicsObjectChain3.body, self.physicsObjectLantern.body,
                                                                    self.positionChain3.x * love08bugfix, self.positionChain3.y * love08bugfix,
                                                                    self.positionLantern.x * love08bugfix, self.positionLantern.y * love08bugfix + offset,
                                                                    40)

    self.physicsObjectLantern.body:setAngularDamping(4)
    self.physicsObjectLantern.body:applyLinearImpulse(self.impulse, 0)
end

function LampChain:onUpdate(dt)
    Lamp.onUpdate(self, dt)

    self.position = Vector(self.physicsObjectAttachment.body:getX(), self.physicsObjectAttachment.body:getY())
    self.rotation = self.physicsObjectAttachment.body:getAngle()

    self.positionChain0 = Vector(self.physicsObjectChain0.body:getX(), self.physicsObjectChain0.body:getY())
    self.rotationChain0 = self.physicsObjectChain0.body:getAngle()

    self.positionChain1 = Vector(self.physicsObjectChain1.body:getX(), self.physicsObjectChain1.body:getY())
    self.rotationChain1 = self.physicsObjectChain1.body:getAngle()

    self.positionChain2 = Vector(self.physicsObjectChain2.body:getX(), self.physicsObjectChain2.body:getY())
    self.rotationChain2 = self.physicsObjectChain2.body:getAngle()

    self.positionChain3 = Vector(self.physicsObjectChain3.body:getX(), self.physicsObjectChain3.body:getY())
    self.rotationChain3 = self.physicsObjectChain3.body:getAngle()

    self.positionLantern = Vector(self.physicsObjectLantern.body:getX(), self.physicsObjectLantern.body:getY())
    self.rotationLantern = self.physicsObjectLantern.body:getAngle()

    self.particleSystem:update(dt)
    self.particleSystem:setEmissionRate(self.glow and 20 or 0)
    self.particleSystem:setPosition(self.positionLantern.x, self.positionLantern.y + 10)
end

function LampChain:onRemove()
    self.physicsObjectAttachment.body:destroy()
    self.physicsObjectChain0.body:destroy()
    self.physicsObjectChain1.body:destroy()
    self.physicsObjectChain2.body:destroy()
    self.physicsObjectChain3.body:destroy()
    self.physicsObjectLantern.body:destroy()
end

function LampChain:onDraw()
    love.graphics.setColor(255, 255, 255)

    love.graphics.draw(self.image_attachment,
                       self.position.x, self.position.y + 40, self.rotation,
                       self.scale * 3 * (self.flipped and -1 or 1), self.scale * 3,
                       self.image_attachment:getWidth() - 5, self.image_attachment:getHeight()/2)

    love.graphics.draw(self.image_chain0,
                       self.positionChain0.x, self.positionChain0.y, self.rotationChain0 + math.pi/2,
                       self.chain_scale, self.chain_scale,
                       self.image_chain0:getWidth() / 2, self.image_chain0:getHeight() / 2)

    love.graphics.draw(self.image_chain1,
                       self.positionChain1.x, self.positionChain1.y, self.rotationChain1 + math.pi/2,
                       self.chain_scale, self.chain_scale,
                       self.image_chain1:getWidth() / 2, self.image_chain1:getHeight() / 2)

    love.graphics.draw(self.image_chain0,
                       self.positionChain2.x, self.positionChain2.y, self.rotationChain2 + math.pi/2,
                       self.chain_scale, self.chain_scale,
                       self.image_chain0:getWidth() / 2, self.image_chain0:getHeight() / 2)

    love.graphics.draw(self.image_chain1,
                       self.positionChain3.x, self.positionChain3.y, self.rotationChain3 + math.pi/2,
                       self.chain_scale, self.chain_scale,
                       self.image_chain1:getWidth() / 2, self.image_chain1:getHeight() / 2)

    love.graphics.draw(self.image_lantern,
                       self.positionLantern.x, self.positionLantern.y, self.rotationLantern,
                       self.scale, self.scale,
                       self.image_lantern:getWidth() / 2, self.image_lantern:getHeight() / 2)

    love.graphics.setBlendMode("additive")
    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(self.particleSystem)
    love.graphics.setBlendMode("alpha")

    if DEBUG then
        love.graphics.setColor(255, 0, 0)
        love.graphics.polygon("fill", self.physicsObjectAttachment.body:getWorldPoints(self.physicsObjectAttachment.shape:getPoints()))
        love.graphics.polygon("fill", self.physicsObjectChain0.body:getWorldPoints(self.physicsObjectChain0.shape:getPoints()))
        love.graphics.polygon("fill", self.physicsObjectChain1.body:getWorldPoints(self.physicsObjectChain1.shape:getPoints()))
        love.graphics.polygon("fill", self.physicsObjectChain2.body:getWorldPoints(self.physicsObjectChain2.shape:getPoints()))
        love.graphics.polygon("fill", self.physicsObjectChain3.body:getWorldPoints(self.physicsObjectChain3.shape:getPoints()))
        love.graphics.polygon("fill", self.physicsObjectLantern.body:getWorldPoints(self.physicsObjectLantern.shape:getPoints()))
        love.graphics.setColor(255, 255, 255)
    end
end

function LampChain:getPosition()
    return self.positionLantern
end
