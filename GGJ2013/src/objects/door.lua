-- (Elevator-) doors opening.

require("core/object")
require("vector")

Door = class("Door", Object)

function Door:__init(angle)
    self.open = false
    self.openness = 0

    self.locked = false
    self.proximityOpen = true

    self.x = 0
    self.y = 0
    self.z = 2
    self.angle = angle or math.pi * 0.5
end

function Door:update(dt)
    if self.locked then
        self.open = false
    else
        if self.proximityOpen then
            local d = Vector(main.player.x - self.x, main.player.y - self.y):len()
            local open = d < 100
            if open ~= self.open then
                self:toggle()
            end
        end
    end

    local target = 0
    if self.open then target = 1 end

    self.openness = self.openness + (target - self.openness ) * dt * 10
end

function Door:toggle()
    self.open = not self.open
    -- love.audio.play(resources.audio.door_slide)
    local snd = resources:makeSound("door_slide")
    snd:setPosition(self.x, 0, self.y)
    snd:play()
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
