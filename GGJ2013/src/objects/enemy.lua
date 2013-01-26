-- the player object that moves around the screen

require("core/object")

Enemy = class("Enemy", Object)

function Enemy:__init()
    self.x = 500
    self.y = 400
    self.z = 1
    self.angle = 0
    self.startled = true
end

function Enemy:update(dt)
    self.angle = math.atan2(main.player.y - self.y, main.player.x - self.x)
end

function Enemy:draw()
    love.graphics.setColor(100, 100, 100)
    love.graphics.draw(resources.images.player, self.x, self.y, self.angel, 1, 1, resources.images.player:getWidth() / 2, resources.images.player:getHeight() / 2)
end
