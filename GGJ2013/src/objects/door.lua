-- the player object that moves around the screen

require("core/object")

Door = class("Door", Object)

function Door:__init(angle)
    self.open = false
    self.openness = 0

    self.x = 100
    self.y = 300
    self.z = 1
    self.angle = angle or math.pi * 0.5

end

function Door:update(dt)
    local target = 0
    if self.open then target = 1 end

    self.openness = self.openness + (target - self.openness ) * dt * 10
end

function Door:toggle()
    self.open = not self.open
end

function Door:draw()
    love.graphics.push()

    love.graphics.translate(self.x, self.y)
    love.graphics.rotate(self.angle)

    love.graphics.setColor(255, 255, 255)
    love.graphics.rectangle("fill", - 64 - self.openness * 50, -10, 64, 20)
    love.graphics.rectangle("fill", self.openness * 50, -10, 64, 20)

    love.graphics.pop()
end
