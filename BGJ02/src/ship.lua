require("util/helper")
require("util/vector")
require("polygonentity")
require("bullet")

Ship = class("Ship", PolygonEntity)

function Ship:__init()
    PolygonEntity.__init(self)
    self.rotation = -math.pi / 2

    self:addPoint(Vector(10, 0))
    self:addPoint(Vector(-10, -7))
    self:addPoint(Vector(-3, 0))
    self:addPoint(Vector(-10, 7))

    self.physicsObject = {}
    self.timeUntilShoot = 0

    turn_speed = math.pi
    --ai_turn_speed = math.pi * 2
    --max_ship_speed = 2

    self.lastVelocity = Vector(0, 0) -- the velocity from last frame so we know whether we accelerated

    self.particleSystem = love.graphics.newParticleSystem(resources.images["particle"], 128)
    self.particleSystem:stop()
    self.particleSystem:setSizes(0.2, 0.01)
    self.particleSystem:setColors(
        240, 250, 50, 250, 
        250, 10, 10, 0)
    self.particleSystem:setEmissionRate(100)
    self.particleSystem:setParticleLife(0.5)
    self.particleSystem:setSpread(0.5)
end

function Ship:enablePhysics()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "dynamic")
    self.physicsObject.shape = love.physics.newPolygonShape(0, -10, 7, 10, 0, 3, -3, 10)
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setSensor(true)
    self.physicsObject.fixture:setUserData(self)
    table.insert(self.world.physicsObjects, self.physicsObject)
end

function Ship:update(dt)
    --local arena = self.world:findByType("Arena")[1]
    if math.abs(self.position.x) > arena.size.x / 2 then
        --self.position.x = - self.position.x
        if self.position.x > 0 then
            self.position.x = - arena.size.x / 2
        else
            self.position.x = arena.size.x / 2
        end
        self.physicsObject.body:setPosition(self.position.x, self.position.y)
    end
    if math.abs(self.position.y) > arena.size.y / 2 then
        --self.position.y = - self.position.y
        if self.position.y > 0 then
            self.position.y = - arena.size.y / 2
        else
            self.position.y = arena.size.y / 2
        end
        self.physicsObject.body:setPosition(self.position.x, self.position.y)
    end

    PolygonEntity.update(self, dt)

    self.particleSystem:setDirection(self.rotation - math.pi)
    self.particleSystem:setSpeed(100)

    if self.velocity ~= self.lastVelocity then
        self.particleSystem:start()
    else
        self.particleSystem:stop()
    end

    --let it slow down
    self.velocity = self.velocity:normalized() * math.max(0, self.velocity:len() - 30 * dt)
    --save velocity for speed comparison for particles
    self.lastVelocity = self.velocity

    self.particleSystem:update(dt)

    local vec_behind_ship = self.position - Vector(6, 0):rotate(self.rotation)
    self.particleSystem:setPosition(vec_behind_ship.x, vec_behind_ship.y)

    if self.timeUntilShoot > 0 then
        self.timeUntilShoot = self.timeUntilShoot - dt
    end

    if self.crashScheduled then
        game:shipCrashed()
    end
end

function Ship:draw()
    love.graphics.setBlendMode("additive")
    love.graphics.setColor(255, 255, 255)
    love.graphics.polygon("line", self:getDrawPoints())
    love.graphics.draw(self.particleSystem)
    love.graphics.setBlendMode("alpha")

    if self.particleSystem:isActive() then
        local vec_behind_ship = self.position - Vector(6, 0):rotate(self.rotation)
        local particle_image_size = Vector(resources.images.particle:getWidth(), resources.images.particle:getHeight())
        local particle_size = 1
        love.graphics.setColor(200, 100, 100, 50)
        love.graphics.draw(resources.images.particle,
                           vec_behind_ship.x -((particle_image_size.x * particle_size) / 2),
                           vec_behind_ship.y -((particle_image_size.y * particle_size) / 2),
                           0,
                           particle_size)
    end
end

function Ship:move(forward, dt)
    --local f = math.max(0, math.min(1, forward))
    local f = math.max(0, math.min(1, forward))

    self.velocity = self.velocity + Vector(f * 500 * dt, 0):rotated(self.rotation)
    --max = 120
    if self.velocity:len() > max_ship_speed then
        self.velocity:normalize()
        self.velocity = self.velocity * max_ship_speed
    end
end

function Ship:shoot()
    if self.timeUntilShoot <= 0 and self.world then
        local b = Bullet(self.position, self.velocity, self.rotation)
        self.world:add(b)
        game:addScore(1)
        self.timeUntilShoot = shoot_delay
    end

    resources.audio.shot:play()
end

function Ship:hitByAsteroid(asteroid) end
