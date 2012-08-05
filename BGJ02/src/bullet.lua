require("util/helper")
require("util/vector")
require("entity")

Bullet = class("Bullet", Entity)

function Bullet:__init(position, ship_velocity, ship_rotation)
    Entity.__init(self)
    self.position = position
    self.own_velocity = Vector(1, 0):rotated(ship_rotation) * 250 --bulletspeed
    --self.velocity = ship_velocity + self.own_velocity
    self.velocity = ship_velocity * 0.2 + self.own_velocity

    self.physicsObject = {}

    self.particleSystem = love.graphics.newParticleSystem(resources.images["particle"], 128)
    self.particleSystem:start()
    self.particleSystem:setSizes(0.5, 0.01)
    self.particleSystem:setColors(255, 0, 0, 70)
    self.particleSystem:setEmissionRate(20)
    self.particleSystem:setParticleLife(0.5)
end

function Bullet:enablePhysics()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "dynamic")
    self.physicsObject.shape = love.physics.newCircleShape(5)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setSensor(true)
    self.physicsObject.fixture:setUserData(self)
    table.insert(self.world.physicsObjects, self.physicsObject)
end

function Bullet:update(dt)
    PolygonEntity.update(self, dt)
    if self.lifetime >= 3 then
        self:kill()
    end

    self.particleSystem:setPosition(self.position.x, self.position.y)

    self.particleSystem:update(dt)
end

function Bullet:draw()
    love.graphics.setColor(255, 20, 20, alpha)
    love.graphics.circle("line", self.position.x, self.position.y, 5)
    love.graphics.draw(self.particleSystem)
end
