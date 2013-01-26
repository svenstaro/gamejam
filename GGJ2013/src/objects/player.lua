-- the player object that moves around the screen

require("core/object")
require("objects/toast")

Player = class("Player", Object)

function Player:__init()
    self.x = 0
    self.y = 0
    self.z = 1
    self.angle = 0
    self.timeSinceLastStep = 0
end

function Player:update(dt)
    local mx, my = main:getMousePosition()
    self.angle = math.atan2(my - self.y, mx - self.x)

    local dx, dy = 0, 0
    if love.keyboard.isDown("a") then dx = dx - 1 end
    if love.keyboard.isDown("d") then dx = dx + 1 end
    if love.keyboard.isDown("w") then dy = dy - 1 end
    if love.keyboard.isDown("s") then dy = dy + 1 end
    if math.abs(dy) + math.abs(dx) == 2 then
        dx = dx * 0.84
        dy = dy * 0.84
    end

    self.timeSinceLastStep = self.timeSinceLastStep + dt

    if (dx ~= 0 or dy ~= 0) and self.timeSinceLastStep >= 0.3 then
        --self:makeToast("Tap", 0.5 - math.random(), {100, 100, 100, 50})
        -- TODO: add random foot step sound here!
        self.timeSinceLastStep = 0
    end

    local speed = 100
    self.x = self.x + dx * speed * dt
    self.y = self.y + dy * speed * dt

    main.centerX = self.x
    main.centerY = self.y
end

function Player:draw()
    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(resources.images.player, self.x, self.y, self.angle, 1, 1, resources.images.player:getWidth() / 2, resources.images.player:getHeight() / 2)
end
