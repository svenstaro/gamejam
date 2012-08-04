require("util/helper")
require("util/vector")
require("polygonentity")

Ship = class("Ship", PolygonEntity)

function Ship:__init()
    PolygonEntity.__init(self)
    self.velocity = Vector(20, 10)

    self:addPoint(Vector(0, -10))
    self:addPoint(Vector(7, 10))
    self:addPoint(Vector(0, 3))
    self:addPoint(Vector(-7, 10))
end

function Ship:update(dt)
    speed_add = 0
    if love.keyboard.isDown("up") then
        speed_add = 500
    elseif  love.keyboard.isDown("down") then
        speed_add = -500
    end
    self.velocity = self.velocity + Vector(0, -speed_add * dt):rotated(self.rotation)
    max = 200
    if self.velocity:len() > max then
        self.velocity:normalize()
        self.velocity = self.velocity * max
    end

    turn_speed = math.pi
    if love.keyboard.isDown("left") then
        self.rotation = self.rotation - turn_speed * dt
    elseif love.keyboard.isDown("right") then
        self.rotation = self.rotation + turn_speed * dt
    end

    PolygonEntity.update(self, dt)

    max_x = 300
    max_y = 200
    if self.position.x > max_x then self.position.x = max_x end
    if self.position.x <-max_x then self.position.x =-max_x end
    if self.position.y > max_y then self.position.y = max_y end
    if self.position.y <-max_y then self.position.y =-max_y end
end

function Ship:draw()
    love.graphics.setColor(255, 255, 255)
    love.graphics.polygon("line", self:getDrawPoints())
end
