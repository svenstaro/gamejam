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
end

function Bullet:update(dt)
    PolygonEntity.update(self, dt)
    if self.lifetime >= 3 then
        self.world:remove(self)
    end
end

function Bullet:draw()
    for i = 5, 1, -1 do
        self:drawPart(i)
    end
end

function Bullet:drawPart(size)
    local alpha = math.max(255 * math.min(self.lifetime * 3, 1) - (5-size) * 50, 0)
    love.graphics.setColor(255, 20, 20, alpha)
    local pos = Vector(self.position.x, self.position.y)
    pos = pos - self.own_velocity:normalized() * (5 - size) * 8
    love.graphics.circle("line", pos.x, pos.y, size, 5)
end
