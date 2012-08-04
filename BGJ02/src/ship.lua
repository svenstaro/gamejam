require("util/helper")
require("util/vector")
require("polygonentity")
require("bullet")

Ship = class("Ship", PolygonEntity)

function Ship:__init()
    PolygonEntity.__init(self)
    self.velocity = Vector(20, 10)

    self:addPoint(Vector(0, -10))
    self:addPoint(Vector(7, 10))
    self:addPoint(Vector(0, 3))
    self:addPoint(Vector(-7, 10))

    --self.physicsObject = {}
    self.timeUntilShoot = 0
end

--function Ship:enablePhysics()
--    self.physicsObject.body = love.physics.newBody(self.world.physicsWorld, self.position.x, self.position.y, "dynamic")
--    self.physicsObject.shape = love.physics.newPolygonShape(0, -10, 7, 10, 0, 3, -3, 10)
--    self.physicsObject.fixture = love.physics.newFixture(self.physicsObject.body, self.physicsObject.shape, 1)
--    table.insert(self.world.physicsObjects, self.physicsObject)
--end

function Ship:update(dt)
    PolygonEntity.update(self, dt)

    max_x = 300
    max_y = 200
    if self.position.x > max_x then self.position.x = max_x end
    if self.position.x <-max_x then self.position.x =-max_x end
    if self.position.y > max_y then self.position.y = max_y end
    if self.position.y <-max_y then self.position.y =-max_y end

    if self.timeUntilShoot > 0 then
        self.timeUntilShoot = self.timeUntilShoot - dt
    end
end

function Ship:draw()
    love.graphics.setColor(255, 255, 255)
    love.graphics.polygon("line", self:getDrawPoints())
end

function Ship:shoot()
    if self.timeUntilShoot <= 0 then
        local b = Bullet(self.position, self.velocity, self.rotation)
        self.world:add(b)
        self.timeUntilShoot = 0.5
    end
end
