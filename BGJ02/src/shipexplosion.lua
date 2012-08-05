require("util/helper")
require("util/vector")
require("entity")

ShipExplosion = class("ShipExplosion", Entity)

function ShipExplosion:__init(position, size)
    Entity.__init(self)
    self.position = position
    self.size = size
    self.opacity = 255

    tween(1, self, { size = 0.2 }, "outQuint")
    tween(1, self, { opacity = 0 }, "inQuint")
end

function ShipExplosion:update(dt)
    Entity.update(self, dt)
    if self.lifetime >= 1 then
        self:kill()
    end
end

function ShipExplosion:draw()
    love.graphics.setBlendMode("additive")
    local ring_image_size = Vector(resources.images.ring:getWidth(), resources.images.ring:getHeight())
    local particle_image_size = Vector(resources.images.particle:getWidth(), resources.images.particle:getHeight())
    love.graphics.setColor(100, 100, 255, self.opacity)
    love.graphics.draw(resources.images.ring,
                       self.position.x -(ring_image_size.x * self.size) / 2,
                       self.position.y -(ring_image_size.y * self.size) / 2,
                       0,
                       self.size)
    love.graphics.draw(resources.images.ring,
                       self.position.x -((ring_image_size.x * self.size) / 2) * 1.2,
                       self.position.y -((ring_image_size.y * self.size) / 2) * 0.5,
                       0,
                       self.size * 1.2,
                       self.size * 0.5)
    love.graphics.setColor(255, 255, 255, self.opacity)
    love.graphics.draw(resources.images.particle,
                       self.position.x -((particle_image_size.x * self.size) / 2) * 2,
                       self.position.y -((particle_image_size.y * self.size) / 2) * 2,
                       0,
                       self.size * 2)
    love.graphics.setBlendMode("alpha")
end
