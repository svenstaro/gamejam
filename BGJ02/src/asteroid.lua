require("polygonentity")

Asteroid = class("Asteroid", PolygonEntity)

--[[
  Asteroids come in 3 sizes:

  # SIZE    MATERIAL    RADIUS
  1 small   1           5-10
  2 medium  5           15-25
  3 large   20          35-50

  Each asteroid splits into 3 of the smaller level, the smalles asteroid breaks away.
]]

function materialValue(size)
    v = {1, 5, 20}
    return v[size]
end

function randomRadius(size)
    v1 = {5, 10, 15}
    v2 = {10, 20, 30}
    return math.random(v1[size], v2[size])
end

function pointCount(size)
    return size * 5
end

function Asteroid:__init(size)
    PolygonEntity.__init(self)   

    self.size = size
    self:generate()
    self.rotationSpeed = math.random(-1.0, 1.0)

    self.physicsObject = {}
end

function Asteroid:enablePhysics()
    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "dynamic")
    self.physicsObject.shape = love.physics.newCircleShape(randomRadius(self.size))
    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
    self.physicsObject.fixture:setUserData(self)
    self.physicsObject.body:setLinearVelocity(self.velocity.x, self.velocity.y)
    table.insert(self.world.physicsObjects, self.physicsObject)
end

function Asteroid:generate()
    self:clear()
    c = pointCount(self.size)
    for i = 1,c do
        v = Vector(0, randomRadius(self.size))
        v:rotate(2.0 * math.pi * i / c)
        self:addPoint(v)
    end
end

function Asteroid:crush()
    if self.size > 1 then
        for i = 1,3 do
            local a = Asteroid(self.size - 1)
            a.position = self.position
            a.velocity = Vector(math.random(-40, 40), math.random(-40, 40)) + self.velocity * 0.7
            self.world:add(a)
        end
    end

    self.world:remove(self)
end
